#include "html_object.h"

#include <utility>
#include <map>
#include <unordered_map>
#include <sstream>
#include <queue>
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

    // A Queue to hold numeric HTML entities for multi-byte (non-ASCII) Unicode characters.
    std::queue<uint32_t> codepointSequence;
    std::unordered_map<uint32_t, std::string> dictionary{};

    // Calculate the total size required for the escaped string.
    // This pre-calculation helps in allocating the exact amount of memory needed.
    size_t escapedSize = 0;
    bool safe = true;
    
    for (int i = 0; i < str.size(); i++) {
        // If the current character can cause injections
        if (charsToEscape.contains(str[i])) {
            // Add the length of its escape sequence to the total size.
            escapedSize += (charsToEscape.find(str[i])->second).size();
            safe = false;
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
                if (!dictionary.contains(codepoint)) {
                    // Save the computed HTML entity in a queue to use later when outputting.
                    dictionary[codepoint] = numericEntity(codepoint);
                }
                escapedSize += dictionary[codepoint].size();
                codepointSequence.emplace(codepoint);
                safe = false;
            }
        }
    }

    if (safe) return std::string(str);

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
                const std::string& entity = dictionary[codepointSequence.front()];
                codepointSequence.pop();
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