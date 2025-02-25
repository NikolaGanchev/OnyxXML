#include "templater.h"

#include <utility>

using namespace Templater;

html::Attribute::Attribute(std::string name, std::string value): m_name(std::move(name)), m_value(std::move(value)) {}

html::Attribute::Attribute(std::string name): m_name(std::move(name)), m_value("") {}

const std::string& html::Attribute::getName() const {
    return m_name;
}

const std::string& html::Attribute::getValue() const {
    return m_value;
}

html::Object::Object(std::initializer_list<Attribute> attributes, std::initializer_list<Object> children) {
    m_object = std::make_shared<InternalObject>(InternalObject{{}, {}, false});
    
    for (auto& attribute: attributes) {
        m_object->m_attributes[attribute.getName()] = attribute.getValue();
    }

    for (auto& child: children) {
        (m_object->m_children).push_back(child);
    }
}

html::Object::~Object() {
    for (auto& child: m_object->m_children) {
        child.m_object->m_isInTree = false;
    }
}

std::vector<html::Object> html::Object::getChildren() const {
    std::vector<html::Object> result;
    result.reserve(m_object->m_children.size());
    
    for (auto& child: m_object->m_children) {
        result.push_back(child);
    }

    return result;
}

bool html::Object::isInTree() const {
    return m_object->m_isInTree;
}

void html::Object::recursiveChildrenParse(std::vector<html::Object>& children, const html::Object& obj, const std::function<bool(Object&)>& condition) const {
    for (auto& child: obj.m_object->m_children) {
        if (condition(child)) {
            children.push_back(child);
        }
        recursiveChildrenParse(children, child, condition);
    }
}

std::vector<html::Object> html::Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
    std::vector<html::Object> result;

    recursiveChildrenParse(result, *(this), 
    ([&attribute, &value](html::Object& obj) -> bool 
        { return obj.m_object->m_attributes.contains(attribute) && obj.m_object->m_attributes[attribute] == value; }));

    return result;
}

std::vector<html::Object> html::Object::getChildrenByClassName(const std::string& className) const {

    return getChildrenByAttribute("class", className);
}

std::vector<html::Object> html::Object::getChildrenByTagName(const std::string& tagName) const {
    std::vector<html::Object> result;

    recursiveChildrenParse(result, *(this), 
    ([&tagName](html::Object& obj) -> bool 
        { return obj.getTagName() == tagName; }));

    return result;
}

std::vector<html::Object> html::Object::getChildrenByName(const std::string& name) const {

    return getChildrenByAttribute("name", name);
}

std::vector<html::Object> html::Object::getChildrenById(const std::string& id) const {

    return getChildrenByAttribute("id", id);
}

const std::unordered_map<std::string, std::string>& html::Object::getAttributes() const {

    return m_object->m_attributes;
}

const std::string & html::Object::getAttributeValue(std::string &name) const {

    return m_object->m_attributes[name];
}

void html::Object::setAttributeValue(std::string &name, std::string &newValue) {

    m_object->m_attributes[name] = newValue;
}

void html::Object::addChild(Object& newChild) {
    if (isVoid()) {
        throw std::runtime_error("Void objects cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("The object is already in a tree.");
        return;
    }

    newChild.m_object->m_isInTree = true;
    m_object->m_children.push_back(newChild);
}

void html::Object::removeChild(Object & child) {
    if (!child.isInTree()) return;

    std::vector<html::Object> result;

    recursiveChildrenParse(result, *(this), 
    ([&child](html::Object& obj) -> bool 
        { return (obj.m_object).get() == (child.m_object).get(); }));
    
}

std::string html::Object::serialise() {
    std::string result = "<" + getTagName() + " ";
    for (auto& [name, value]: getAttributes()) {
        result += "\"" + name + "\"=\"" + value + "\" ";
    }
    result += ">\n";

    if (!isVoid) {
        for (Object& immediateChildren: m_object->m_children) {
            result += immediateChildren.serialise();
        }

        result += "</" + getTagName() + ">";
    }

    return result;
}

std::string& html::Object::operator[](const std::string& name) {
    return m_object->m_attributes[name];
}

html::Object& html::Object::operator+(Object& right) {
    addChild(right);
    return (*this);
}

html::Object& html::Object::operator+=(Object& right) {
    addChild(right);
    return (*this);
}

html::GenericObject::GenericObject(const std::string tagName, bool isVoid, std::initializer_list<Attribute> attributes, std::initializer_list<Object> children)
    : Object{attributes, children}, m_tag{std::move(tagName)}, m_isVoid{isVoid} {}

const std::string& html::GenericObject::GenericObject::getTagName() const {
    return m_tag;
}

bool html::GenericObject::isVoid() const {
    return m_isVoid;
}