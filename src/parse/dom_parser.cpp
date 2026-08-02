#include "parse/dom_parser.h"

#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"

#include "nodes/cdata_node.h"
#include "nodes/comment_node.h"
#include "nodes/doctype_node.h"
#include "nodes/empty_node.h"
#include "nodes/generic_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/text_node.h"
#include "nodes/xml_declaration_node.h"
#include "text.h"

namespace onyx::dynamic::parser {

Arena DomParser::parseDryRun(std::string_view input) {
    struct DomDryRunParserPolicy {
        std::vector<std::string_view> stack;
        Arena::Builder builder;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;

        ONYX_INLINE void textAction(StringType text, bool hasEntities,
                                    CursorType& cursor) {
            builder.preallocate<tags::Text>();
        }

        ONYX_INLINE void commentAction(StringType commentText,
                                       CursorType& cursor) {
            builder.preallocate<tags::Comment>();
        }

        ONYX_INLINE void cdataAction(StringType cdataText, CursorType& cursor) {
            builder.preallocate<tags::CData>();
        }

        ONYX_INLINE void instructionAction(StringType tagName,
                                           StringType processingInstruction,
                                           CursorType& cursor) {
            builder.preallocate<tags::ProcessingInstruction>();
        }

        ONYX_INLINE void xmlDeclarationAction(
            StringType version, StringType encoding, bool hasEncoding,
            bool isStandalone, bool hasStandalone, CursorType& cursor) {
            builder.preallocate<tags::XmlDeclaration>();
        }

        ONYX_INLINE void doctypeAction(StringType doctypeText,
                                       CursorType& cursor) {
            builder.preallocate<tags::Doctype>();
        }

        ONYX_INLINE void openAction(
            StringType tagName, bool isSelfClosing,
            std::vector<StringType> attributeNames,
            std::vector<std::pair<StringType, bool>> attributeValues,
            CursorType& cursor) {
            builder.preallocate<tags::GenericNode>();
            if (!isSelfClosing) {
                stack.push_back(tagName);
            }
        }

        ONYX_INLINE void closeAction(StringType tagName, CursorType& cursor) {
            if (stack.back() != tagName) {
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

    policy.stack.push_back(root);

    skipWhitespace(pos);

    parseBody<true, StringType, StringCursor, DomDryRunParserPolicy>(pos,
                                                                     policy);

    // Invariant - stack may only contain the root
    if (policy.stack.size() != 1 || policy.stack[0] != root) {
        throw std::invalid_argument("Unclosed tags left");
    }

    return policy.builder.build();
}

ParseResult<Arena> DomParser::parse(std::string_view input) {
    struct DomStringParserPolicy {
        Arena arena;
        std::vector<Node*> stack;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;

        ONYX_INLINE void textAction(StringType text, bool hasEntities,
                                    CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(
                hasEntities ? text::expandEntities(text) : std::string(text)));
        }

        ONYX_INLINE void commentAction(StringType commentText,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Comment>(std::string(commentText)));
        }

        ONYX_INLINE void cdataAction(StringType cdataText, CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::CData>(std::string(cdataText)));
        }

        ONYX_INLINE void instructionAction(StringType tagName,
                                           StringType processingInstruction,
                                           CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(
                std::string(tagName), std::string(processingInstruction)));
        }

        ONYX_INLINE void xmlDeclarationAction(
            StringType version, StringType encoding, bool hasEncoding,
            bool isStandalone, bool hasStandalone, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(
                std::string(version), std::string(encoding), hasEncoding,
                isStandalone, hasStandalone, false));
        }

        ONYX_INLINE void doctypeAction(StringType doctypeText,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Doctype>(std::string(doctypeText)));
        }

        ONYX_INLINE void openAction(
            StringType tagName, bool isSelfClosing,
            std::vector<StringType> attributeNames,
            std::vector<std::pair<StringType, bool>> attributeValues,
            CursorType& cursor) {
            Node* newNode = arena.allocate<tags::GenericNode>(
                std::string(tagName), isSelfClosing);

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

    parseBody<false, StringType, StringCursor, DomStringParserPolicy>(pos,
                                                                      policy);

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

        using CursorType = StreamCursor;
        using StringType = CursorType::StringType;

        ONYX_INLINE void textAction(StringType&& text, bool hasEntities,
                                    CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(
                hasEntities ? text::expandEntities(text) : std::move(text)));
        }

        ONYX_INLINE void commentAction(StringType&& commentText,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Comment>(std::move(commentText)));
        }

        ONYX_INLINE void cdataAction(StringType&& cdataText,
                                     CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::CData>(std::move(cdataText)));
        }

        ONYX_INLINE void instructionAction(StringType&& tagName,
                                           StringType&& processingInstruction,
                                           CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(
                std::move(tagName), processingInstruction));
        }

        ONYX_INLINE void xmlDeclarationAction(
            StringType&& version, StringType&& encoding, bool hasEncoding,
            bool isStandalone, bool hasStandalone, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(
                std::move(version), std::move(encoding), hasEncoding,
                isStandalone, hasStandalone, false));
        }

        ONYX_INLINE void doctypeAction(StringType&& doctypeText,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Doctype>(std::move(doctypeText)));
        }

        ONYX_INLINE void openAction(
            StringType&& tagName, bool isSelfClosing,
            std::vector<StringType> attributeNames,
            std::vector<std::pair<StringType, bool>> attributeValues,
            CursorType& cursor) {
            Node* newNode = arena.allocate<tags::GenericNode>(
                std::move(tagName), isSelfClosing);

            auto& attributes = newNode->attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(
                    std::move(attributeNames[i]),
                    attributeValues[i].second
                        ? text::expandEntities(attributeValues[i].first)
                        : std::move(attributeValues[i].first));
            }

            stack.back()->addChild(newNode);
            if (!isSelfClosing) {
                stack.push_back(newNode);
            }

            attributeNames.clear();
            attributeValues.clear();
        }

        ONYX_INLINE void closeAction(StringType&& tagName, CursorType& cursor) {
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

    parseBody<true, StringType, StreamCursor, DomStreamParserPolicy>(pos,
                                                                     policy);

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