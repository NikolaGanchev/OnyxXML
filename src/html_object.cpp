#include "html_object.h"

#include <utility>
#include <map>
#include <unordered_map>
#include <sstream>
#include <queue>

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

dynamic::Object::Object(const Object& other): m_object{other.m_object} {}

dynamic::Object::Object(Object&& other): m_object{std::move(other.m_object)} {}

dynamic::Object::Object(const std::vector<Attribute>& attributes, std::vector<std::shared_ptr<Object>> children) { 
    std::unordered_map<std::string, std::string> attributesMap;
    for (auto& attr: attributes) {
        attributesMap[attr.getName()] = attr.getValue();
    }

    for (auto& child: children) {
        child->m_object->m_isInTree = true;
    }

    m_object = std::make_shared<InternalObject>(InternalObject{attributesMap, std::move(children)});
}

void dynamic::Object::processConstructorAttribute(const Attribute& attribute) {
    m_object->m_attributes[attribute.getName()] = attribute.getValue();
}

void dynamic::Object::processConstructorObject(Object& child) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Templater::html::Object with a child that is already a child of another Templater::html::Object.");
    }
    child.m_object->m_isInTree = true;
    (m_object->m_children).push_back(child.clone());
}

void dynamic::Object::processConstructorAttribute(Attribute&& attribute) {
    m_object->m_attributes[std::move(attribute.getName())] = std::move(attribute.getValue());
}

void dynamic::Object::processConstructorObject(Object&& child) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Templater::html::Object with a child that is already a child of another Templater::html::Object.");
    }
    child.m_object->m_isInTree = true;
    (m_object->m_children).push_back(std::move(child.clone()));
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

size_t dynamic::Object::size() {
    size_t size = 1;
    for (auto& child: m_object->m_children) {
        size += child->size();
    }
    return size;
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

dynamic::dtags::GenericObject::GenericObject(std::string  tagName, bool isVoid, const std::vector<Attribute>& attributes, std::vector<std::shared_ptr<Object>> children)
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

std::string dynamic::dtags::Text::serialiseRecursive(std::string& identation, const std::string& identationSequence = getIdentationSequence(), bool sortAttributes = getSortAttributes()) const {
    return identation + dynamic::text::escape(m_text, m_escapeMultiByte);
}

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

// Escapes a given string so that it is safe for use in HTML contexts.
// This function replaces reserved HTML characters with their corresponding entities
// and converts non-ASCII characters to numeric HTML entities if enabled
// Note: non-ASCII character escaping is disabled by default
std::string Templater::dynamic::text::escape(const std::string& str, bool escapeMultiByte) {

    // Map of characters that must be escaped in HTML to their corresponding entity strings to keep HTML integrity
    static const std::unordered_map<char, const std::string> charsToEscape = {
        {'&', "&amp;"},
        {'<', "&lt;"},
        {'>', "&gt;"},
        {'\"', "&quot;"},
        {'\'', "&apos;"}
    };

    // Queue to hold numeric HTML entities for multi-byte (non-ASCII) Unicode characters.
    std::queue<std::string> unicodeHtmlEntities;

    // Calculate the total size required for the escaped string.
    // This pre-calculation helps in allocating the exact amount of memory needed.
    size_t escapedSize = 0;
    
    for (int i = 0; i < str.size(); i++) {
        // If the current character can cause injections
        if (charsToEscape.contains(str[i])) {
            // Add the length of its escape sequence to the total size.
            escapedSize += (charsToEscape.find(str[i])->second).size();
        } else {
            // If multi-byte escaping is disabled, just increment
            if (!escapeMultiByte) {
                escapedSize++;
            }
            // Get the full Unicode codepoint starting from the current character pointer.
            uint32_t codepoint = getUnicodeCodepoint(&str[i]);
            if (codepoint < 128) {
                // ASCII characters require one character in the output.
                escapedSize += 1;
            } else {
                // For non-ASCII characters, convert the codepoint to a numeric HTML entity.
                std::string htmlEntity = numericEntity(codepoint);
                escapedSize += htmlEntity.size();
                // Save the computed HTML entity in a queue to use later when outputting.
                unicodeHtmlEntities.push(htmlEntity);
            }
        }
    }

    // Create a new string with the computed size.
    // Initialize with null characters as placeholders.
    std::string escaped(escapedSize, '\0');

    // Setup pointers for reading the input string and writing to the escaped string.
    const char* read = &str[0];
    char* write = escaped.data();
    const char* escapeSequence = 0;

    // Process the input string until we reach its null terminator.
    while (*read != '\0') {
        // If the current character can cause injections
        if (charsToEscape.contains(*read)) {
            // Get the escape sequence for the character.
            escapeSequence = &((charsToEscape.find(*read)->second)[0]);
            // Write the escape sequence into the output.
            while (*escapeSequence != '\0') {
                *write = *escapeSequence;
                escapeSequence++;
                write++;
            }
            // Move the read pointer past the escaped character.
            read++;
        } else {
            // If the character is ASCII (single-byte) or multi-byte escaping is disabled, copy it directly.
            if ((unsigned char)*read < 128 || !escapeMultiByte) {
                *write = *read;
                read++;
                write++;
            } else {
                // For multi-byte characters (non-ASCII), we need to advance the read pointer
                // based on the size of the UTF-8 sequence.
                if (((unsigned char)*read >> 5) == 0x6) { // 2-byte sequence
        
                    if (*(read + 1) != '\0') {
                        read += 2;
                    }
                } else if (((unsigned char)*read >> 4) == 0xE) { // 3-byte sequence
                
                    if (*(read + 2) != '\0') {
                        read += 3;
                    }
                } else if (((unsigned char)*read >> 3) == 0x1E) { // 4-byte sequence
                
                    if (*(read + 3) != '\0') {
                        read += 4;
                    }
                }

                // Retrieve the pre-computed HTML entity for this Unicode character.
                std::string entity = unicodeHtmlEntities.front();
                unicodeHtmlEntities.pop();
                 // Write the entity into the output.
                for (int i = 0; i < entity.size(); i++) {
                    *write = entity[i];
                    *write++;
                }
            }
        }
    }

    // Adjust the final string size based on how many characters were written.
    // This trims any extra null characters.
    escaped.resize(write - &escaped[0]);

    return escaped;
}

// Returns the Unicode codepoint for a UTF-8 encoded character starting at 'read'.
// Handles 1-, 2-, 3-, and 4-byte sequences.
uint32_t Templater::dynamic::text::getUnicodeCodepoint(const char* read) {
    uint32_t codepoint = 0;

    // 1-byte ASCII character (0xxxxxxx)
    if ((unsigned char)*read < 128) { // 1-byte ASCII

        codepoint = *read;
    }   
    // 2-byte sequence (110xxxxx 10xxxxxx) 
    else if (((unsigned char)*read >> 5) == 0x6) { // 2-byte sequence

        if (*(read + 1) != '\0') {
            codepoint = (((unsigned char)*read & 0x1F) << 6) | ((unsigned char)*(read + 1) & 0x3F);
        }
    } 
    // 3-byte sequence (1110xxxx 10xxxxxx 10xxxxxx)
    else if (((unsigned char)*read >> 4) == 0xE) { // 3-byte sequence

        if (*(read + 2) != '\0') {
            codepoint = (((unsigned char)*read & 0x0F) << 12) |
                        (((unsigned char)*(read + 1) & 0x3F) << 6) |
                        ((unsigned char)*(read + 2) & 0x3F);
        }
    }
    // 4-byte sequence (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
    else if (((unsigned char)*read >> 3) == 0x1E) { // 4-byte sequence

        if (*(read + 3) != '\0') {
            codepoint = (((unsigned char)*read & 0x07) << 18) |
                        (((unsigned char)*(read + 1) & 0x3F) << 12) |
                        (((unsigned char)*(read + 2) & 0x3F) << 6) |
                        ((unsigned char)*(read + 3) & 0x3F);
        }
    }

    return codepoint;
}

// Converts a Unicode codepoint into its corresponding numeric HTML entity string.
// For example, codepoint 160 becomes "&#xa0;".
std::string Templater::dynamic::text::numericEntity(uint32_t codepoint) {
    std::ostringstream oss;
    oss << "&#x" << std::hex << codepoint << ";";
    return oss.str();
}