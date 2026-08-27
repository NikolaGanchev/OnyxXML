#include "node.h"

#include <algorithm>
#include <sstream>
#include <stack>
#include <unordered_map>

#include "index.h"
#include "text.h"

namespace onyx::dynamic {
Node::Node()
    : attributes{},
      firstChild{nullptr},
      prevSibling{this},
      nextSibling{this},
      indices{},
      parent{nullptr} {
    this->setFlag<FlagBitIndices::BIT_IS_OWNING>(true);
}

Node::Node(NonOwningNodeTag)
    : attributes{},
      firstChild{nullptr},
      prevSibling{this},
      nextSibling{this},
      indices{},
      parent{nullptr} {
    this->setFlag<FlagBitIndices::BIT_IS_OWNING>(false);
}

Node::Node(Node&& other) noexcept
    : attributes{std::move(other.attributes)},
      firstChild{other.firstChild},
      indices{std::move(other.indices)},
      flags{other.flags},
      parent{other.parent} {
    if (other.nextSibling == &other) {
        this->prevSibling = this;
        this->nextSibling = this;
    } else {
        this->prevSibling = other.prevSibling;
        this->nextSibling = other.nextSibling;
    }

    other.parent = nullptr;
    other.firstChild = nullptr;
    other.prevSibling = nullptr;
    other.nextSibling = nullptr;
    this->takeOverIndices(other);

    this->iterateDirectChildren([this](Node* child) { child->parent = this; });
}

Node::Node(std::vector<Attribute> attributes,
           std::vector<NodeHandle>&& children)
    : attributes{std::move(attributes)},
      indices{},
      firstChild{nullptr},
      prevSibling{this},
      nextSibling{this},
      parent{nullptr} {
    this->setFlag<FlagBitIndices::BIT_IS_OWNING>(true);
    for (auto& child : children) {
        if (child.owning() != this->isOwning()) {
            throw std::invalid_argument(
                "Mixing Nodes with different ownership modes");
        }
    }

    for (auto& child : children) {
        this->attachChildBack(child.release());
        this->firstChild->prevSibling->parent = this;
    }
}

Node::Node(NonOwningNodeTag, std::vector<Attribute> attributes,
           std::vector<NodeHandle>&& children)
    : attributes{std::move(attributes)},
      indices{},
      firstChild{nullptr},
      prevSibling{this},
      nextSibling{this},
      parent{nullptr} {
    this->setFlag<FlagBitIndices::BIT_IS_OWNING>(false);
    for (auto& child : children) {
        if (child.owning() != this->isOwning()) {
            throw std::invalid_argument(
                "Mixing Nodes with different ownership modes");
        }

        this->attachChildBack(child.release());
        this->firstChild->prevSibling->parent = this;
    }
}

Node& Node::operator=(Node&& other) noexcept {
    if (this == &other) return *this;
    this->destroy();
    this->attributes = std::move(other.attributes);
    this->firstChild = other.firstChild;
    this->flags = other.flags;

    if (other.nextSibling == &other) {
        this->prevSibling = this;
        this->nextSibling = this;
    } else {
        this->prevSibling = other.prevSibling;
        this->nextSibling = other.nextSibling;
    }

    other.firstChild = nullptr;
    other.prevSibling = nullptr;
    other.nextSibling = nullptr;

    this->indices = std::move(other.indices);

    this->setFlag<FlagBitIndices::BIT_IS_OWNING>(other.isOwning());

    this->takeOverIndices(other);
    this->parent = other.parent;
    other.parent = nullptr;

    this->iterateDirectChildren([this](Node* child) { child->parent = this; });

    return *this;
}

void Node::takeOverIndices(Node& other) {
    for (auto& index : this->indices) {
        if (index->getRoot() == &other) {
            index->root = this;
        }
    }
}

void Node::processConstructorAttribute(Attribute&& attribute) {
    for (auto& attr : this->attributes) {
        if (attr.getName() == attribute.getName()) {
            processConstructorObjectMoveCleanup();
            throw std::invalid_argument("Adding duplicate Attribute");
        }
    }

    this->attributes.push_back(attribute);
}

void Node::processConstructorObjectMoveCleanup() {
    this->iterateDirectChildren([](Node* child) { delete child; });
}

void Node::destroy() {
    // In owning trees, a Node can only leave the tree using removeChild()
    // removeChild() of course removes the Node from all indices
    // Upon destruction of an owning tree, it is surely known that any
    // indices applied on the tree will be invalidated In turn meaning that
    // the Node does not need to be removed (which is not guaranteed to be a
    // cheap operation)
    // In non-owning trees this is not guaranteed, as the
    // destructor of a Node in the tree can be called arbitrarily
    if (this->parent && !this->parent->isOwning()) {
        this->iterativeProcessor([this](Node* obj) -> void {
            this->propagateIndexUpdateUp(obj, IndexPropagationMessage::REMOVE);
        });
    }

    for (auto index : this->indices) {
        index->invalidate();
    };

    this->iterateDirectChildren([](Node* child) { child->parent = nullptr; });

    if (this->isOwning()) {
        this->iterateDirectChildren([](Node* child) {
            child->parent = nullptr;
            child->prevSibling = nullptr;
            child->nextSibling = nullptr;

            delete child;
        });
    }

    if (this->parent && !this->parent->isOwning()) {
        // In non-owning trees, nodes are not guaranteed to be sequentially
        // destroyed, so they need to manually remove themselves from the
        // parent's children to guarantee no dangling pointers are left
        if (this == this->parent->firstChild) {
            if (this == this->nextSibling) {
                this->parent->firstChild = nullptr;
            } else {
                this->parent->firstChild = this->nextSibling;
            }
        }

        this->prevSibling->nextSibling = this->nextSibling;
        this->nextSibling->prevSibling = this->prevSibling;
    }
}

Node::~Node() { this->destroy(); }

Node* Node::addChild(NodeHandle newChild) {
    if (isVoid()) {
        throw std::runtime_error("Void " + getTagName() +
                                 " cannot have children.");
    }
    if (newChild->isInTree()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() +
                                 " that is already a child of another Object.");
    }
    if (newChild.owning() != this->isOwning()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() +
                                 " with different owning mode.");
    }

    newChild->parent = this;

    newChild->iterativeProcessor([&newChild](Node* current) -> void {
        newChild->propagateIndexUpdateUp(current, IndexPropagationMessage::PUT);
    });

    Node* newChildRef = newChild.get();

    this->attachChildBack(newChild.release());

    return newChildRef;
}

Node* Node::addChild(std::unique_ptr<Node> child) {
    return addChild(NodeHandle(std::move(child)));
}

Node* Node::addChild(Node* child) { return addChild(NodeHandle(child, false)); }

std::vector<Node*> Node::getChildren() const {
    std::vector<Node*> res;

    this->iterateDirectChildren(
        [&res](const Node* child) { res.push_back(const_cast<Node*>(child)); });

    return res;
}

size_t Node::getChildrenCount() const {
    size_t count = 0;

    this->iterateDirectChildrenReverse(
        [&count](const Node* current) { count++; });

    return count;
}

void Node::addIndex(Node::Index* index) {
    iterativeProcessor(
        [&index](Node* obj) -> void { index->putIfNeeded(obj); });
}

void Node::removeIndex(Node::Index* indexToRemove) {
    iterativeProcessor([&indexToRemove](Node* obj) -> void {
        indexToRemove->removeIfNeeded(obj);
    });
    for (std::forward_list<Index*>::iterator
             prev = this->indices.before_begin(),
             index = this->indices.begin();
         index != this->indices.end();) {
        if (indexToRemove == *index) {
            this->indices.erase_after(prev);
            break;
        } else {
            index++;
            prev++;
        }
    }
}

void Node::replaceIndex(Node::Index* oldIndex, Node::Index* newIndex) {
    for (auto index = this->indices.begin(); index != this->indices.end();) {
        if (oldIndex == *index) {
            *index = newIndex;
            break;
        } else {
            index++;
        }
    }
}

void Node::indexUpdate(Node* updated, IndexPropagationMessage message) {
    for (auto index : this->indices) {
        switch (message) {
            case IndexPropagationMessage::PUT: {
                index->putIfNeeded(updated);
                break;
            }
            case IndexPropagationMessage::UPDATE: {
                index->update(updated);
                break;
            }
            case IndexPropagationMessage::REMOVE: {
                index->removeIfNeeded(updated);
                break;
            }
            default: {
                throw std::runtime_error("Unhandled message type");
            }
        }
    }
}

void Node::propagateIndexUpdateUp(Node* updated,
                                  IndexPropagationMessage message) {
    Node* parent = this->parent;
    while (parent) {
        parent->indexUpdate(updated, message);
        parent = parent->parent;
    }
}

void Node::updateAndPropagateUp(IndexPropagationMessage message) {
    this->indexUpdate(this, message);
    this->propagateIndexUpdateUp(this, message);
}

bool Node::isInTree() const { return this->parent != nullptr; }

bool Node::isOwning() const {
    return this->getFlag<FlagBitIndices::BIT_IS_OWNING>();
}

Node* Node::getParentNode() const { return this->parent; }

std::vector<Node*> Node::getChildrenByAttribute(
    const std::string& attribute, const std::string& value) const {
    return iterativeChildrenParse(([&attribute, &value](Node* obj) -> bool {
        return obj->hasAttribute(attribute) &&
               obj->getAttributeValue(attribute) == value;
    }));
}

std::vector<Node*> Node::getChildrenByAttributeName(
    const std::string& attribute) const {
    return iterativeChildrenParse(([&attribute](Node* obj) -> bool {
        return obj->hasAttribute(attribute);
    }));
}

std::vector<Node*> Node::getChildrenByClassName(
    const std::string& className) const {
    return getChildrenByAttribute("class", className);
}

std::vector<Node*> Node::getChildrenByTagName(
    const std::string& tagName) const {
    return iterativeChildrenParse(
        [&tagName](Node* obj) -> bool { return obj->getTagName() == tagName; });
}

std::vector<Node*> Node::getChildrenByName(const std::string& name) const {
    return getChildrenByAttribute("name", name);
}

std::vector<Node*> Node::getChildrenById(const std::string& id) const {
    return getChildrenByAttribute("id", id);
}

const std::vector<Attribute>& Node::getAttributes() const {
    return this->attributes;
}

NodeHandle Node::removeChild(Node* childToRemove) {
    bool foundThis = false;
    Node* par = childToRemove->parent;
    while (par != nullptr) {
        if (par == this) {
            foundThis = true;
            break;
        }
        par = par->parent;
    }

    if (!foundThis) return nullptr;

    if (childToRemove == childToRemove->parent->firstChild) {
        if (childToRemove->nextSibling == childToRemove) {
            childToRemove->parent->firstChild = nullptr;
        } else {
            childToRemove->parent->firstChild = childToRemove->nextSibling;
        }
    }

    childToRemove->prevSibling->nextSibling = childToRemove->nextSibling;
    childToRemove->nextSibling->prevSibling = childToRemove->prevSibling;

    childToRemove->iterativeProcessor(
        [this, &childToRemove](Node* obj) -> void {
            childToRemove->propagateIndexUpdateUp(
                obj, IndexPropagationMessage::REMOVE);
        });

    childToRemove->prevSibling = childToRemove;
    childToRemove->nextSibling = childToRemove;
    childToRemove->parent = nullptr;

    return NodeHandle(childToRemove, this->isOwning());
}

std::unique_ptr<Node> Node::deepCopy() const {
    std::unique_ptr<Node> root = this->shallowCopy();

    struct ParseNode {
        const Node* obj;
        Node* copy;
    };

    std::vector<ParseNode> s;

    s.emplace_back(ParseNode{this, root.get()});

    while (!s.empty()) {
        ParseNode node = s.back();
        s.pop_back();
        const Node* obj = node.obj;

        if (!(obj->isVoid())) {
            obj->iterateDirectChildren([&s, &node](const Node* current) {
                std::unique_ptr<Node> child = current->shallowCopy();
                s.emplace_back(ParseNode{current, child.get()});
                node.copy->addChild(std::move(child));
            });
        }
    }
    return root;
}

bool Node::shallowEquals(const Node& other) const {
    if (this == &other) return true;
    if (this->isVoid() != other.isVoid()) return false;
    if (this->getTagName() != other.getTagName()) return false;
    if (this->attributes.size() != other.attributes.size()) return false;
    if (this->getChildrenCount() != other.getChildrenCount()) return false;

    std::vector<const Attribute*> attributes1;
    std::vector<const Attribute*> attributes2;

    for (size_t i = 0; i < this->attributes.size(); i++) {
        attributes1.push_back(&(this->attributes[i]));
        attributes2.push_back(&(other.attributes[i]));
    }

    std::sort(attributes1.begin(), attributes1.end(),
              [](const Attribute* lhs, const Attribute* rhs) {
                  return lhs->getName() < rhs->getName();
              });

    std::sort(attributes2.begin(), attributes2.end(),
              [](const Attribute* lhs, const Attribute* rhs) {
                  return lhs->getName() < rhs->getName();
              });

    for (size_t i = 0; i < attributes1.size(); i++) {
        if ((*attributes1[i]) != (*attributes2[i])) {
            return false;
        }
    }

    return true;
}

bool Node::deepEquals(const Node& other) const {
    struct ParseNode {
        const Node* obj;
        const Node* other;
    };

    std::vector<ParseNode> s;

    s.emplace_back(ParseNode{this, &other});

    while (!s.empty()) {
        ParseNode node = s.back();
        s.pop_back();
        const Node* obj = node.obj;
        const Node* other = node.other;

        if (!obj->shallowEquals(*other)) return false;

        if (!(obj->isVoid())) {
            Node* current = obj->firstChild;
            Node* original = current;
            Node* currentOther = other->firstChild;
            Node* originalOther = currentOther;
            if (current == nullptr || currentOther == nullptr) {
                continue;
            }
            do {
                // Because of obj->shallowEquals(*other) succeeding, it is
                // known that at this point the two nodes have the same
                // amount of children
                s.emplace_back(ParseNode{current, currentOther});
                current = current->nextSibling;
                currentOther = currentOther->nextSibling;
            } while (current != original && currentOther != originalOther);
        }
    }
    return true;
}

size_t Node::size() const {
    size_t size = 1;
    iterativeChildrenParse([&size](Node* cur) -> bool {
        size++;
        return false;
    });
    return size;
}

size_t Node::depth() const {
    std::vector<const Node*> s;
    size_t maxDepth = 0;
    size_t depth = 0;

    s.emplace_back(this);

    while (!s.empty()) {
        const Node* obj = s.back();
        s.pop_back();

        if (obj == nullptr) {
            depth--;
            continue;
        }

        if (!(obj->isVoid())) {
            if (obj->firstChild != nullptr) {
                depth++;
                if (depth > maxDepth) {
                    maxDepth = depth;
                }
                s.emplace_back(nullptr);
                obj->iterateDirectChildrenReverse(
                    [&s](const Node* child) { s.emplace_back(child); });
            }
        }
    }

    return maxDepth;
}

size_t Node::leafCount() const {
    std::vector<const Node*> s;
    size_t leaves = 0;

    s.emplace_back(this);

    while (!s.empty()) {
        const Node* obj = s.back();
        s.pop_back();

        if (!(obj->isVoid())) {
            if (obj->firstChild != nullptr) {
                obj->iterateDirectChildrenReverse(
                    [&s](const Node* current) { s.emplace_back(current); });
            } else {
                leaves++;
            }
        } else {
            leaves++;
        }
    }

    return leaves;
}

bool Node::hasAttribute(const std::string& name) const {
    for (auto& attr : this->attributes) {
        if (attr.getName() == name) {
            return true;
        }
    }

    return false;
}

const std::string& Node::getAttributeValue(const std::string& name) const {
    for (auto& attr : this->attributes) {
        if (attr.getName() == name) {
            return attr.getValue();
        }
    }

    throw std::runtime_error("Trying to get Attribute which does not exist: " +
                             name);
}

void Node::setAttributeValue(const std::string& name,
                             const std::string& newValue) {
    for (auto& attr : this->attributes) {
        if (attr.getName() == name) {
            attr.setValue(newValue);

            updateAndPropagateUp(IndexPropagationMessage::UPDATE);
            return;
        }
    }

    this->attributes.emplace_back(name, newValue);
    updateAndPropagateUp(IndexPropagationMessage::UPDATE);
}

void Node::removeAttribute(const std::string& name) {
    for (auto index = this->attributes.begin();
         index != this->attributes.end();) {
        if (index->getName() == name) {
            this->attributes.erase(index);

            updateAndPropagateUp(IndexPropagationMessage::UPDATE);

            return;
        }
    }
}

bool Node::hasSpecialSerialization() const { return false; }

std::string Node::serialize() const {
    std::vector<SerializationNode> s;

    std::ostringstream result;
    result.imbue(std::locale::classic());

    std::vector<const Attribute*> attributes;

    s.emplace_back(SerializationNode{this, false});

    while (!s.empty()) {
        SerializationNode& node = s.back();
        const Node* obj = node.obj;

        const std::string& tagName = obj->getTagName();

        if (node.visited) {
            if (obj->hasSpecialSerialization()) {
                const Node::SpecialSerializable* objSpecialSerializable =
                    dynamic_cast<const Node::SpecialSerializable*>(obj);
                objSpecialSerializable->specialSerialize(s, result);
                continue;
            }
            result << "</" << tagName << ">";
            s.pop_back();
            continue;
        }
        node.visited = true;

        if (obj->hasSpecialSerialization()) {
            const Node::SpecialSerializable* objSpecialSerializable =
                dynamic_cast<const Node::SpecialSerializable*>(obj);
            objSpecialSerializable->specialSerialize(s, result);
            continue;
        }

        result << "<" << tagName;

        attributes.clear();
        for (size_t i = 0; i < obj->attributes.size(); i++) {
            attributes.push_back(&(obj->attributes[i]));
        }

        for (const auto& attr : attributes) {
            result << " " << attr->getName() << "=\""
                   << (attr->shouldEscape()
                           ? text::escape(attr->getValue(),
                                          attr->shouldEscapeMultiByte())
                           : attr->getValue())
                   << "\"";
        }

        if (!(obj->isVoid())) {
            if (obj->firstChild != nullptr) {
                result << ">";

                obj->iterateDirectChildrenReverse([&s](const Node* current) {
                    s.emplace_back(SerializationNode{current, false});
                });
            } else {
                result << "></" << tagName << ">";
                s.pop_back();
                continue;
            }
        } else {
            result << "/>";
            s.pop_back();
            continue;
        }
    }

    return result.str();
}

std::string Node::serializePretty(const std::string& indentationSequence,
                                  bool sortAttributes) const {
    std::vector<SerializationNode> s;

    std::string indentation;
    std::ostringstream result;
    result.imbue(std::locale::classic());

    std::vector<const Attribute*> attributes;

    s.emplace_back(SerializationNode{this, false});

    while (!s.empty()) {
        SerializationNode& node = s.back();
        const Node* obj = node.obj;

        if (obj == nullptr) {
            indentation.resize(indentation.size() - indentationSequence.size());
            s.pop_back();
            continue;
        }

        const std::string& tagName = obj->getTagName();

        if (node.visited) {
            if (obj->hasSpecialSerialization()) {
                const Node::SpecialSerializable* objSpecialSerializable =
                    dynamic_cast<const Node::SpecialSerializable*>(obj);
                objSpecialSerializable->specialSerializePretty(
                    s, result, indentation, indentationSequence,
                    sortAttributes);
                continue;
            }
            result << indentation << "</" << tagName << ">\n";
            s.pop_back();
            continue;
        }
        node.visited = true;

        if (obj->hasSpecialSerialization()) {
            const Node::SpecialSerializable* objSpecialSerializable =
                dynamic_cast<const Node::SpecialSerializable*>(obj);
            objSpecialSerializable->specialSerializePretty(
                s, result, indentation, indentationSequence, sortAttributes);
            continue;
        }

        result << indentation << "<" << tagName;

        attributes.clear();
        for (size_t i = 0; i < obj->attributes.size(); i++) {
            attributes.push_back(&(obj->attributes[i]));
        }
        if (sortAttributes) {
            std::sort(attributes.begin(), attributes.end(),
                      [](const Attribute* lhs, const Attribute* rhs) {
                          return lhs->getName() < rhs->getName();
                      });
        }

        for (const auto& attr : attributes) {
            result << " " << attr->getName() << "=\""
                   << (attr->shouldEscape()
                           ? text::escape(attr->getValue(),
                                          attr->shouldEscapeMultiByte())
                           : attr->getValue())
                   << "\"";
        }

        if (!(obj->isVoid())) {
            if (obj->firstChild != nullptr) {
                result << ">\n";
                s.emplace_back(SerializationNode{nullptr, false});
                indentation += indentationSequence;

                obj->iterateDirectChildrenReverse([&s](const Node* current) {
                    s.emplace_back(SerializationNode{current, false});
                });
            } else {
                result << "></" << tagName << ">\n";
                s.pop_back();
                continue;
            }
        } else {
            result << "/>\n";
            s.pop_back();
            continue;
        }
    }

    std::string strResult = result.str();

    if (strResult[strResult.size() - 1] == '\n') {
        strResult.pop_back();
    }

    return strResult;
}

Node::ObservableStringRef::ObservableStringRef(std::string* ref, Node* origin)
    : ptr(ref), origin(origin) {}

Node::ObservableStringRef::operator const std::string*() const {
    return this->ptr;
}

const std::string* Node::ObservableStringRef::operator->() const {
    return this->ptr;
}

const std::string& Node::ObservableStringRef::operator*() const {
    return *this->ptr;
}

bool Node::ObservableStringRef::operator==(const std::string& str) const {
    return *this->ptr == str;
}

bool Node::ObservableStringRef::operator!=(const std::string& str) const {
    return !(*this == str);
}

Node::ObservableStringRef& Node::ObservableStringRef::operator=(
    std::string newPtr) {
    if (*this->ptr != newPtr) {
        *this->ptr = newPtr;
        this->origin->updateAndPropagateUp(IndexPropagationMessage::UPDATE);
    }
    return *this;
}

Node::ObservableStringRef Node::operator[](const std::string& name) {
    if (!hasAttribute(name)) {
        setAttributeValue(name, "");
    }

    for (auto& attr : this->attributes) {
        if (attr.getName() == name) {
            return ObservableStringRef(&(attr.getValueMutable()), this);
        }
    }

    throw std::logic_error(
        "Unreachable: attribute not found even after being inserted. Check for "
        "memory or multithreading issues.");
}

Node& Node::operator+=(NodeHandle right) {
    addChild(std::move(right));
    return (*this);
}

std::string Node::indentationSequence = "\t";

void Node::setIndentationSequence(const std::string& newSequence) {
    indentationSequence = newSequence;
}

const std::string& Node::getIndentationSequence() {
    return indentationSequence;
}

bool Node::sortAttributes = false;

void Node::setSortAttributes(bool shouldSort) { sortAttributes = shouldSort; }

bool Node::getSortAttributes() { return sortAttributes; }

const Node* Node::getFirstChild() const { return this->firstChild; }

const Node* Node::getLastChild() const {
    return this->firstChild ? this->firstChild->prevSibling : nullptr;
}

const Node* Node::getPrevSibling() const { return this->prevSibling; }

const Node* Node::getNextSibling() const { return this->nextSibling; }

Node* Node::getFirstChild() { return this->firstChild; }

Node* Node::getLastChild() {
    return this->firstChild ? this->firstChild->prevSibling : nullptr;
}

Node* Node::getPrevSibling() { return this->prevSibling; }

Node* Node::getNextSibling() { return this->nextSibling; }

void Node::attachChildBack(Node* child) {
    if (this->firstChild) {
        this->firstChild->prevSibling->nextSibling = child;
        child->prevSibling = this->firstChild->prevSibling;
        this->firstChild->prevSibling = child;
        child->nextSibling = this->firstChild;
    } else {
        this->firstChild = child;
        child->prevSibling = child;
        child->nextSibling = child;
    }
}

std::string Node::getStringValue() const {
    std::stringstream res;

    iterativeChildrenParse([&res](const Node* obj) -> bool {
        if (obj->getXPathType() == XPathType::TEXT) {
            res << obj->getStringValue();
        }

        return false;
    });

    return res.str();
}

Node::XPathType Node::getXPathType() const { return XPathType::ELEMENT; }
}  // namespace onyx::dynamic