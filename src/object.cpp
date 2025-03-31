#include "object.h"
#include "text.h"

#include <unordered_map>
#include <sstream>
#include <stack>

namespace Templater::dynamic {

    Attribute::Attribute(std::string name, std::string value, bool shouldEscape) 
        : m_name(std::move(name)), m_value(std::move(value)), m_shouldEscape(shouldEscape) {}

    Attribute::Attribute(std::string name): m_name(std::move(name)), m_value(""), m_shouldEscape(true) {}

    void Attribute::setValue(const std::string& value) {
        m_value = value;
    }

    const std::string& Attribute::getName() const {
        return m_name;
    }

    const std::string& Attribute::getValue() const {
        return m_value;
    }

    std::string& Attribute::getValueMutable() {
        return m_value;
    }

    bool Attribute::shouldEscape() const {
        return m_shouldEscape;
    }

    Object::Object()
        : m_attributes{}, m_children{}, m_indices{}, m_isInTree{false} { }

    Object::Object(Object&& other)
        : m_attributes{std::move(other.m_attributes)}, m_children{std::move(other.m_children)}, m_indices{other.m_indices}, m_isInTree{other.m_isInTree} {}

    Object::Object(std::vector<Attribute> attributes, std::vector<std::unique_ptr<Object>>&& children)
        : m_attributes{std::move(attributes)}, m_children{std::move(children)}, m_indices{} {
        for (auto& child: m_children) {
            child->m_isInTree = true;
        }
    }

    void Object::processConstructorAttribute(Attribute&& attribute) {
        this->operator[](std::move(attribute.getName())) = std::move(attribute.getValue());
    }

    Object::~Object() {
        for (auto& child: m_children) {
            child->m_isInTree = false;

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
    Object* Object::addChild(std::unique_ptr<Object> newChild)  {
        if (isVoid()) {
            throw std::runtime_error("Void " + getTagName() + " cannot have children.");
        }
        if (newChild->isInTree()) {
            throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
            return nullptr;
        }

        newChild->m_isInTree = true;
        this->indexParse([&newChild](index::Index* id) -> void {
            newChild->addIndex(id);
        });

        Object* newChildRef = newChild.get();

        m_children.push_back(std::move(newChild));

        return newChildRef;
    }

    std::vector<Object*> Object::getChildren() const {
        std::vector<Object*> copy;

        copy.reserve(m_children.size());
        for (const auto& child : m_children) {
            copy.push_back(child.get());
        }

        return copy;
    }

    size_t Object::getChildrenCount() const {
        return this->m_children.size();
    }

    void Object::indexParse(std::function<void(index::Index*)> callback) {
        for (auto index: m_indices) {
            callback(index);
            index++;
        }
    }

    // Iteratively adds the index to this node and all its children
    void Object::addIndex(index::Index* index) {
        iterativeProcessor(*this, [&index](Object* obj) -> void {
            obj->m_indices.push_back(index);
            index->putIfNeeded(obj);
        });
    }

    void Object::removeIndex(index::Index* indexToRemove) {
        iterativeProcessor(*this, [&indexToRemove](Object* obj) -> void {
            for (auto index = obj->m_indices.begin(); index != obj->m_indices.end();) {
                if (indexToRemove == *index) {
                    obj->m_indices.erase(index);
                    indexToRemove->removeIfNeeded(obj);
                    break;
                } else {
                    index++;
                }
            }
        });
    }


    void Object::iterativeProcessor(Object& object, std::function<void(Object*)> process) {
        std::vector<Object*> s;

        s.push_back(this);

        while(!s.empty()) {
            Object* obj = s.back();

            process(obj);

            s.pop_back();

            const std::vector<std::unique_ptr<Object>>& children = obj->m_children;
            for (int i = 0; i < children.size(); i++) {
                s.push_back(children[i].get());
            }
        }
    }

    bool Object::isInTree() const {
        return m_isInTree;
    }



    std::vector<Object*> Object::iterativeChildrenParse(const Object& object, std::function<bool(Object*)> condition) const {
        std::vector<Object*> s;
        std::vector<Object*> result;

        auto& objectChildren = object.m_children;
        for (int i = 0; i < objectChildren.size(); i++) {
            s.push_back(objectChildren[i].get());
        }

        while(!s.empty()) {
            Object* obj = s.back();

            if (condition(obj)) {
                result.push_back(obj);
            }

            s.pop_back();

            auto& children = obj->m_children;
            for (int i = children.size()-1; i >= 0; i--) {
                s.push_back(children[i].get());
            }
        }

        return result;
    }

    std::vector<Object*> Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
        return iterativeChildrenParse(*this, ([&attribute, &value](Object* obj) -> bool 
        {  return obj->hasAttributeValue(attribute) && obj->getAttributeValue(attribute) == value; }));
    }

    std::vector<Object*> Object::getChildrenByClassName(const std::string& className) const {

        return getChildrenByAttribute("class", className);
    }

    std::vector<Object*> Object::getChildrenByTagName(const std::string& tagName) const {
        return iterativeChildrenParse(*this, [&tagName](Object* obj) -> bool 
            { return obj->getTagName() == tagName; });
    }

    std::vector<Object*> Object::getChildrenByName(const std::string& name) const {

        return getChildrenByAttribute("name", name);
    }

    std::vector<Object*> Object::getChildrenById(const std::string& id) const {

        return getChildrenByAttribute("id", id);
    }

    const std::vector<Attribute>& Object::getAttributes() const {

        return m_attributes;
    }

    std::unique_ptr<Object> Object::removeChild(Object* childToRemove, Object& currentRoot) {
        auto& children = currentRoot.m_children;

        for (int i = 0; i < children.size(); i++) {
            if (children[i].get() == childToRemove) {

                this->indexParse([&childToRemove, this](index::Index* id) -> void {
                    if (id->getRoot() == this) {
                        childToRemove->removeIndex(id);
                    }
                });

                childToRemove->m_isInTree = false;
                std::unique_ptr<Object> ref = std::move(children[i]);
                children.erase(children.begin() + i);

                return std::move(ref);
            } else {
                if(std::unique_ptr<Object> ptr = removeChild(childToRemove, *(children[i].get()))) {
                    return std::move(ptr);
                }
            }
        }

        return nullptr;
    }

    std::unique_ptr<Object> Object::removeChild(Object* childToRemove) {
        if (!childToRemove->isInTree()) return nullptr;

        return std::move(removeChild(childToRemove, *this));
    }

    size_t Object::size() const {
        size_t size = 1;
        for (auto& child: m_children) {
            size += child->size();
        }
        return size;
    }

    bool Object::hasAttributeValue(const std::string &name) const {
        for (auto& attr: m_attributes) {
            if (attr.getName() == name) {
                return true;
            }
        }

        return false;
    }

    const std::string & Object::getAttributeValue(const std::string &name) const {
        for (auto& attr: m_attributes) {
            if (attr.getName() == name) {
                return attr.getValue();
            }
        }

        throw std::runtime_error("Trying to get Attribute which does not exist: " + name);
    }

    void Object::setAttributeValue(const std::string &name, const std::string &newValue) {
        for (auto& attr: m_attributes) {
            if (attr.getName() == name) {
                attr.setValue(newValue);

                this->indexParse([this](index::Index* id) -> void {
                    id->update(this);
                });
                return;
            }
        }

        m_attributes.emplace_back(name, newValue);
        this->indexParse([this](index::Index* id) -> void {
            id->update(this);
        });
    }

    std::string Object::serialise(const std::string& indentationSequence, bool sortAttributes) const {
        struct Node {
            const Object* obj;
            bool visited;
        };

        std::vector<Node> s;

        std::string indentation;
        std::ostringstream result; 
        result.imbue(std::locale::classic());

        std::vector<const Attribute*> attributes;

        s.emplace_back(this, false);

        while(!s.empty()) {
            Node& node = s.back();
            const Object* obj = node.obj;

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

            if (tagName == "text") {
                result << indentation << obj->serialise(indentationSequence, sortAttributes) << "\n";
                s.pop_back();
                continue;
            }

            if (tagName == "") {
                s.pop_back();
                const std::vector<std::unique_ptr<Object>>& children = obj->m_children;
                for (size_t i = children.size(); i > 0; --i) {
                    s.emplace_back(children[i-1].get(), false);
                }
                continue;
            }

            result << indentation << "<" << tagName;

            attributes.clear();
            for (int i = 0; i < obj->m_attributes.size(); i++) {
                attributes.push_back(&(obj->m_attributes[i]));
            }
            if (sortAttributes) {
                sort(attributes.begin(), attributes.end(), [](const Attribute* lhs, const Attribute* rhs)
                {
                    return lhs->getName() < rhs->getName();
                });
            }

            for (const auto& attr: attributes) {
                result << " " << attr->getName() << "=\"" << 
                    (attr->shouldEscape() ? text::escape(attr->getValue()) : attr->getValue()) << "\"";
            }

            if (!(obj->isVoid())) {

                const std::vector<std::unique_ptr<Object>>& children = obj->m_children;
                if (!children.empty()) {
                    result << ">\n";
                    s.emplace_back(nullptr, false);
                    indentation += indentationSequence;
                    for (size_t i = children.size(); i > 0; --i) {
                        s.emplace_back(children[i-1].get(), false);
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

    Object::ObservableStringRef::ObservableStringRef(std::string* ref, std::function<void()> callback) 
                            : m_ptr(ref), m_callback(std::move(callback)) {}

    Object::ObservableStringRef::operator const std::string*() const { 
        return m_ptr; 
    }

    const std::string* Object::ObservableStringRef::operator->() const { 
        return m_ptr; 
    }

    const std::string& Object::ObservableStringRef::operator*() const { 
        return *m_ptr; 
    }

    bool Object::ObservableStringRef::operator==(const std::string& str) const { 
        return *m_ptr == str; 
    }

    Object::ObservableStringRef& Object::ObservableStringRef::operator=(std::string newPtr) {
        if (*m_ptr != newPtr) {
            *m_ptr = newPtr;
            m_callback();
        }
        return *this;
    }

    Object::ObservableStringRef Object::operator[](const std::string& name) {
        if (!hasAttributeValue(name)) {
            setAttributeValue(name, "");
        }

        for (auto& attr: m_attributes) {
            if (attr.getName() == name) {
                return ObservableStringRef(&(attr.getValueMutable()), [this]() { 
                    this->indexParse([this](index::Index* id) -> void {
                        id->update(this);
                    });
                 });
            }
        }
    }

    Object& Object::operator+=(std::unique_ptr<Object> right) {
        addChild(std::move(right));
        return (*this);
    }

    bool Object::operator==(Object& right) {
        return this == &right;
    }

    std::string Object::indentationSequence = "\t";

    void Object::setIndentationSequence(const std::string& newSequence) {
        indentationSequence = newSequence;
    }

    const std::string& Object::getIndentationSequence() {
        return indentationSequence;
    }

    bool Object::sortAttributes = false;

    void Object::setSortAttributes(bool shouldSort) {
        sortAttributes = shouldSort;
    }

    bool Object::getSortAttributes() {
        return sortAttributes;
    }

    bool VoidObject::isVoid() const {
        return true;
    }

    VoidObject::VoidObject(std::vector<Attribute> attributes): Object(std::move(attributes), {}) {}

    namespace dtags {
        
        GenericObject::GenericObject(std::string tagName, bool isVoid)
                : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object() {}

        GenericObject::GenericObject(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::unique_ptr<Object>>&& children)
                : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object{std::move(attributes), std::move(children)} {
                    if (this->isVoid() && this->getChildrenCount() > 0) {
                        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
                    }
        }

        const std::string& GenericObject::getTagName() const {
            return m_tag;
        }

        bool GenericObject::isVoid() const {
            return m_isVoid;
        }

        GenericObject::GenericObject(Object&& other): m_tag{other.getTagName()}, m_isVoid{other.isVoid()}, Object{std::move(other)} {};

        Text::Text(std::string text, bool escapeMultiByte): m_text(text), m_escapeMultiByte(escapeMultiByte), Object{} {}

        Text::Text(const Text& other): m_text(other.m_text), m_escapeMultiByte(other.m_escapeMultiByte), Object{} {}
        Text::Text(Text&& other): m_text(std::move(other.m_text)), m_escapeMultiByte(other.m_escapeMultiByte), Object{} {}

        const std::string& Text::getText() const {
            return m_text;
        }

        const std::string& Text::getTagName() const {
            static const std::string name = "text";
            return name;
        }

        bool Text::isVoid() const {
            return true;
        }

        std::string Text::serialise(const std::string& indentationSequence, bool sortAttributes) const {
            return text::escape(m_text, m_escapeMultiByte);
        }

        const std::string& EmptyTag::getTagName() const {
            static const std::string name = "";
            return name;
        }

        bool EmptyTag::isVoid() const {
            return false;
        }
    }


    namespace index {

        Index::Index(Object* root): m_root{root}, m_valid{true} {}

        const Object* Index::getRoot() const {
            return m_root;
        }
    
        bool Index::putIfNeeded(Object* object) {
            return false;
        }
    
        bool Index::isValid() const {
            return m_valid;
        }
    
        void Index::invalidate() {
            m_valid = false;
        }
    
        void Index::init() {
            m_root->addIndex(this);
        }
    
        Index::~Index() {
            if (m_valid) {
                m_root->iterativeProcessor(*m_root, [this](Object* obj) -> void {
                    for (auto index = obj->m_indices.begin(); index != obj->m_indices.end();) {
                        if (this == *index) {
                            obj->m_indices.erase(index);
                            break;
                        } else {
                            index++;
                        }
                    }
                });
            }
        }
    }
}