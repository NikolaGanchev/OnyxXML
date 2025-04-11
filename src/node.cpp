#include "node.h"
#include "text.h"
#include "index.h"

#include <unordered_map>
#include <sstream>
#include <stack>
#include <algorithm>

namespace Templater::dynamic {
    Node::Node()
        : attributes{}, children{}, indices{}, _isInTree{false} { }

    Node::Node(Node&& other)
        : attributes{std::move(other.attributes)}, children{std::move(other.children)}, indices{other.indices}, _isInTree{other._isInTree} {}

    Node::Node(std::vector<Attribute> attributes, std::vector<std::unique_ptr<Node>>&& children)
        : attributes{std::move(attributes)}, children{std::move(children)}, indices{} {
        for (auto& child: this->children) {
            child->_isInTree = true;
        }
    }

    void Node::processConstructorAttribute(Attribute&& attribute) {
        this->operator[](std::move(attribute.getName())) = std::move(attribute.getValue());
    }

    Node::~Node() {
        for (auto& child: this->children) {
            child->_isInTree = false;

            this->indexParse([this, &child](index::Index* id) -> void {
                if (id->getRoot() == this) {
                    child->removeIndex(id);
                }
            });
        }

        this->indexParse([this](index::Index* id) -> void {
            if (id->getRoot() == this) {
                id->invalidate();
            }
        });
    }
    Node* Node::addChild(std::unique_ptr<Node> newChild)  {
        if (isVoid()) {
            throw std::runtime_error("Void " + getTagName() + " cannot have children.");
        }
        if (newChild->isInTree()) {
            throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
            return nullptr;
        }

        newChild->_isInTree = true;
        this->indexParse([&newChild](index::Index* id) -> void {
            newChild->addIndex(id);
        });

        Node* newChildRef = newChild.get();

         this->children.push_back(std::move(newChild));

        return newChildRef;
    }

    std::vector<Node*> Node::getChildren() const {
        std::vector<Node*> copy;

        copy.reserve( this->children.size());
        for (const auto& child : this->children) {
            copy.push_back(child.get());
        }

        return copy;
    }

    size_t Node::getChildrenCount() const {
        return this->children.size();
    }

    void Node::indexParse(std::function<void(index::Index*)> callback) {
        for (auto index: this->indices) {
            callback(index);
            index++;
        }
    }

    // Iteratively adds the index to this node and all its children
    void Node::addIndex(index::Index* index) {
        iterativeProcessor(*this, [&index](Node* obj) -> void {
            obj->indices.push_back(index);
            index->putIfNeeded(obj);
        });
    }

    void Node::removeIndex(index::Index* indexToRemove) {
        iterativeProcessor(*this, [&indexToRemove](Node* obj) -> void {
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


    void Node::iterativeProcessor(Node& object, std::function<void(Node*)> process) {
        std::vector<Node*> s;

        s.push_back(this);

        while(!s.empty()) {
            Node* obj = s.back();

            process(obj);

            s.pop_back();

            const std::vector<std::unique_ptr<Node>>& children = obj->children;
            for (int i = 0; i < children.size(); i++) {
                s.push_back(children[i].get());
            }
        }
    }

    bool Node::isInTree() const {
        return this->_isInTree;
    }

    std::vector<Node*> Node::iterativeChildrenParse(const Node& object, std::function<bool(Node*)> condition) const {
        std::vector<Node*> s;
        std::vector<Node*> result;

        auto& objectChildren = object.children;
        for (int i = 0; i < objectChildren.size(); i++) {
            s.push_back(objectChildren[i].get());
        }

        while(!s.empty()) {
            Node* obj = s.back();

            if (condition(obj)) {
                result.push_back(obj);
            }

            s.pop_back();

            auto& children = obj->children;
            for (int i = children.size()-1; i >= 0; i--) {
                s.push_back(children[i].get());
            }
        }

        return result;
    }

    std::vector<Node*> Node::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
        return iterativeChildrenParse(*this, ([&attribute, &value](Node* obj) -> bool 
        {  return obj->hasAttribute(attribute) && obj->getAttributeValue(attribute) == value; }));
    }

    std::vector<Node*> Node::getChildrenByClassName(const std::string& className) const {

        return getChildrenByAttribute("class", className);
    }

    std::vector<Node*> Node::getChildrenByTagName(const std::string& tagName) const {
        return iterativeChildrenParse(*this, [&tagName](Node* obj) -> bool 
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

    std::unique_ptr<Node> Node::removeChild(Node* childToRemove) {
        if (!childToRemove->isInTree()) return nullptr;

        std::vector<std::vector<std::unique_ptr<Node>>*> s;

        s.push_back(&(this->children));

        while(!s.empty()) {
            std::vector<std::unique_ptr<Node>>* children = s.back();

            for (int i = 0; i < children->size(); i++) {
                if (children->at(i).get() == childToRemove) {
    
                    this->indexParse([&childToRemove, this](index::Index* id) -> void {
                        if (id->getRoot() == this) {
                            childToRemove->removeIndex(id);
                        }
                    });
    
                    childToRemove->_isInTree = false;
                    std::unique_ptr<Node> ref = std::move(children->operator[](i));
                    children->erase(children->begin() + i);
    
                    return std::move(ref);
                }
            }

            s.pop_back();

            for (int i = 0; i < children->size(); i++) {
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

                const std::vector<std::unique_ptr<Node>>& children = obj->children;
                if (!children.empty()) {
                    for (size_t i = 0; i < children.size(); i++) {
                        std::unique_ptr<Node> child = children[i]->shallowCopy();
                        s.emplace_back(ParseNode{children[i].get(), child.get()});
                        node.copy->addChild(std::move(child));
                    }
                }
            }
        }
        return std::move(root);
    }
           
    bool Node::shallowEquals(const Node& other) const {
        if (this == &other) return true;
        if (this->_isInTree != other._isInTree) return false;
        if (this->isVoid() != other.isVoid()) return false;
        if (this->getTagName() != other.getTagName()) return false;
        if (this->attributes.size() != other.attributes.size()) return false;
        if (this->children.size() != other.children.size()) return false;
        
        std::vector<const Attribute*> attributes1;
        std::vector<const Attribute*> attributes2;

        for (int i = 0; i < this->attributes.size(); i++) {
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

        for (int i = 0; i < attributes1.size(); i++) {
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

                const std::vector<std::unique_ptr<Node>>& children = obj->children;
                const std::vector<std::unique_ptr<Node>>& childrenOther = other->children;
                if (!children.empty()) {
                    for (size_t i = 0; i < children.size(); i++) {
                        // Because of obj->shallowEquals(*other) succeeding, it is known
                        // that at this point the two nodes have the same amount of children
                        s.emplace_back(ParseNode{children[i].get(), childrenOther[i].get()});
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
        struct ParseNode {
            const Node* obj;
            bool visited;
        };

        std::vector<ParseNode> s;
        size_t maxDepth = 0;
        size_t depth = 0;

        s.emplace_back(ParseNode{this, false});

        while(!s.empty()) {
            ParseNode node = s.back();
            s.pop_back();
            const Node* obj = node.obj;

            if (obj == nullptr) {
                depth--;
                continue;
            }

            if (!(obj->isVoid())) {

                const std::vector<std::unique_ptr<Node>>& children = obj->children;
                if (!children.empty()) {
                    depth++;
                    if (depth > maxDepth) {
                        maxDepth = depth;
                    }
                    s.emplace_back(ParseNode{nullptr, false});
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(ParseNode{children[i-1].get(), false});
                    }
                }
            }
        }

        return maxDepth;
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

                this->indexParse([this](index::Index* id) -> void {
                    id->update(this);
                });
                return;
            }
        }

        this->attributes.emplace_back(name, newValue);
        this->indexParse([this](index::Index* id) -> void {
            id->update(this);
        });
    }

    void Node::removeAttribute(const std::string &name) {
        for (auto index = this->attributes.begin(); index != this->attributes.end();) {
            if (index->getName() == name) {
                this->attributes.erase(index);

                this->indexParse([this](index::Index* id) -> void {
                    id->update(this);
                });
                
                return;
            }
        }
    }

    bool Node::hasSpecialSerialisation() const {
        return false;
    }

    std::string Node::serialise(const std::string& indentationSequence, bool sortAttributes) const {
        struct ParseNode {
            const Node* obj;
            bool visited;
        };

        std::vector<ParseNode> s;

        std::string indentation;
        std::ostringstream result; 
        result.imbue(std::locale::classic());

        std::vector<const Attribute*> attributes;

        s.emplace_back(ParseNode{this, false});

        while(!s.empty()) {
            ParseNode& node = s.back();
            const Node* obj = node.obj;

            if (obj == nullptr) {
                indentation.resize(indentation.size() - indentationSequence.size());
                s.pop_back();
                continue;
            }

            const std::string& tagName = obj->getTagName();

            if (node.visited) {
                result << indentation << "</" << tagName << ">\n";
                s.pop_back();
                continue;
            }
            node.visited = true;

            if (obj->hasSpecialSerialisation()) {
                result << indentation << obj->serialise(indentationSequence, sortAttributes) << "\n";
                s.pop_back();
                continue;
            }

            if (tagName == ".empty") {
                s.pop_back();
                const std::vector<std::unique_ptr<Node>>& children = obj->children;
                for (size_t i = children.size(); i > 0; --i) {
                    s.emplace_back(ParseNode{children[i-1].get(), false});
                }
                continue;
            }

            result << indentation << "<" << tagName;

            attributes.clear();
            for (int i = 0; i < obj->attributes.size(); i++) {
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

                const std::vector<std::unique_ptr<Node>>& children = obj->children;
                if (!children.empty()) {
                    result << ">\n";
                    s.emplace_back(ParseNode{nullptr, false});
                    indentation += indentationSequence;
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(ParseNode{children[i-1].get(), false});
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
                    this->indexParse([this](index::Index* id) -> void {
                        id->update(this);
                    });
                 });
            }
        }
    }

    Node& Node::operator+=(std::unique_ptr<Node> right) {
        addChild(std::move(right));
        return (*this);
    }

    bool Node::operator==(Node& right) {
        return this == &right;
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
}