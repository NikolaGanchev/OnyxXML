#include "parse/sax_parser.h"

#include "parse/_parse_macro.h"
#include "parse/string_cursor.h"

namespace onyx::dynamic::parser {
SaxParser::SaxParser(SaxListener& listener) : listener(listener) {}

void SaxParser::parse(std::string_view input) {
    std::vector<std::string_view> stack;
    std::vector<std::string_view> attributeNames;
    std::vector<std::pair<std::string_view, bool>> attributeValues;

    StringCursor pos(input.data());

    std::string_view root = ".empty";

    stack.push_back(root);

    skipWhitespace(pos);

#define TEXT_ACTION(text, hasEntities, pos)                        \
    this->listener.onText(hasEntities ? text::expandEntities(text) \
                                      : std::string(text));

#define COMMENT_ACTION(commentText, pos) \
    this->listener.onComment(std::string(commentText));

#define CDATA_ACTION(cdataText, pos) \
    this->listener.onCData(std::string(cdataText));

#define INSTRUCTION_ACTION(tagName, processingInstruction, pos) \
    this->listener.onInstruction(std::string(tagName),          \
                                 std::string(processingInstruction));

#define ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos) \
    attributeNames.push_back(attributeName);                              \
    attributeValues.push_back(std::make_pair(attributeValue, hasEntities));

#define XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, \
                               hasStandalone, pos)                           \
    this->listener.onXMLDeclaration(std::string(version),                    \
                                    std::string(encoding), hasEncoding,      \
                                    isStandalone, hasStandalone);

#define DOCTYPE_ACTION(doctypeText, pos) \
    this->listener.onDoctype(std::string(doctypeText));

#define OPEN_ACTION(tagName, pos, isSelfClosing)                  \
    std::vector<Attribute> attributes;                            \
    for (int i = 0; i < attributeNames.size(); i++) {             \
        attributes.emplace_back(                                  \
            std::string(attributeNames[i]),                       \
            attributeValues[i].second                             \
                ? text::expandEntities(attributeValues[i].first)  \
                : std::string(attributeValues[i].first));         \
    }                                                             \
                                                                  \
    if (!isSelfClosing) {                                         \
        stack.push_back(tagName);                                 \
    }                                                             \
    attributeNames.clear();                                       \
    attributeValues.clear();                                      \
    this->listener.onTagOpen(std::string(tagName), isSelfClosing, \
                             std::move(attributes));

#define CLOSE_ACTION(tagName, pos)                                \
    std::string_view thisNode = stack.back();                     \
    if (thisNode != tagName) {                                    \
        throw std::invalid_argument("Closing unopened tag");      \
    }                                                             \
    if (stack.size() == 1) {                                      \
        throw std::invalid_argument("Closing non-existent tags"); \
    }                                                             \
    stack.pop_back();

    this->listener.onStart();
    try {
        PARSE_BODY(true);
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    this->listener.onEnd();

#undef TEXT_ACTION
#undef COMMENT_ACTION
#undef CDATA_ACTION
#undef INSTRUCTION_ACTION
#undef ATTRIBUTE_ACTION
#undef OPEN_ACTION
#undef XML_DECLARATION_ACTION
#undef DOCTYPE_ACTION
#undef CLOSE_ACTION

    if (stack.size() != 1 || stack[0] != root) {
        throw std::invalid_argument("Unclosed tags left");
    }
}
}  // namespace onyx::dynamic::parser