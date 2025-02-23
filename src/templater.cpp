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

bool html::Object::isInTree() const {
    return m_object->m_isInTree;
}

html::Object::Object(std::string tag, bool isVoid, std::initializer_list<Attribute> attributes, std::initializer_list<Object> children) {
    m_object = std::make_shared<InternalObject>(InternalObject{tag, {}, {}, isVoid, false});
    
    for (auto& attribute: attributes) {
        m_object->m_attributes[attribute.getName()] = attribute.getValue();
    }

    for (auto& child: children) {
        m_object->m_children.push_back(child.m_object);
    }
}

html::Object::Object(std::shared_ptr<InternalObject> ptr) {
    m_object = ptr;
}

const std::string & html::Object::getTagName() const {
    return m_object->m_tag;
}

std::vector<html::Object> html::Object::getChildren() const {
    std::vector<html::Object> result;
    result.reserve(m_object->m_children.size());
    
    for (auto& child: m_object->m_children) {
        result.push_back(Object(std::move(child)));
    }

    return result;
}

void html::Object::recursiveChildrenParse(std::vector<html::Object>& children, const html::InternalObject& obj, const std::function<bool(InternalObject&)>& condition) const {
    for (auto& child: obj.m_children) {
        if (condition(*child)) {
            children.push_back(Object(std::move(child)));
        }
        recursiveChildrenParse(children, *child, condition);
    }
}

std::vector<html::Object> html::Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
    std::vector<html::Object> result;

    recursiveChildrenParse(result, *(this->m_object), 
    ([&attribute, &value](html::InternalObject& obj) -> bool 
        { return obj.m_attributes.contains(attribute) && obj.m_attributes[attribute] == value; }));

    return result;
}

std::vector<html::Object> html::Object::getChildrenByClassName(const std::string& className) const {

    return getChildrenByAttribute("class", className);
}

std::vector<html::Object> html::Object::getChildrenByTagName(const std::string& tagName) const {
    std::vector<html::Object> result;

    recursiveChildrenParse(result, *(this->m_object), 
    ([&tagName](html::InternalObject& obj) -> bool 
        { return obj.m_tag == tagName; }));

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

void html::Object::addChild(Object & newChild) {
    // Throw exception instead
    if (newChild.isInTree()) {
        return;
    }

    newChild.m_object->m_isInTree = true;
    m_object->m_children.push_back(newChild.m_object);
}

void html::Object::removeChild(Object & child) {
    if (!child.isInTree()) return;

    std::vector<html::Object> result;

    recursiveChildrenParse(result, *(this->m_object), 
    ([&child](html::InternalObject& obj) -> bool 
        { return &obj == (child.m_object).get() }));
    
}

std::string html::Object::serialise() {
    //TODO
    return "";
}
