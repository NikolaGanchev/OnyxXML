#include "html_object.h"

#include <utility>
#include <map>

using namespace Templater;

dynamic::Attribute::Attribute(std::string name, std::string value): m_name(std::move(name)), m_value(std::move(value)) {}

dynamic::Attribute::Attribute(std::string name): m_name(std::move(name)), m_value("") {}

const std::string& dynamic::Attribute::getName() const {
    return m_name;
}

const std::string& dynamic::Attribute::getValue() const {
    return m_value;
}

dynamic::Object::Object() { 
    m_object = std::make_shared<InternalObject>(InternalObject{{}, {}, false});
}

dynamic::Object::Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children) { 
    std::unordered_map<std::string, std::string> attributesMap;
    for (auto& attr: attributes) {
        attributesMap[attr.getName()] = attr.getValue();
    }

    for (auto& child: children) {
        child->m_object->m_isInTree = true;
    }

    m_object = std::make_shared<InternalObject>(InternalObject{attributesMap, children});
}

void dynamic::Object::processConstructorAttribute(Attribute attribute) {
    m_object->m_attributes[attribute.getName()] = attribute.getValue();
}

void dynamic::Object::processConstructorObject(Object& child) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Templater::html::Object with a child that is already a child of another Templater::html::Object.");
    }
    child.m_object->m_isInTree = true;
    (m_object->m_children).push_back(child.clone());
}

dynamic::InternalObject::~InternalObject() {
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
    addChild(newChild);
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
        { return obj->m_object->m_attributes.contains(attribute) && obj->m_object->m_attributes[attribute] == value; }));

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

const std::unordered_map<std::string, std::string>& dynamic::Object::getAttributes() const {

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

const std::string & dynamic::Object::getAttributeValue(const std::string &name) const {

    return m_object->m_attributes[name];
}

void dynamic::Object::setAttributeValue(const std::string &name, const std::string &newValue) {

    m_object->m_attributes[name] = newValue;
}

std::string dynamic::Object::serialiseRecursive(std::string& identation, const std::string& identationSequence, bool sortAttributes) const {
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
            
            for (const std::shared_ptr<dynamic::Object>& immediateChild: m_object->m_children) {
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

std::string dynamic::Object::serialise(const std::string& identationSequence, bool sortAttributes) const {
    std::string identation;
    std::string result = serialiseRecursive(identation, identationSequence, sortAttributes);
    return result;
}

std::string& dynamic::Object::operator[](const std::string& name) {
    return m_object->m_attributes[name];
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
        : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object(std::move(attributes), std::move(children)) {}

const std::string& dynamic::dtags::GenericObject::getTagName() const {
    return m_tag;
}

bool dynamic::dtags::GenericObject::isVoid() const {
    return m_isVoid;
}

std::shared_ptr<dynamic::Object> dynamic::dtags::GenericObject::clone() const {
    return std::make_shared<GenericObject>(*this);
}

dynamic::dtags::Text::Text(std::string text): m_text(text), Object{} {}

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

std::string dynamic::dtags::Text::serialiseRecursive(std::string& identation, const std::string& identationSequence = getIdentationSequence(), bool sortAttributes = getSortAttributes()) const {
    return identation + dynamic::text::escape(m_text);
}

dynamic::dtags::EmptyTag::EmptyTag(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children)
        : Object(std::move(attributes), std::move(children)) {}

const std::string& dynamic::dtags::EmptyTag::getTagName() const {
    static const std::string name = "";
    return name;
}

bool dynamic::dtags::EmptyTag::isVoid() const {
    return false;
}

std::shared_ptr<dynamic::Object> dynamic::dtags::EmptyTag::clone() const {
    return std::make_shared<EmptyTag>(*this);
}

std::string dynamic::dtags::EmptyTag::serialiseRecursive(std::string& identation, const std::string& identationSequence, bool sortAttributes) const {
    std::string res;

    for (const std::shared_ptr<dynamic::Object>& immediateChild: getChildren()) {
        res += identation + immediateChild->serialiseRecursive(identation, identationSequence, sortAttributes);
        res += "\n";
    }

    // Remove last newline
    res.pop_back();

    return res;
}

std::string Templater::dynamic::text::escape(const std::string& str) {
    static constexpr std::array<const char*, 128> charsToEscape = [] {
        std::array<const char*, 128> arr{};
        arr['&'] = "&amp;";
        arr['<'] = "&lt;";
        arr['>'] = "&gt;";
        arr['\"'] = "&quot;";
        arr['\''] = "&apos;";
        return arr;
    }();

    size_t escapedSize = 0;
    
    for (int i = 0; i < str.size(); i++) {
        if (str[i] < 128 && charsToEscape[str[i]] != 0) {
            escapedSize += strlen(charsToEscape[str[i]]);
        } else {
            escapedSize++;
        }
    }

    std::string escaped(escapedSize, '\0');

    const char* read = &str[0];
    char* write = escaped.data();
    const char* escapeSequence = 0;

    while (*read != '\0') {
        if (*read < 128 && charsToEscape[*read] != 0) {
            escapeSequence = &(charsToEscape[*read][0]);
            while (*escapeSequence != '\0') {
                *write = *escapeSequence;
                escapeSequence++;
                write++;
            }
            read++;
        } else {
           *write = *read;
           read++;
           write++;
        }
    }

    return escaped;
}