#include "html_object.h"

#include <utility>
#include <map>

using namespace Templater;

html::Attribute::Attribute(std::string name, std::string value): m_name(std::move(name)), m_value(std::move(value)) {}

html::Attribute::Attribute(std::string name): m_name(std::move(name)), m_value("") {}

const std::string& html::Attribute::getName() const {
    return m_name;
}

const std::string& html::Attribute::getValue() const {
    return m_value;
}

html::Object::Object() { 
    m_object = std::make_shared<InternalObject>(InternalObject{{}, {}, false});
}

html::Object::Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children) { 
    std::unordered_map<std::string, std::string> attributesMap;
    for (auto& attr: attributes) {
        attributesMap[attr.getName()] = attr.getValue();
    }

    for (auto& child: children) {
        child->m_object->m_isInTree = true;
    }

    m_object = std::make_shared<InternalObject>(InternalObject{attributesMap, children});
}

void html::Object::processConstructorAttribute(Attribute attribute) {
    m_object->m_attributes[attribute.getName()] = attribute.getValue();
}

void html::Object::processConstructorObject(Object& child) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Templater::html::Object with a child that is already a child of another Templater::html::Object.");
    }
    child.m_object->m_isInTree = true;
    (m_object->m_children).push_back(child.clone());
}

html::InternalObject::~InternalObject() {
    for (auto& child: m_children) {
        child->m_object->m_isInTree = false;
    }
}

html::Object::~Object() {}

void html::Object::addChild(Object& newChild)  {
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

void html::Object::addChild(Object&& newChild)  {
    addChild(newChild);
}

const std::vector<std::shared_ptr<html::Object>> html::Object::getChildren() const {
    std::vector<std::shared_ptr<html::Object>> copy;

    copy.reserve(m_object->m_children.size());
    for (const auto& child : m_object->m_children) {
        copy.push_back(child);
    }

    return copy;
}

bool html::Object::isInTree() const {
    return m_object->m_isInTree;
}

void html::Object::recursiveChildrenParse(std::vector<std::shared_ptr<html::Object>>& children, const std::shared_ptr<Object> obj, const std::function<bool(std::shared_ptr<Object>)>& condition) const {
    for (auto& child: obj->m_object->m_children) {
        if (condition(child)) {
            children.push_back(child);
        }
        recursiveChildrenParse(children, child, condition);
    }
}

std::vector<std::shared_ptr<html::Object>> html::Object::getChildrenByAttribute(const std::string& attribute, const std::string& value) const {
    std::vector<std::shared_ptr<html::Object>> result;

    recursiveChildrenParse(result, this->clone(), 
    ([&attribute, &value](std::shared_ptr<html::Object> obj) -> bool 
        { return obj->m_object->m_attributes.contains(attribute) && obj->m_object->m_attributes[attribute] == value; }));

    return result;
}

std::vector<std::shared_ptr<html::Object>> html::Object::getChildrenByClassName(const std::string& className) const {

    return getChildrenByAttribute("class", className);
}

std::vector<std::shared_ptr<html::Object>> html::Object::getChildrenByTagName(const std::string& tagName) const {
    std::vector<std::shared_ptr<html::Object>> result;

    recursiveChildrenParse(result, this->clone(), 
    ([&tagName](std::shared_ptr<html::Object> obj) -> bool 
        { return obj->getTagName() == tagName; }));

    return result;
}

std::vector<std::shared_ptr<html::Object>> html::Object::getChildrenByName(const std::string& name) const {

    return getChildrenByAttribute("name", name);
}

std::vector<std::shared_ptr<html::Object>> html::Object::getChildrenById(const std::string& id) const {

    return getChildrenByAttribute("id", id);
}

const std::unordered_map<std::string, std::string>& html::Object::getAttributes() const {

    return m_object->m_attributes;
}

bool html::Object::removeChild(Object& childToRemove, Object& currentRoot) {
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

bool html::Object::removeChild(Object& childToRemove) {
    if (!childToRemove.isInTree()) return false;

    return removeChild(childToRemove, *this);
}

bool html::Object::removeChild(const std::shared_ptr<Object>& childToRemove) {
    return removeChild(*(childToRemove));
}

const std::string & html::Object::getAttributeValue(const std::string &name) const {

    return m_object->m_attributes[name];
}

void html::Object::setAttributeValue(const std::string &name, const std::string &newValue) {

    m_object->m_attributes[name] = newValue;
}

std::string html::Object::serialiseRecursive(std::string& identation, const std::string& identationSequence, bool sortAttributes) const {
    std::string result = identation + "<" + getTagName() + "";
    
    if (sortAttributes) {
        auto attributes = getAttributes();
        std::map<std::string, std::string> sortedAttributes(attributes.begin(), attributes.end());
        for (auto& [name, value]: sortedAttributes) {
            result += " " + name + "=\"" + value + "\"";
        }
    } else {
        for (auto& [name, value]: getAttributes()) {
            result += " " + name + "=\"" + value + "\"";
        }
    }

    if (!isVoid()) {

        result += ">";

        if (!getChildren().empty()) {

            result += "\n";
            
            for (const std::shared_ptr<html::Object>& immediateChild: m_object->m_children) {
                identation += identationSequence;

                result += immediateChild->serialiseRecursive(identation, identationSequence, sortAttributes);

                // Backtrack identation
                for (int i = 0; i < identationSequence.size(); i++) {
                    identation.pop_back();
                }

                result += "\n";
            }
        }

        if (result[result.size() - 1] == '\n') {
            result += identation;
        }

        result += "</" + getTagName() + ">";
    } else {
        result += "/>";
    }
 
    return result;
}

std::string html::Object::serialise(const std::string& identationSequence, bool sortAttributes) const {
    std::string identation;
    std::string result = serialiseRecursive(identation, identationSequence, sortAttributes);
    return result;
}

std::string& html::Object::operator[](const std::string& name) {
    return m_object->m_attributes[name];
}

html::Object& html::Object::operator+=(html::Object& right) {
    addChild(right);
    return (*this);
}

html::Object& html::Object::operator+=(html::Object&& right) {
    addChild(right);
    return (*this);
}

bool html::Object::operator==(html::Object& right) {
    return m_object.get() == right.m_object.get();
}

std::string html::Object::identationSequence = "\t";

void html::Object::setIdentationSequence(const std::string& newSequence) {
    identationSequence = newSequence;
}

const std::string& html::Object::getIdentationSequence() {
    return identationSequence;
}

bool html::Object::sortAttributes = false;

void html::Object::setSortAttributes(bool shouldSort) {
    sortAttributes = shouldSort;
}

bool html::Object::getSortAttributes() {
    return sortAttributes;
}
        
html::GenericObject::GenericObject(std::string  tagName, bool isVoid)
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object() {}

html::GenericObject::GenericObject(std::string  tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object(std::move(attributes), std::move(children)) {}

const std::string& html::GenericObject::getTagName() const {
    return m_tag;
}

bool html::GenericObject::isVoid() const {
    return m_isVoid;
}

std::shared_ptr<html::Object> html::GenericObject::clone() const {
    return std::make_shared<GenericObject>(*this);
}

html::Text::Text(std::string text): m_text(text), Object{} {}

const std::string& html::Text::getText() const {
    return m_text;
}

const std::string& html::Text::getTagName() const {
    static const std::string name = "text";
    return name;
}

bool html::Text::isVoid() const {
    return true;
}

std::shared_ptr<html::Object> html::Text::clone() const {
    return std::make_shared<Text>(*this);
}

std::string html::Text::serialiseRecursive(std::string& identation, const std::string& identationSequence = getIdentationSequence(), bool sortAttributes = getSortAttributes()) const {
    return identation + m_text;
}

html::EmptyTag::EmptyTag(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
        : Object(std::move(attributes), std::move(children)) {}

const std::string& html::EmptyTag::getTagName() const {
    static const std::string name = "";
    return name;
}

bool html::EmptyTag::isVoid() const {
    return false;
}

std::shared_ptr<html::Object> html::EmptyTag::clone() const {
    return std::make_shared<EmptyTag>(*this);
}

std::string html::EmptyTag::serialiseRecursive(std::string& identation, const std::string& identationSequence, bool sortAttributes) const {
    std::string res;

    for (const std::shared_ptr<html::Object>& immediateChild: getChildren()) {
        res += identation + immediateChild->serialiseRecursive(identation, identationSequence, sortAttributes);
        res += "\n";
    }

    // Remove last newline
    res.pop_back();

    return res;
}