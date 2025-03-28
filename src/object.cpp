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

dynamic::Object::Object(std::vector<Attribute> attributes, std::vector<std::unique_ptr<Object>>&& children)
    : m_attributes{std::move(attributes)}, m_children{std::move(children)}, m_indices{} {
    for (auto& child: m_children) {
        child->m_isInTree = true;
    }
}

void dynamic::Object::processConstructorAttribute(Attribute&& attribute) {
    this->operator[](std::move(attribute.getName())) = std::move(attribute.getValue());
}

dynamic::Object::~Object() {
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
dynamic::Object* dynamic::Object::addChild(std::unique_ptr<Object> newChild)  {
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

std::vector<dynamic::Object*> dynamic::Object::getChildren() const {
    std::vector<dynamic::Object*> copy;

    copy.reserve(m_children.size());
    for (const auto& child : m_children) {
        copy.push_back(child.get());
    }

    return copy;
}

size_t dynamic::Object::getChildrenCount() const {
    return this->m_children.size();
}

void dynamic::Object::indexParse(std::function<void(index::Index*)> callback) {
    for (auto index: m_indices) {
        callback(index);
        index++;
    }
}

// Iteratively adds the index to this node and all its children
void dynamic::Object::addIndex(index::Index* index) {
    iterativeProcessor(*this, [&index](Object* obj) -> void {
        obj->m_indices.push_back(index);
        index->putIfNeeded(obj);
    });
}

void dynamic::Object::removeIndex(index::Index* indexToRemove) {
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


void dynamic::Object::iterativeProcessor(Object& object, std::function<void(Object*)> process) {
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

bool dynamic::Object::isInTree() const {
    return m_isInTree;
}



std::vector<dynamic::Object*> dynamic::Object::iterativeChildrenParse(const Object& object, std::function<bool(Object*)> condition) const {
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

std::vector<dynamic::Object*> dynamic::Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
    return iterativeChildrenParse(*this, ([&attribute, &value](Object* obj) -> bool 
    {  return obj->hasAttributeValue(attribute) && obj->getAttributeValue(attribute) == value; }));
}

std::vector<dynamic::Object*> dynamic::Object::getChildrenByClassName(const std::string& className) const {

    return getChildrenByAttribute("class", className);
}

std::vector<dynamic::Object*> dynamic::Object::getChildrenByTagName(const std::string& tagName) const {
    return iterativeChildrenParse(*this, [&tagName](Object* obj) -> bool 
        { return obj->getTagName() == tagName; });
}

std::vector<dynamic::Object*> dynamic::Object::getChildrenByName(const std::string& name) const {

    return getChildrenByAttribute("name", name);
}

std::vector<dynamic::Object*> dynamic::Object::getChildrenById(const std::string& id) const {

    return getChildrenByAttribute("id", id);
}

const std::vector<dynamic::Attribute>& dynamic::Object::getAttributes() const {

    return m_attributes;
}

std::unique_ptr<dynamic::Object> dynamic::Object::removeChild(Object* childToRemove, Object& currentRoot) {
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

std::unique_ptr<dynamic::Object> dynamic::Object::removeChild(Object* childToRemove) {
    if (!childToRemove->isInTree()) return nullptr;

    return std::move(removeChild(childToRemove, *this));
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
                (attr->shouldEscape() ? Templater::dynamic::text::escape(attr->getValue()) : attr->getValue()) << "\"";
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
                this->indexParse([this](index::Index* id) -> void {
                    id->update(this);
                });
             });
        }
    }
}

dynamic::Object& dynamic::Object::operator+=(std::unique_ptr<Object> right) {
    addChild(std::move(right));
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

dynamic::dtags::GenericObject::GenericObject(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::unique_ptr<Object>>&& children)
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

Templater::dynamic::index::Index::Index(Object* root): m_root{root}, m_valid{true} {}

const Templater::dynamic::Object* Templater::dynamic::index::Index::getRoot() const {
    return m_root;
}

bool Templater::dynamic::index::Index::putIfNeeded(Object* object) {
    return false;
}

bool Templater::dynamic::index::Index::isValid() const {
    return m_valid;
}

void Templater::dynamic::index::Index::invalidate() {
    m_valid = false;
}

void Templater::dynamic::index::Index::init() {
    m_root->addIndex(this);
}

Templater::dynamic::index::Index::~Index() {
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