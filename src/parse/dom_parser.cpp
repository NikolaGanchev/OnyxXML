#include "parse/dom_parser.h"

#include "parse/_parse_macro.h"
#include "parse/string_cursor.h"
#include "parse/stream_cursor.h"

namespace onyx::dynamic::parser {
ParseResult::ParseResult() : arena{0}, root{nullptr} {}

ParseResult::ParseResult(Arena arena, Node* root)
    : arena{std::move(arena)}, root{root} {}

ParseResult::ParseResult(ParseResult&& other) : arena{std::move(other.arena)} {
    this->root = other.root;
    other.root = nullptr;
}

ParseResult& ParseResult::operator=(ParseResult&& other) {
    this->arena = std::move(other.arena);
    this->root = other.root;
    other.root = nullptr;

    return *this;
}

Arena DomParser::parseDryRun(std::string_view input) {
    std::vector<size_t> stack;

    size_t id = 0;

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());

    Arena::Builder builder;

    std::hash<std::string_view> hasher;

    // Root
    builder.preallocate<tags::EmptyNode>();

    std::string_view root = ".empty";

    stack.push_back(hasher(root));

    skipWhitespace(pos);

#define TEXT_ACTION(text, hasEntities, pos) builder.preallocate<tags::Text>();

#define COMMENT_ACTION(commentText, pos) builder.preallocate<tags::Comment>();

#define CDATA_ACTION(cdataText, pos) builder.preallocate<tags::CData>();

#define XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, \
                               hasStandalone, pos)                           \
    builder.preallocate<tags::XmlDeclaration>();

#define INSTRUCTION_ACTION(tagName, processingInstruction, pos) \
    builder.preallocate<tags::ProcessingInstruction>();

#define ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos)

#define DOCTYPE_ACTION(doctypeText, pos) builder.preallocate<tags::Doctype>();

#define OPEN_ACTION(tagName, pos, isSelfClosing) \
    builder.preallocate<tags::GenericNode>();    \
    if (!isSelfClosing) {                        \
        stack.push_back(hasher(tagName));        \
    }

#define CLOSE_ACTION(tagName, pos)                                \
    size_t thisNode = stack.back();                               \
    if (thisNode != hasher(tagName)) {                            \
        throw std::invalid_argument("Closing unopened tag");      \
    }                                                             \
    if (stack.size() == 1) {                                      \
        throw std::invalid_argument("Closing non-existent tags"); \
    }                                                             \
    stack.pop_back();

    PARSE_BODY(true);

#undef TEXT_ACTION
#undef COMMENT_ACTION
#undef CDATA_ACTION
#undef INSTRUCTION_ACTION
#undef ATTRIBUTE_ACTION
#undef OPEN_ACTION
#undef XML_DECLARATION_ACTION
#undef DOCTYPE_ACTION
#undef CLOSE_ACTION

    // Invariant - stack may only contain the root
    if (stack.size() != 1 || stack[0] != hasher(root)) {
        throw std::invalid_argument("Unclosed tags left");
    }

    return builder.build();
}

ParseResult DomParser::parse(std::string_view input) {
    std::vector<Node*> stack;
    std::vector<std::string_view> attributeNames;
    std::vector<std::pair<std::string_view, bool>> attributeValues;

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());

    Arena arena = parseDryRun(input);

    Node* root = arena.allocate<tags::EmptyNode>();

    stack.push_back(root);

    skipWhitespace(pos);

#define TEXT_ACTION(text, hasEntities, pos)            \
    stack.back()->addChild(arena.allocate<tags::Text>( \
        hasEntities ? text::expandEntities(text) : std::string(text)));

#define COMMENT_ACTION(commentText, pos) \
    stack.back()->addChild(              \
        arena.allocate<tags::Comment>(std::string(commentText)));

#define CDATA_ACTION(cdataText, pos) \
    stack.back()->addChild(arena.allocate<tags::CData>(std::string(cdataText)));

#define INSTRUCTION_ACTION(tagName, processingInstruction, pos)         \
    stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>( \
        std::string(tagName), std::string(processingInstruction)));

#define ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos) \
    attributeNames.push_back(attributeName);                              \
    attributeValues.push_back(std::make_pair(attributeValue, hasEntities));

#define XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, \
                               hasStandalone, pos)                           \
    stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(             \
        std::string(version), std::string(encoding), hasEncoding,            \
        isStandalone, hasStandalone, false));

#define DOCTYPE_ACTION(doctypeText, pos) \
    stack.back()->addChild(              \
        arena.allocate<tags::Doctype>(std::string(doctypeText)));

#define OPEN_ACTION(tagName, pos, isSelfClosing)                            \
    Node* newNode = arena.allocate<tags::GenericNode>(std::string(tagName), \
                                                      isSelfClosing);       \
                                                                            \
    auto& attributes = newNode->attributes;                                 \
    for (int i = 0; i < attributeNames.size(); i++) {                       \
        attributes.emplace_back(                                            \
            std::string(attributeNames[i]),                                 \
            attributeValues[i].second                                       \
                ? text::expandEntities(attributeValues[i].first)            \
                : std::string(attributeValues[i].first));                   \
    }                                                                       \
                                                                            \
    stack.back()->addChild(newNode);                                        \
    if (!isSelfClosing) {                                                   \
        stack.push_back(newNode);                                           \
    }                                                                       \
    attributeNames.clear();                                                 \
    attributeValues.clear();

#define CLOSE_ACTION(tagName, pos) \
    stack.pop_back();

    PARSE_BODY(false);

#undef TEXT_ACTION
#undef COMMENT_ACTION
#undef CDATA_ACTION
#undef INSTRUCTION_ACTION
#undef ATTRIBUTE_ACTION
#undef OPEN_ACTION
#undef XML_DECLARATION_ACTION
#undef DOCTYPE_ACTION
#undef CLOSE_ACTION

    // Invariant - stack may only contain the root
    if (stack.size() != 1 || stack[0] != root) {
        throw std::invalid_argument("Unclosed tags left");
    }

    if (root->getChildrenCount() == 1) {
        Node* newRoot = root->removeChild(root->getChildren()[0]).release();
        root = std::move(newRoot);
    }

    return ParseResult{std::move(arena), root};
}

NodeHandle DomParser::parse(std::istream& input) {
    std::vector<Node*> stack;
    std::vector<std::string> attributeNames;
    std::vector<std::pair<std::string, bool>> attributeValues;

    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);

    std::unique_ptr<Node> root = std::make_unique<tags::EmptyNode>();

    stack.push_back(root.get());

    skipWhitespace(pos);

#define TEXT_ACTION(text, hasEntities, pos)            \
    stack.back()->addChild(tags::Text(hasEntities ? text::expandEntities(text) : text));

#define COMMENT_ACTION(commentText, pos) \
    stack.back()->addChild(tags::Comment(commentText));

#define CDATA_ACTION(cdataText, pos) \
    stack.back()->addChild(tags::CData(cdataText));

#define INSTRUCTION_ACTION(tagName, processingInstruction, pos)         \
    stack.back()->addChild(tags::ProcessingInstruction(tagName, processingInstruction));

#define ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos) \
    attributeNames.push_back(attributeName);                              \
    attributeValues.push_back(std::make_pair(attributeValue, hasEntities));

#define XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, \
                               hasStandalone, pos)                           \
    stack.back()->addChild(tags::XmlDeclaration(             \
        version, encoding, hasEncoding,            \
        isStandalone, hasStandalone, false));

#define DOCTYPE_ACTION(doctypeText, pos) \
    stack.back()->addChild(tags::Doctype(doctypeText));

#define OPEN_ACTION(tagName, pos, isSelfClosing)                            \
    std::unique_ptr<Node> newNode = std::make_unique<tags::GenericNode>(tagName, isSelfClosing);       \
                                                                            \
    auto& attributes = newNode->attributes;                                 \
    for (int i = 0; i < attributeNames.size(); i++) {                       \
        attributes.emplace_back(                                            \
            attributeNames[i],                                 \
            attributeValues[i].second                                       \
                ? text::expandEntities(attributeValues[i].first)            \
                : attributeValues[i].first);                   \
    }                                                                       \
                                                                             \
    Node* newNodePtr = newNode.get();   \
    stack.back()->addChild(std::move(newNode));                                        \
    if (!isSelfClosing) {                                                   \
        stack.push_back(newNodePtr);                                           \
    }   \
                                                                          \
    attributeNames.clear();                                                 \
    attributeValues.clear();

#define CLOSE_ACTION(tagName, pos) \
    Node* thisNode = stack.back();                               \
    if (thisNode->getTagName() != tagName) {                            \
        throw std::invalid_argument("Closing unopened tag");      \
    }                                                             \
    if (stack.size() == 1) {                                      \
        throw std::invalid_argument("Closing non-existent tags"); \
    }                                                             \
    stack.pop_back();

    PARSE_BODY(true);

#undef TEXT_ACTION
#undef COMMENT_ACTION
#undef CDATA_ACTION
#undef INSTRUCTION_ACTION
#undef ATTRIBUTE_ACTION
#undef OPEN_ACTION
#undef XML_DECLARATION_ACTION
#undef DOCTYPE_ACTION
#undef CLOSE_ACTION

    // Invariant - stack may only contain the root
    if (stack.size() != 1 || stack[0] != root.get()) {
        throw std::invalid_argument("Unclosed tags left");
    }

    if (root->getChildrenCount() == 1) {
        NodeHandle newRoot = root->removeChild(root->getChildren()[0]);
        root = std::move(newRoot.toUnique());
    }

    return NodeHandle(std::move(root));
}
}  // namespace onyx::dynamic::parser