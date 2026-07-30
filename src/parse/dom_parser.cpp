#include "parse/dom_parser.h"

#include "parse/_parse_macro.h"
#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"

namespace onyx::dynamic::parser {

Arena DomParser::parseDryRun(std::string_view input) {
    struct DomDryRunParserPolicy {
        std::vector<size_t> stack;
        std::hash<std::string_view> hasher;
        Arena::Builder builder;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;

        inline void textAction(StringType text, bool hasEntities,
                               CursorType& cursor) {
            builder.preallocate<tags::Text>();
        }

        inline void commentAction(StringType commentText, CursorType& cursor) {
            builder.preallocate<tags::Comment>();
        }

        inline void cdataAction(StringType cdataText, CursorType& cursor) {
            builder.preallocate<tags::CData>();
        }

        inline void instructionAction(StringType tagName,
                                      StringType processingInstruction,
                                      CursorType& cursor) {
            builder.preallocate<tags::ProcessingInstruction>();
        }

        inline void attributeAction(StringType attributeName,
                                    StringType attributeValue, bool hasEntities,
                                    CursorType& cursor) {}

        inline void xmlDeclarationAction(StringType version,
                                         StringType encoding, bool hasEncoding,
                                         bool isStandalone, bool hasStandalone,
                                         CursorType& cursor) {
            builder.preallocate<tags::XmlDeclaration>();
        }

        inline void doctypeAction(StringType doctypeText, CursorType& cursor) {
            builder.preallocate<tags::Doctype>();
        }

        inline void openAction(StringType tagName, bool isSelfClosing,
                               CursorType& cursor) {
            builder.preallocate<tags::GenericNode>();
            if (!isSelfClosing) {
                stack.push_back(hasher(tagName));
            }
        }

        inline void closeAction(StringType tagName, CursorType& cursor) {
            size_t thisNode = stack.back();
            if (thisNode != hasher(tagName)) {
                throw std::invalid_argument("Closing unopened tag");
            } 
            if (stack.size() == 1) {
                throw std::invalid_argument("Closing non-existent tags");
            }
            stack.pop_back();
        }
    };

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());
    
    DomDryRunParserPolicy policy{};

    // Root
    policy.builder.preallocate<tags::EmptyNode>();

    std::string_view root = ".empty";

    policy.stack.push_back(policy.hasher(root));

    skipWhitespace(pos);

    parseBody<true, StringType, StringCursor, DomDryRunParserPolicy>(pos, policy);

    // Invariant - stack may only contain the root
    if (policy.stack.size() != 1 || policy.stack[0] != policy.hasher(root)) {
        throw std::invalid_argument("Unclosed tags left");
    }

    return policy.builder.build();
}

ParseResult<Arena> DomParser::parse(std::string_view input) {
    struct DomStringParserPolicy {
        Arena arena;
        std::vector<Node*> stack;
        std::vector<std::string_view> attributeNames;
        std::vector<std::pair<std::string_view, bool>> attributeValues;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;

        inline void textAction(StringType text, bool hasEntities,
                               CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(
                hasEntities ? text::expandEntities(text) : std::string(text)));
        }

        inline void commentAction(StringType commentText, CursorType& cursor) {
            stack.back()->addChild(
                    arena.allocate<tags::Comment>(std::string(commentText)));
        }

        inline void cdataAction(StringType cdataText, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::CData>(std::string(cdataText)));
        }

        inline void instructionAction(StringType tagName,
                                      StringType processingInstruction,
                                      CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(
                std::string(tagName), std::string(processingInstruction)));
        }

        inline void attributeAction(StringType attributeName,
                                    StringType attributeValue, bool hasEntities,
                                    CursorType& cursor) {
            attributeNames.push_back(attributeName);
            attributeValues.push_back(std::make_pair(attributeValue, hasEntities));
        }

        inline void xmlDeclarationAction(StringType version,
                                         StringType encoding, bool hasEncoding,
                                         bool isStandalone, bool hasStandalone,
                                         CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(
                std::string(version), std::string(encoding), hasEncoding,
                isStandalone, hasStandalone, false));
        }

        inline void doctypeAction(StringType doctypeText, CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Doctype>(std::string(doctypeText)));
        }

        inline void openAction(StringType tagName, bool isSelfClosing,
                               CursorType& cursor) {
            Node* newNode = arena.allocate<tags::GenericNode>(std::string(tagName),
                                                            isSelfClosing);

            auto& attributes = newNode->attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(
                    std::string(attributeNames[i]),
                    attributeValues[i].second
                        ? text::expandEntities(attributeValues[i].first)
                        : std::string(attributeValues[i].first));
            }

            stack.back()->addChild(newNode);
            if (!isSelfClosing) {
                stack.push_back(newNode);
            }
            attributeNames.clear();
            attributeValues.clear();
        }

        inline void closeAction(StringType tagName, CursorType& cursor) {
            stack.pop_back();
        }
    };

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());

    DomStringParserPolicy policy{std::move(parseDryRun(input))};

    Node* root = policy.arena.allocate<tags::EmptyNode>();

    policy.stack.push_back(root);

    skipWhitespace(pos);

    parseBody<false, StringType, StringCursor, DomStringParserPolicy>(pos, policy);

    // Invariant - stack may only contain the root
    if (policy.stack.size() != 1 || policy.stack[0] != root) {
        throw std::invalid_argument("Unclosed tags left");
    }

    if (root->getChildrenCount() == 1) {
        Node* newRoot = root->removeChild(root->getChildren()[0]).release();
        root = std::move(newRoot);
    }

    return ParseResult{std::move(policy.arena), root};
}

ParseResult<PagedArena> DomParser::parse(std::istream& input) {
    struct DomStreamParserPolicy {
        PagedArena arena;
        std::vector<Node*> stack;
        std::vector<std::string> attributeNames;
        std::vector<std::pair<std::string, bool>> attributeValues;

        using CursorType = StreamCursor;
        using StringType = CursorType::StringType;

        inline void textAction(StringType text, bool hasEntities,
                               CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(
                hasEntities ? text::expandEntities(text) : text));
        }

        inline void commentAction(StringType commentText, CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Comment>(std::move(commentText)));
        }

        inline void cdataAction(StringType cdataText, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::CData>(std::move(cdataText)));
        }

        inline void instructionAction(StringType tagName,
                                      StringType processingInstruction,
                                      CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(
                std::move(tagName), processingInstruction));
        }

        inline void attributeAction(StringType attributeName,
                                    StringType attributeValue, bool hasEntities,
                                    CursorType& cursor) {
            attributeNames.push_back(std::move(attributeName));
            attributeValues.push_back(
                std::make_pair(std::move(attributeValue), hasEntities));
        }

        inline void xmlDeclarationAction(StringType version,
                                         StringType encoding, bool hasEncoding,
                                         bool isStandalone, bool hasStandalone,
                                         CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(
                std::move(version), std::move(encoding), hasEncoding, isStandalone,
                hasStandalone, false));
        }

        inline void doctypeAction(StringType doctypeText, CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Doctype>(std::move(doctypeText)));
        }

        inline void openAction(StringType tagName, bool isSelfClosing,
                               CursorType& cursor) {
            Node* newNode =
                arena.allocate<tags::GenericNode>(std::move(tagName), isSelfClosing);

            auto& attributes = newNode->attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(
                    attributeNames[i],
                    attributeValues[i].second
                        ? text::expandEntities(attributeValues[i].first)
                        : attributeValues[i].first);
            }

            stack.back()->addChild(newNode);
            if (!isSelfClosing) {
                stack.push_back(newNode);
            }

            attributeNames.clear();
            attributeValues.clear();
        }

        inline void closeAction(StringType tagName, CursorType& cursor) {
            Node* thisNode = stack.back();
            if (thisNode->getTagName() != tagName) {
                throw std::invalid_argument("Closing unopened tag");
            }
            if (stack.size() == 1) {
                throw std::invalid_argument("Closing non-existent tags");
            }
            stack.pop_back();
        }
    };

    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);

    DomStreamParserPolicy policy{};

    Node* root = policy.arena.allocate<tags::EmptyNode>();

    policy.stack.push_back(root);

    skipWhitespace(pos);

    parseBody<true, StringType, StreamCursor, DomStreamParserPolicy>(pos, policy);

    // Invariant - stack may only contain the root
    if (policy.stack.size() != 1 || policy.stack[0] != root) {
        throw std::invalid_argument("Unclosed tags left");
    }

    if (root->getChildrenCount() == 1) {
        Node* newRoot = root->removeChild(root->getChildren()[0]).release();
        root = std::move(newRoot);
    }

    return ParseResult{std::move(policy.arena), root};
}
}  // namespace onyx::dynamic::parser