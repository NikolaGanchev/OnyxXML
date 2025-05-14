#include "node.h"
#include "text.h"
#include "index.h"

#include <unordered_map>
#include <sstream>
#include <stack>
#include <algorithm>

namespace Templater::dynamic {
    Node::Node()
        : attributes{}, children{}, indices{}, parent{nullptr}, _isOwning(true) { }

    Node::Node(NonOwningNodeTag)
        : attributes{}, children{}, indices{}, parent{nullptr}, _isOwning(false) { }

    Node::Node(Node&& other) noexcept
        : attributes{std::move(other.attributes)}, 
        children{std::move(other.children)}, 
        indices{std::move(other.indices)}, 
        parent{other.parent}, 
        _isOwning(other._isOwning) {
        other.parent = nullptr;
        this->takeOverIndices(other);
        for (auto& child: this->children) {
            child->parent = this;
        }
    }

    Node::Node(std::vector<Attribute> attributes, std::vector<NodeHandle>&& children)
        : attributes{std::move(attributes)}, indices{}, parent{nullptr}, _isOwning(true) {
        for (auto& child: children) {
            if (child.owning() != this->_isOwning) {
                throw std::invalid_argument("Mixing Nodes with different ownership modes");
            }
        }
        for (auto& child: children) {
            this->children.push_back(child.release());
        }
        for (auto& child: this->children) {
            child->parent = this;
        }
    }

    
    Node::Node(NonOwningNodeTag, std::vector<Attribute> attributes, std::vector<NodeHandle>&& children)
        : attributes{std::move(attributes)}, indices{}, parent{nullptr}, _isOwning(false) {
        for (auto& child: children) {
            if (child.owning() != this->_isOwning) {
                throw std::invalid_argument("Mixing Nodes with different ownership modes");
            }
            this->children.push_back(child.release());
        }
        for (auto& child: this->children) {
            child->parent = this;
        }
    }
    
    Node& Node::operator=(Node&& other) noexcept {
        if (this == &other) return *this;
        this->destroy();
        this->attributes = std::move(other.attributes);
        this->children = std::move(other.children);
        this->indices = std::move(other.indices);
        this->_isOwning = other._isOwning;

        this->takeOverIndices(other);
        this->parent = other.parent;
        other.parent = nullptr;

        for (auto& child: this->children) {
            child->parent = this;
        }

        return *this;
    }

    void Node::takeOverIndices(Node& other) {
        for (auto& index: this->indices) {
            if (index->getRoot() == &other) {
                index->root = this;
            }
        }
    }

    void Node::processConstructorAttribute(Attribute&& attribute) {
        this->operator[](attribute.getName()) = attribute.getValue();
    }

    void Node::destroy() {
        for (auto& child: this->children) {
            child->parent = nullptr;

            this->indexParse([this, &child](Node::Index* id) -> void {
                if (id->getRoot() == this) {
                    child->removeIndex(id);
                }
            });
        }

        this->indexParse([this](Node::Index* id) -> void {
            if (id->getRoot() == this) {
                id->invalidate();
            }
        });

        if (this->_isOwning) {
            for (auto& child: this->children) {
                delete child;
            }
        }
    }

    Node::~Node() {
        this->destroy();
    }
    
    Node* Node::addChild(NodeHandle newChild)  {
        if (isVoid()) {
            throw std::runtime_error("Void " + getTagName() + " cannot have children.");
        }
        if (newChild->isInTree()) {
            throw std::runtime_error("Attempted to add child to " + getTagName() + " that is already a child of another Object.");
        }
        if (newChild.owning() != this->_isOwning) {
            throw std::runtime_error("Attempted to add child to " + getTagName() + " with different owning mode.");
        }

        newChild->parent = this;
        this->indexParse([&newChild](Node::Index* id) -> void {
            newChild->addIndex(id);
        });

        Node* newChildRef = newChild.get();

        this->children.push_back(newChild.release());

        return newChildRef;
    }
    
    Node* Node::addChild(std::unique_ptr<Node> child) {
        return addChild(NodeHandle(std::move(child)));
    }

    Node* Node::addChild(Node* child) {
        return addChild(NodeHandle(child, false));
    }

    std::vector<Node*> Node::getChildren() const {
        return std::vector<Node*>(this->children);
    }

    size_t Node::getChildrenCount() const {
        return this->children.size();
    }

    void Node::indexParse(const std::function<void(Node::Index*)>& callback) {
        for (auto index: this->indices) {
            callback(index);
            index++;
        }
    }

    // Iteratively adds the index to this node and all its children
    void Node::addIndex(Node::Index* index) {
        iterativeProcessor([&index](Node* obj) -> void {
            obj->indices.push_back(index);
            index->putIfNeeded(obj);
        });
    }

    void Node::removeIndex(Node::Index* indexToRemove) {
        iterativeProcessor([&indexToRemove](Node* obj) -> void {
            for (auto index = obj->indices.begin(); index != obj->indices.end();) {
                if (indexToRemove == *index) {
                    obj->indices.erase(index);
                    indexToRemove->removeIfNeeded(obj);
                    break;
                } else {
                    index++;
                }
            }
        });
    }

    void Node::replaceIndex(Node::Index* oldIndex, Node::Index* newIndex) {
        iterativeProcessor([&oldIndex, &newIndex](Node* obj) -> void {
            for (auto index = obj->indices.begin(); index != obj->indices.end();) {
                if (oldIndex == *index) {
                    *index = newIndex;
                    break;
                } else {
                    index++;
                }
            }
        });
    }

    void Node::iterativeProcessor(const std::function<void(Node*)>& process) {
        std::vector<Node*> s;

        s.push_back(this);

        while(!s.empty()) {
            Node* obj = s.back();

            process(obj);

            s.pop_back();

            const std::vector<Node*>& children = obj->children;
            for (size_t i = 0; i < children.size(); i++) {
                s.push_back(children[i]);
            }
        }
    }

    bool Node::isInTree() const {
        return this->parent != nullptr;
    }

    Node* Node::getParentNode() const {
        return this->parent;
    }

    std::vector<Node*> Node::iterativeChildrenParse(const std::function<bool(Node*)>& condition) const {
        std::vector<Node*> s;
        std::vector<Node*> result;

        for (size_t i = 0; i < this->children.size(); i++) {
            s.push_back(this->children[i]);
        }

        while(!s.empty()) {
            Node* obj = s.back();

            if (condition(obj)) {
                result.push_back(obj);
            }

            s.pop_back();

            auto& children = obj->children;
            for (size_t i = children.size(); i > 0; i--) {
                s.push_back(children[i - 1]);
            }
        }

        return result;
    }

    std::vector<Node*> Node::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
        return iterativeChildrenParse(([&attribute, &value](Node* obj) -> bool 
        {  return obj->hasAttribute(attribute) && obj->getAttributeValue(attribute) == value; }));
    }

    std::vector<Node*> Node::getChildrenByAttributeName(const std::string& attribute) const {
        return iterativeChildrenParse(([&attribute](Node* obj) -> bool 
        {  return obj->hasAttribute(attribute); }));
    }

    std::vector<Node*> Node::getChildrenByClassName(const std::string& className) const {

        return getChildrenByAttribute("class", className);
    }

    std::vector<Node*> Node::getChildrenByTagName(const std::string& tagName) const {
        return iterativeChildrenParse([&tagName](Node* obj) -> bool 
            { return obj->getTagName() == tagName; });
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
        if (!childToRemove->isInTree()) return nullptr;

        std::vector<std::vector<Node*>*> s;

        s.push_back(&(this->children));

        while(!s.empty()) {
            std::vector<Node*>* children = s.back();

            for (size_t i = 0; i < children->size(); i++) {
                if (children->at(i) == childToRemove) {
    
                    this->indexParse([&childToRemove, this](Node::Index* id) -> void {
                        if (id->getRoot() == this) {
                            childToRemove->removeIndex(id);
                        }
                    });
    
                    childToRemove->parent = nullptr;
                    NodeHandle ref(children->operator[](i), this->_isOwning);
                    
                    children->erase(children->begin() + i);
    
                    return ref;
                }
            }

            s.pop_back();

            for (size_t i = 0; i < children->size(); i++) {
                s.push_back(&(children->at(i)->children));
            }
        }

        return nullptr;
    }

    std::unique_ptr<Node> Node::deepCopy() const {
        std::unique_ptr<Node> root = this->shallowCopy();
        
        struct ParseNode {
            const Node* obj;
            Node* copy;
        };

        std::vector<ParseNode> s;

        s.emplace_back(ParseNode{this, root.get()});

        while(!s.empty()) {
            ParseNode node = s.back();
            s.pop_back();
            const Node* obj = node.obj;

            if (!(obj->isVoid())) {

                const std::vector<Node*>& children = obj->children;
                if (!children.empty()) {
                    for (size_t i = 0; i < children.size(); i++) {
                        std::unique_ptr<Node> child = children[i]->shallowCopy();
                        s.emplace_back(ParseNode{children[i], child.get()});
                        node.copy->addChild(std::move(child));
                    }
                }
            }
        }
        return root;
    }
           
    bool Node::shallowEquals(const Node& other) const {
        if (this == &other) return true;
        if (this->isVoid() != other.isVoid()) return false;
        if (this->getTagName() != other.getTagName()) return false;
        if (this->attributes.size() != other.attributes.size()) return false;
        if (this->children.size() != other.children.size()) return false;
        
        std::vector<const Attribute*> attributes1;
        std::vector<const Attribute*> attributes2;

        for (size_t i = 0; i < this->attributes.size(); i++) {
            attributes1.push_back(&(this->attributes[i]));
            attributes2.push_back(&(other.attributes[i]));
        }

        std::sort(attributes1.begin(), attributes1.end(), [](const Attribute* lhs, const Attribute* rhs)
        {
            return lhs->getName() < rhs->getName();
        });

        std::sort(attributes2.begin(), attributes2.end(), [](const Attribute* lhs, const Attribute* rhs)
        {
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

        while(!s.empty()) {
            ParseNode node = s.back();
            s.pop_back();
            const Node* obj = node.obj;
            const Node* other = node.other;

            if (!obj->shallowEquals(*other)) return false;

            if (!(obj->isVoid())) {

                const std::vector<Node*>& children = obj->children;
                const std::vector<Node*>& childrenOther = other->children;
                if (!children.empty()) {
                    for (size_t i = 0; i < children.size(); i++) {
                        // Because of obj->shallowEquals(*other) succeeding, it is known
                        // that at this point the two nodes have the same amount of children
                        s.emplace_back(ParseNode{children[i], childrenOther[i]});
                    }
                }
            }
        }
        return true;
    }

    size_t Node::size() const {
        size_t size = 1;
        for (auto& child: this->children) {
            size += child->size();
        }
        return size;
    }

    size_t Node::depth() const {
        std::vector<const Node*> s;
        size_t maxDepth = 0;
        size_t depth = 0;

        s.emplace_back(this);

        while(!s.empty()) {
            const Node* obj = s.back();
            s.pop_back();

            if (obj == nullptr) {
                depth--;
                continue;
            }

            if (!(obj->isVoid())) {

                const std::vector<Node*>& children = obj->children;
                if (!children.empty()) {
                    depth++;
                    if (depth > maxDepth) {
                        maxDepth = depth;
                    }
                    s.emplace_back(nullptr);
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(children[i-1]);
                    }
                }
            }
        }

        return maxDepth;
    }
    
    
    size_t Node::leafCount() const {
        std::vector<const Node*> s;
        size_t leaves = 0;

        s.emplace_back(this);

        while(!s.empty()) {
            const Node* obj = s.back();
            s.pop_back();

            if (!(obj->isVoid())) {

                const std::vector<Node*>& children = obj->children;
                if (!children.empty()) {
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(children[i-1]);
                    }
                } else {
                    leaves++;
                }
            } else {
                leaves++;
            }
        }

        return leaves;
    }


    bool Node::hasAttribute(const std::string &name) const {
        for (auto& attr: this->attributes) {
            if (attr.getName() == name) {
                return true;
            }
        }

        return false;
    }

    const std::string & Node::getAttributeValue(const std::string &name) const {
        for (auto& attr: this->attributes) {
            if (attr.getName() == name) {
                return attr.getValue();
            }
        }

        throw std::runtime_error("Trying to get Attribute which does not exist: " + name);
    }

    void Node::setAttributeValue(const std::string &name, const std::string &newValue) {
        for (auto& attr: this->attributes) {
            if (attr.getName() == name) {
                attr.setValue(newValue);

                this->indexParse([this](Node::Index* id) -> void {
                    id->update(this);
                });
                return;
            }
        }

        this->attributes.emplace_back(name, newValue);
        this->indexParse([this](Node::Index* id) -> void {
            id->update(this);
        });
    }

    void Node::removeAttribute(const std::string &name) {
        for (auto index = this->attributes.begin(); index != this->attributes.end();) {
            if (index->getName() == name) {
                this->attributes.erase(index);

                this->indexParse([this](Node::Index* id) -> void {
                    id->update(this);
                });
                
                return;
            }
        }
    }

    bool Node::hasSpecialSerialization() const {
        return false;
    }

    std::string Node::serialize() const {
        std::vector<SerializationNode> s;

        std::ostringstream result; 
        result.imbue(std::locale::classic());

        std::vector<const Attribute*> attributes;

        s.emplace_back(SerializationNode{this, false});

        while(!s.empty()) {
            SerializationNode& node = s.back();
            const Node* obj = node.obj;

            const std::string& tagName = obj->getTagName();

            if (node.visited) {
                if (obj->hasSpecialSerialization()) {
                    const Node::SpecialSerializable* objSpecialSerializable = dynamic_cast<const Node::SpecialSerializable*>(obj);
                    objSpecialSerializable->specialSerialize(s, result);
                    continue;
                }
                result << "</" << tagName << ">";
                s.pop_back();
                continue;
            }
            node.visited = true;

            if (obj->hasSpecialSerialization()) {
                const Node::SpecialSerializable* objSpecialSerializable = dynamic_cast<const Node::SpecialSerializable*>(obj);
                objSpecialSerializable->specialSerialize(s, result);
                continue;
            }

            result << "<" << tagName;

            attributes.clear();
            for (size_t i = 0; i < obj->attributes.size(); i++) {
                attributes.push_back(&(obj->attributes[i]));
            }

            for (const auto& attr: attributes) {
                result << " " << attr->getName() << "=\"" << 
                    (attr->shouldEscape() ? text::escape(attr->getValue()) : attr->getValue()) << "\"";
            }

            if (!(obj->isVoid())) {

                const std::vector<Node*>& children = obj->children;
                if (!children.empty()) {
                    result << ">";
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(SerializationNode{children[i-1], false});
                    }
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

    std::string Node::serializePretty(const std::string& indentationSequence, bool sortAttributes) const {
        std::vector<SerializationNode> s;

        std::string indentation;
        std::ostringstream result; 
        result.imbue(std::locale::classic());

        std::vector<const Attribute*> attributes;

        s.emplace_back(SerializationNode{this, false});

        while(!s.empty()) {
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
                    const Node::SpecialSerializable* objSpecialSerializable = dynamic_cast<const Node::SpecialSerializable*>(obj);
                    objSpecialSerializable->specialSerializePretty(s, result, indentation, indentationSequence, sortAttributes);
                    continue;
                }
                result << indentation << "</" << tagName << ">\n";
                s.pop_back();
                continue;
            }
            node.visited = true;

            if (obj->hasSpecialSerialization()) {
                const Node::SpecialSerializable* objSpecialSerializable = dynamic_cast<const Node::SpecialSerializable*>(obj);
                objSpecialSerializable->specialSerializePretty(s, result, indentation, indentationSequence, sortAttributes);
                continue;
            }

            result << indentation << "<" << tagName;

            attributes.clear();
            for (size_t i = 0; i < obj->attributes.size(); i++) {
                attributes.push_back(&(obj->attributes[i]));
            }
            if (sortAttributes) {
                std::sort(attributes.begin(), attributes.end(), [](const Attribute* lhs, const Attribute* rhs)
                {
                    return lhs->getName() < rhs->getName();
                });
            }

            for (const auto& attr: attributes) {
                result << " " << attr->getName() << "=\"" << 
                    (attr->shouldEscape() ? text::escape(attr->getValue()) : attr->getValue()) << "\"";
            }

            if (!(obj->isVoid())) {

                const std::vector<Node*>& children = obj->children;
                if (!children.empty()) {
                    result << ">\n";
                    s.emplace_back(SerializationNode{nullptr, false});
                    indentation += indentationSequence;
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(SerializationNode{children[i-1], false});
                    }
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

    Node::ObservableStringRef::ObservableStringRef(std::string* ref, std::function<void()> callback) 
                            : ptr(ref), callback(std::move(callback)) {}

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

    Node::ObservableStringRef& Node::ObservableStringRef::operator=(std::string newPtr) {
        if (*this->ptr != newPtr) {
            *this->ptr = newPtr;
            this->callback();
        }
        return *this;
    }

    Node::ObservableStringRef Node::operator[](const std::string& name) {
        if (!hasAttribute(name)) {
            setAttributeValue(name, "");
        }

        for (auto& attr: this->attributes) {
            if (attr.getName() == name) {
                return ObservableStringRef(&(attr.getValueMutable()), [this]() { 
                    this->indexParse([this](Node::Index* id) -> void {
                        id->update(this);
                    });
                 });
            }
        }

        throw std::logic_error("Unreachable: attribute not found even after being inserted. Check for memory or multithreading issues.");
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

    void Node::setSortAttributes(bool shouldSort) {
        sortAttributes = shouldSort;
    }

    bool Node::getSortAttributes() {
        return sortAttributes;
    }

    const std::vector<Node*>& Node::getChildrenLive() const {
        return this->children;
    }
}