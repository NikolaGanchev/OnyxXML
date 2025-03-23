#include "html_object.h"
#include "text.h"

#include <unordered_map>
#include <sstream>
#include <stack>

using namespace Templater;

dynamic::Attribute::Attribute(std::string name, std::string value): m_name(std::move(name)), m_value(std::move(value)) {}

dynamic::Attribute::Attribute(std::string name): m_name(std::move(name)), m_value("") {}

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

dynamic::Object::Object() { 
    m_object = std::make_shared<Data>(Data{{}, {}, false});
}

dynamic::Object::Object(const Object& other): m_object{other.m_object} {}

dynamic::Object::Object(Object&& other): m_object{std::move(other.m_object)} {}

dynamic::Object::Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children) {
    for (auto& child: children) {
        child->m_object->m_isInTree = true;
    }

    m_object = std::make_shared<Data>(Data{std::move(attributes), std::move(children)});
}

void dynamic::Object::processConstructorAttribute(const Attribute& attribute) {
    m_object->m_attributes.push_back(attribute);
}

void dynamic::Object::processConstructorObject(Object& child) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Templater::html::Object with a child that is already a child of another Templater::html::Object.");
    }
    child.m_object->m_isInTree = true;
    (m_object->m_children).push_back(child.clone());
}

void dynamic::Object::processConstructorAttribute(Attribute&& attribute) {
    this->operator[](std::move(attribute.getName())) = std::move(attribute.getValue());
}

void dynamic::Object::processConstructorObject(Object&& child) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Templater::html::Object with a child that is already a child of another Templater::html::Object.");
    }
    child.m_object->m_isInTree = true;
    (m_object->m_children).push_back(std::move(child.clone()));
}

dynamic::Object::Data::~Data() {
    for (auto& child: m_children) {
        child->m_object->m_isInTree = false;
    }
}

dynamic::Object::~Object() {}

void dynamic::Object::addChild(Object& newChild)  {
    if (isVoid()) {
        throw std::runtime_error("Void Templater::html::" + getTagName() + " cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("Attempted to add child to Templater::html::" + getTagName() + "  that is already a child of another Templater::html::Object.");
        return;
    }

    newChild.m_object->m_isInTree = true;
    m_object->m_children.push_back(newChild.clone());
}

void dynamic::Object::addChild(Object&& newChild)  {
    if (isVoid()) {
        throw std::runtime_error("Void Templater::html::" + getTagName() + " cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("Attempted to add child to Templater::html::" + getTagName() + "  that is already a child of another Templater::html::Object.");
        return;
    }

    newChild.m_object->m_isInTree = true;
    m_object->m_children.push_back(std::move(newChild.clone()));
}

const std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildren() const {
    std::vector<std::shared_ptr<dynamic::Object>> copy;

    copy.reserve(m_object->m_children.size());
    for (const auto& child : m_object->m_children) {
        copy.push_back(child);
    }

    return copy;
}

bool dynamic::Object::isInTree() const {
    return m_object->m_isInTree;
}

void dynamic::Object::recursiveChildrenParse(std::vector<std::shared_ptr<dynamic::Object>>& children, const std::shared_ptr<Object> obj, const std::function<bool(std::shared_ptr<Object>)>& condition) const {
    for (auto& child: obj->m_object->m_children) {
        if (condition(child)) {
            children.push_back(child);
        }
        recursiveChildrenParse(children, child, condition);
    }
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
    std::vector<std::shared_ptr<dynamic::Object>> result;

    recursiveChildrenParse(result, this->clone(), 
    ([&attribute, &value](std::shared_ptr<dynamic::Object> obj) -> bool 
        { return obj->hasAttributeValue(attribute) && obj->getAttributeValue(attribute) == value; }));

    return result;
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByClassName(const std::string& className) const {

    return getChildrenByAttribute("class", className);
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByTagName(const std::string& tagName) const {
    std::vector<std::shared_ptr<dynamic::Object>> result;

    recursiveChildrenParse(result, this->clone(), 
    ([&tagName](std::shared_ptr<dynamic::Object> obj) -> bool 
        { return obj->getTagName() == tagName; }));

    return result;
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenByName(const std::string& name) const {

    return getChildrenByAttribute("name", name);
}

std::vector<std::shared_ptr<dynamic::Object>> dynamic::Object::getChildrenById(const std::string& id) const {

    return getChildrenByAttribute("id", id);
}

const std::vector<dynamic::Attribute>& dynamic::Object::getAttributes() const {

    return m_object->m_attributes;
}

bool dynamic::Object::removeChild(Object& childToRemove, Object& currentRoot) {
    auto& children = currentRoot.m_object->m_children;

    for (int i = 0; i < children.size(); i++) {
        if (*(children[i].get()) == childToRemove) {
            childToRemove.m_object->m_isInTree = false;
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

size_t dynamic::Object::size() const {
    size_t size = 1;
    for (auto& child: m_object->m_children) {
        size += child->size();
    }
    return size;
}

bool dynamic::Object::hasAttributeValue(const std::string &name) const {
    for (auto& attr: m_object->m_attributes) {
        if (attr.getName() == name) {
            return true;
        }
    }

    return false;
}

const std::string & dynamic::Object::getAttributeValue(const std::string &name) const {
    for (auto& attr: m_object->m_attributes) {
        if (attr.getName() == name) {
            return attr.getValue();
        }
    }

    throw std::runtime_error("Trying to get Attribute which does not exist: " + name);
}

void dynamic::Object::setAttributeValue(const std::string &name, const std::string &newValue) {
    for (auto& attr: m_object->m_attributes) {
        if (attr.getName() == name) {
            attr.setValue(newValue);
            return;
        }
    }
    
    m_object->m_attributes.emplace_back(name, newValue);
}

std::string dynamic::Object::serialise(const std::string& identationSequence, bool sortAttributes) const {
    struct Node {
        const Templater::dynamic::Object* obj;
        bool visited;
    };

    std::vector<Node> s;
    
    std::string identation;
    std::ostringstream result; 
    result.imbue(std::locale::classic());
    
    std::vector<Attribute*> attributes;

    s.emplace_back(this, false);

    while(!s.empty()) {
        Node& node = s.back();
        const Object* obj = node.obj;

        if (obj == nullptr) {
            identation.resize(identation.size() - identationSequence.size());
            s.pop_back();
            continue;
        }

        const std::string& tagName = obj->getTagName();

        if (node.visited) {
            result << identation << "</" << tagName << ">\n";
            s.pop_back();
            continue;
        }
        node.visited = true;

        if (tagName == "text") {
            result << identation << obj->serialise(identationSequence, sortAttributes) << "\n";
            s.pop_back();
            continue;
        }
        
        if (tagName == "empty") {
            s.pop_back();
            const std::vector<std::shared_ptr<Object>>& children = obj->m_object->m_children;
            for (size_t i = children.size(); i > 0; --i) {
                s.emplace_back(children[i-1].get(), false);
            }
            continue;
        }

        result << identation << "<" << tagName << "";

        attributes.clear();
        for (int i = 0; i < obj->m_object->m_attributes.size(); i++) {
            attributes.push_back(&obj->m_object->m_attributes[i]);
        }
        if (sortAttributes) {
            sort(attributes.begin(), attributes.end(), [](const Attribute* lhs, const Attribute* rhs)
            {
                return lhs->getName() < rhs->getName();
            });
        }

        for (const auto& attr: attributes) {
            result << " " << attr->getName() << "=\"" << Templater::dynamic::text::escape(attr->getValue()) << "\"";
        }

        if (!(obj->isVoid())) {
            
            const std::vector<std::shared_ptr<Object>>& children = obj->m_object->m_children;
            if (!children.empty()) {
                result << ">\n";
                s.emplace_back(nullptr, false);
                identation += identationSequence;
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

std::string& dynamic::Object::operator[](const std::string& name) {
    if (!hasAttributeValue(name)) {
        setAttributeValue(name, "");
    }
    
    for (auto& attr: m_object->m_attributes) {
        if (attr.getName() == name) {
            return attr.getValueMutable();
        }
    }
}

dynamic::Object& dynamic::Object::operator+=(dynamic::Object& right) {
    addChild(right);
    return (*this);
}

dynamic::Object& dynamic::Object::operator+=(dynamic::Object&& right) {
    addChild(right);
    return (*this);
}

bool dynamic::Object::operator==(dynamic::Object& right) {
    return m_object.get() == right.m_object.get();
}

std::string dynamic::Object::identationSequence = "\t";

void dynamic::Object::setIdentationSequence(const std::string& newSequence) {
    identationSequence = newSequence;
}

const std::string& dynamic::Object::getIdentationSequence() {
    return identationSequence;
}

bool dynamic::Object::sortAttributes = false;

void dynamic::Object::setSortAttributes(bool shouldSort) {
    sortAttributes = shouldSort;
}

bool dynamic::Object::getSortAttributes() {
    return sortAttributes;
}
        
dynamic::dtags::GenericObject::GenericObject(std::string  tagName, bool isVoid)
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object() {}

dynamic::dtags::GenericObject::GenericObject(std::string  tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object{attributes, std::move(children)} {}

const std::string& dynamic::dtags::GenericObject::getTagName() const {
    return m_tag;
}

bool dynamic::dtags::GenericObject::isVoid() const {
    return m_isVoid;
}

std::shared_ptr<dynamic::Object> dynamic::dtags::GenericObject::clone() const {
    return std::make_shared<GenericObject>(*this);
}

dynamic::dtags::GenericObject::GenericObject(const Object& other): m_tag{other.getTagName()}, m_isVoid{other.isVoid()}, Object{other} {}

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

std::shared_ptr<dynamic::Object> dynamic::dtags::Text::clone() const {
    return std::make_shared<Text>(*this);
}

std::string dynamic::dtags::Text::serialise(const std::string& identationSequence, bool sortAttributes) const {
    return dynamic::text::escape(m_text, m_escapeMultiByte);
}

const std::string& dynamic::dtags::EmptyTag::getTagName() const {
    static const std::string name = "empty";
    return name;
}

bool dynamic::dtags::EmptyTag::isVoid() const {
    return false;
}

std::shared_ptr<dynamic::Object> dynamic::dtags::EmptyTag::clone() const {
    return std::make_shared<EmptyTag>(*this);
}
