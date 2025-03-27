#include "object.h"
#include "text.h"

#include <unordered_map>
#include <sstream>
#include <stack>

using namespace Templater;

dynamic::Attribute::Attribute(std::string name, std::string value, bool shouldEscape) 
    : m_name(std::move(name)), m_value(std::move(value)), m_shouldEscape(shouldEscape) {}

dynamic::Attribute::Attribute(std::string name): m_name(std::move(name)), m_value(""), m_shouldEscape(true) {}

void dynamic::Attribute::setValue(const std::string& value) {
    m_value = value;
}

const std::string& dynamic::Attribute::getName() const {
    return m_name;
}

const std::string& dynamic::Attribute::getValue() const {
    return m_value;
}

std::string& dynamic::Attribute::getValueMutable() {
    return m_value;
}

bool dynamic::Attribute::shouldEscape() const {
    return m_shouldEscape;
}

dynamic::Object::Object()
    : m_attributes{}, m_children{}, m_indices{}, m_isInTree{false} { }

dynamic::Object::Object(Object&& other)
    : m_attributes{std::move(other.m_attributes)}, m_children{std::move(other.m_children)}, m_indices{other.m_indices}, m_isInTree{other.m_isInTree} {}

dynamic::Object::Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
    : m_attributes{std::move(attributes)}, m_children{std::move(children)}, m_indices{} {
    for (auto& child: m_children) {
        child->m_isInTree = true;
    }
}

void dynamic::Object::processConstructorAttribute(const Attribute& attribute) {
    m_attributes.push_back(attribute);
}

void dynamic::Object::processConstructorObject(std::shared_ptr<Object>& child) {
    if (child->isInTree()) {
        throw std::runtime_error("Attempted to construct Object with a child that is already a child of another Object.");
    }
    child->m_isInTree = true;
    m_children.push_back(child);
}

void dynamic::Object::processConstructorAttribute(Attribute&& attribute) {
    this->operator[](std::move(attribute.getName())) = std::move(attribute.getValue());
}

dynamic::Object::~Object() {
    for (auto& child: m_children) {
        child->m_isInTree = false;

        this->indexParse([this, &child](std::shared_ptr<index::Index> id) -> void {
            if (std::shared_ptr<Object> root = id->getRoot().lock()) {
                if (root.get() == this) {
                    child->removeIndex(id);
                }
            }
        });
    }
}
void dynamic::Object::addChild(std::shared_ptr<Object> newChild)  {
    if (isVoid()) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
    if (newChild->isInTree()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
        return;
    }

    newChild->m_isInTree = true;
    this->indexParse([&newChild](std::shared_ptr<index::Index> id) -> void {
        newChild->addIndex(id);
    });

    m_children.push_back(newChild);
}

const std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildren() const {
    std::vector<std::shared_ptr<dynamic::Object>> copy;

    copy.reserve(m_children.size());
    for (const auto& child : m_children) {
        copy.push_back(child);
    }

    return copy;
}

size_t dynamic::Object::getChildrenCount() const {
    return this->m_children.size();
}

void dynamic::Object::indexParse(std::function<void(std::shared_ptr<index::Index>)> callback) {
    for (auto index = m_indices.begin(); index != m_indices.end();) {
        if (std::shared_ptr<index::Index> id = index->lock()) {
            callback(id);
            index++;
        } else {
            m_indices.erase(index);
        }
    }
}

// Iteratively adds the index to this node and all its children
void dynamic::Object::addIndex(std::shared_ptr<index::Index> index) {
    iterativeChildrenProcessor(*this, [&index](std::shared_ptr<Object> obj) -> void {
        obj->m_indices.push_back(index);
        index->putIfNeeded(obj);
    });
}

void dynamic::Object::removeIndex(std::shared_ptr<index::Index> indexToRemove) {
    iterativeChildrenProcessor(*this, [&indexToRemove](std::shared_ptr<Object> obj) -> void {
        for (auto index = obj->m_indices.begin(); index != obj->m_indices.end();) {
            if (std::shared_ptr<index::Index> id = index->lock()) {
                if (indexToRemove.get() == id.get()) {
                    obj->m_indices.erase(index);
                    indexToRemove->removeIfNeeded(obj);
                    break;
                } else {
                    index++;
                }
            } else {
                obj->m_indices.erase(index);
            }
        }
    });
}


void dynamic::Object::iterativeChildrenProcessor(Object& object, std::function<void(std::shared_ptr<Object>)> process) {
    std::vector<std::shared_ptr<Object>> s;

    while(!s.empty()) {
        std::shared_ptr<Object> obj = s.back();

        process(obj);

        s.pop_back();

        const std::vector<std::shared_ptr<Object>>& children = obj->m_children;
        for (int i = 0; i < children.size(); i++) {
            s.push_back(children[i]);
        }
    }
}

bool dynamic::Object::isInTree() const {
    return m_isInTree;
}



std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::iterativeChildrenParse(const Object& object, std::function<bool(std::shared_ptr<Object>)> condition) const {
    std::vector<std::shared_ptr<Object>> s;
    std::vector<std::shared_ptr<Object>> result;

    const auto& objectChildren = object.m_children;
    for (int i = 0; i < objectChildren.size(); i++) {
        s.push_back(objectChildren[i]);
    }

    while(!s.empty()) {
        const std::shared_ptr<Object> obj = s.back();

        if (condition(obj)) {
            result.push_back(obj);
        }

        s.pop_back();

        const auto& children = obj->m_children;
        for (int i = children.size()-1; i >= 0; i--) {
            s.push_back(children[i]);
        }
    }

    return result;
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
    return iterativeChildrenParse(*this, ([&attribute, &value](std::shared_ptr<Object> obj) -> bool 
    {  return obj->hasAttributeValue(attribute) && obj->getAttributeValue(attribute) == value; }));
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByClassName(const std::string& className) const {

    return getChildrenByAttribute("class", className);
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByTagName(const std::string& tagName) const {
    return iterativeChildrenParse(*this, ([&tagName](std::shared_ptr<dynamic::Object> obj) -> bool 
        { return obj->getTagName() == tagName; }));
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByName(const std::string& name) const {

    return getChildrenByAttribute("name", name);
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenById(const std::string& id) const {

    return getChildrenByAttribute("id", id);
}

const std::vector<dynamic::Attribute>& dynamic::Object::getAttributes() const {

    return m_attributes;
}

bool dynamic::Object::removeChild(Object& childToRemove, Object& currentRoot) {
    auto& children = currentRoot.m_children;

    for (int i = 0; i < children.size(); i++) {
        if (*(children[i].get()) == childToRemove) {

            this->indexParse([&childToRemove, this](std::shared_ptr<index::Index> id) -> void {
                if (std::shared_ptr<Object> root = id->getRoot().lock()) {
                    if (root.get() == this) {
                        childToRemove.removeIndex(id);
                    }
                }
            });

            childToRemove.m_isInTree = false;
            children.erase(children.begin() + i);

            return true;
        } else {
            if(removeChild(childToRemove, *(children[i].get()))) {
                return true;
            }
        }
    }

    return false;
}

bool dynamic::Object::removeChild(Object& childToRemove) {
    if (!childToRemove.isInTree()) return false;

    return removeChild(childToRemove, *this);
}

bool dynamic::Object::removeChild(const std::shared_ptr<Object>& childToRemove) {
    return removeChild(*(childToRemove));
}

std::shared_ptr<dynamic::Object> dynamic::Object::pointer() {
    return shared_from_this();
}

size_t dynamic::Object::size() const {
    size_t size = 1;
    for (auto& child: m_children) {
        size += child->size();
    }
    return size;
}

bool dynamic::Object::hasAttributeValue(const std::string &name) const {
    for (auto& attr: m_attributes) {
        if (attr.getName() == name) {
            return true;
        }
    }

    return false;
}

const std::string & dynamic::Object::getAttributeValue(const std::string &name) const {
    for (auto& attr: m_attributes) {
        if (attr.getName() == name) {
            return attr.getValue();
        }
    }

    throw std::runtime_error("Trying to get Attribute which does not exist: " + name);
}

void dynamic::Object::setAttributeValue(const std::string &name, const std::string &newValue) {
    for (auto& attr: m_attributes) {
        if (attr.getName() == name) {
            attr.setValue(newValue);
            
            this->indexParse([this](std::shared_ptr<index::Index> id) -> void {
                id->update(this->pointer());
            });
            return;
        }
    }
    
    m_attributes.emplace_back(name, newValue);
    this->indexParse([this](std::shared_ptr<index::Index> id) -> void {
        id->update(this->pointer());
    });
}

std::string dynamic::Object::serialise(const std::string& indentationSequence, bool sortAttributes) const {
    struct Node {
        const Templater::dynamic::Object* obj;
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
            const std::vector<std::shared_ptr<Object>>& children = obj->m_children;
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
                (attr->shouldEscape() ? Templater::dynamic::text::escape(attr->getValue()) : attr->getValue()) << "\"";
        }

        if (!(obj->isVoid())) {
            
            const std::vector<std::shared_ptr<Object>>& children = obj->m_children;
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

Templater::dynamic::Object::ObservableStringRef::ObservableStringRef(std::string* ref, std::function<void()> callback) 
                        : m_ptr(ref), m_callback(std::move(callback)) {}
                
Templater::dynamic::Object::ObservableStringRef::operator const std::string*() const { 
    return m_ptr; 
}
                
const std::string* Templater::dynamic::Object::ObservableStringRef::operator->() const { 
    return m_ptr; 
}

const std::string& Templater::dynamic::Object::ObservableStringRef::operator*() const { 
    return *m_ptr; 
}

bool Templater::dynamic::Object::ObservableStringRef::operator==(const std::string& str) const { 
    return *m_ptr == str; 
}
                
Templater::dynamic::Object::ObservableStringRef& Templater::dynamic::Object::ObservableStringRef::operator=(std::string newPtr) {
    if (*m_ptr != newPtr) {
        *m_ptr = newPtr;
        m_callback();
    }
    return *this;
}

Templater::dynamic::Object::ObservableStringRef dynamic::Object::operator[](const std::string& name) {
    if (!hasAttributeValue(name)) {
        setAttributeValue(name, "");
    }
    
    for (auto& attr: m_attributes) {
        if (attr.getName() == name) {
            return ObservableStringRef(&(attr.getValueMutable()), [this]() { 
                this->indexParse([this](std::shared_ptr<index::Index> id) -> void {
                    id->update(this->pointer());
                });
             });
        }
    }
}

dynamic::Object& dynamic::Object::operator+=(std::shared_ptr<Object>& right) {
    addChild(right);
    return (*this);
}

bool dynamic::Object::operator==(dynamic::Object& right) {
    return this == &right;
}

std::string dynamic::Object::indentationSequence = "\t";

void dynamic::Object::setIndentationSequence(const std::string& newSequence) {
    indentationSequence = newSequence;
}

const std::string& dynamic::Object::getIndentationSequence() {
    return indentationSequence;
}

bool dynamic::Object::sortAttributes = false;

void dynamic::Object::setSortAttributes(bool shouldSort) {
    sortAttributes = shouldSort;
}

bool dynamic::Object::getSortAttributes() {
    return sortAttributes;
}
        
dynamic::dtags::GenericObject::GenericObject(std::string tagName, bool isVoid)
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object() {}

dynamic::dtags::GenericObject::GenericObject(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object{std::move(attributes), std::move(children)} {
            if (this->isVoid() && this->getChildrenCount() > 0) {
                throw std::runtime_error("Void " + getTagName() + " cannot have children.");
            }
}

const std::string& dynamic::dtags::GenericObject::getTagName() const {
    return m_tag;
}

bool dynamic::dtags::GenericObject::isVoid() const {
    return m_isVoid;
}

dynamic::dtags::GenericObject::GenericObject(Object&& other): m_tag{other.getTagName()}, m_isVoid{other.isVoid()}, Object{std::move(other)} {};

dynamic::dtags::Text::Text(std::string text, bool escapeMultiByte): m_text(text), m_escapeMultiByte(escapeMultiByte), Object{} {}

dynamic::dtags::Text::Text(const Text& other): m_text(other.m_text), m_escapeMultiByte(other.m_escapeMultiByte), Object{} {}
dynamic::dtags::Text::Text(Text&& other): m_text(std::move(other.m_text)), m_escapeMultiByte(other.m_escapeMultiByte), Object{} {}

const std::string& dynamic::dtags::Text::getText() const {
    return m_text;
}

const std::string& dynamic::dtags::Text::getTagName() const {
    static const std::string name = "text";
    return name;
}

bool dynamic::dtags::Text::isVoid() const {
    return true;
}

std::string dynamic::dtags::Text::serialise(const std::string& indentationSequence, bool sortAttributes) const {
    return dynamic::text::escape(m_text, m_escapeMultiByte);
}

const std::string& dynamic::dtags::EmptyTag::getTagName() const {
    static const std::string name = "";
    return name;
}

bool dynamic::dtags::EmptyTag::isVoid() const {
    return false;
}

bool dynamic::VoidObject::isVoid() const {
    return true;
}

Templater::dynamic::VoidObject::VoidObject(std::vector<Attribute> attributes): Object(std::move(attributes), {}) {}

Templater::dynamic::index::Index::Index(std::shared_ptr<Object> root): m_root{root} {
    putIfNeeded(root);
    root->addIndex(pointer());
}

const std::weak_ptr<Templater::dynamic::Object> Templater::dynamic::index::Index::getRoot() const {
    return m_root;
}

std::shared_ptr<Templater::dynamic::index::Index> Templater::dynamic::index::Index::pointer() {
    return shared_from_this();
}

bool Templater::dynamic::index::Index::putIfNeeded(std::shared_ptr<Object> object) {
    return false;
}