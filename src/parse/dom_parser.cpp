#include "parse/dom_parser.h"

#include <cstddef>
#include <limits>
#include <string_view>

#include "nodes/cdata_node.h"
#include "nodes/comment_node.h"
#include "nodes/doctype_node.h"
#include "nodes/empty_node.h"
#include "nodes/generic_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/text_node.h"
#include "nodes/xml_declaration_node.h"
#include "parse/helpers.h"
#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"
#include "text.h"

namespace onyx::dynamic::parser {

struct DryRunConfig {
    constexpr static bool validate = true;
    constexpr static bool validateDuplicateAttributes = true;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};

struct ValidatingConfig {
    constexpr static bool validate = true;
    constexpr static bool validateDuplicateAttributes = true;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};

struct NonValidatingConfig {
    constexpr static bool validate = false;
    constexpr static bool validateDuplicateAttributes = false;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};

Arena DomParser::parseDryRun(std::string_view input) {
    struct DomDryRunParserPolicy {
        Arena::Builder builder;
        std::string_view root = ".empty";

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;
        using StackType = std::string_view;
        using Stack = std::vector<StackType>;

        ONYX_INLINE void textAction(StringType text, bool requiresExpansion,
                                    Stack& stack, CursorType& cursor) {
            builder.preallocate<tags::Text>();
        }

        ONYX_INLINE void commentAction(StringType commentText, Stack& stack,
                                       CursorType& cursor) {
            builder.preallocate<tags::Comment>();
        }

        ONYX_INLINE void cdataAction(StringType cdataText, Stack& stack,
                                     CursorType& cursor) {
            builder.preallocate<tags::CData>();
        }

        ONYX_INLINE void instructionAction(StringType tagName,
                                           StringType processingInstruction,
                                           Stack& stack, CursorType& cursor) {
            builder.preallocate<tags::ProcessingInstruction>();
        }

        ONYX_INLINE void xmlDeclarationAction(StringType version,
                                              StringType encoding,
                                              bool hasEncoding,
                                              bool isStandalone,
                                              bool hasStandalone, Stack& stack,
                                              CursorType& cursor) {
            builder.preallocate<tags::XmlDeclaration>();
        }

        ONYX_INLINE void doctypeAction(StringType doctypeText, Stack& stack,
                                       CursorType& cursor) {
            builder.preallocate<tags::Doctype>();
        }

        ONYX_INLINE void openAction(
            StringType tagName, bool isSelfClosing,
            std::vector<StringType>& attributeNames,
            std::vector<std::pair<StringType, bool>>& attributeValues,
            std::vector<StackType>& stack, CursorType& cursor) {
            builder.preallocate<tags::GenericNode>();
            if (!isSelfClosing) {
                stack.push_back(tagName);
            }
        }

        ONYX_INLINE void closeAction(StringType tagName,
                                     std::vector<StackType>& stack,
                                     CursorType& cursor) {
            stack.pop_back();
        }

        ONYX_INLINE void initStack(std::vector<StackType>& stack) {
            stack.push_back(root);
        }

        ONYX_INLINE bool equalStackElementToTag(StackType& el,
                                                StringType& tag) {
            return el == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }
    };

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());

    DomDryRunParserPolicy policy{};

    // Root
    policy.builder.preallocate<tags::EmptyNode>();

    skipWhitespace(pos);

    parseBody<DryRunConfig, DomDryRunParserPolicy>(pos, policy);

    return policy.builder.build();
}

ParseResult<Arena> DomParser::parse(std::string_view input) {
    struct DomStringParserPolicy {
        Arena arena;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;
        using StackType = Node*;
        using Stack = std::vector<StackType>;

        Node* root = arena.allocate<tags::EmptyNode>();

        ONYX_INLINE void textAction(StringType text, bool requiresExpansion,
                                    Stack& stack, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(
                requiresExpansion ? text::expandEntities(text) : std::string(text)));
        }

        ONYX_INLINE void commentAction(StringType commentText, Stack& stack,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Comment>(std::string(commentText)));
        }

        ONYX_INLINE void cdataAction(StringType cdataText, Stack& stack,
                                     CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::CData>(std::string(cdataText)));
        }

        ONYX_INLINE void instructionAction(StringType tagName,
                                           StringType processingInstruction,
                                           Stack& stack, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(
                std::string(tagName), std::string(processingInstruction)));
        }

        ONYX_INLINE void xmlDeclarationAction(StringType version,
                                              StringType encoding,
                                              bool hasEncoding,
                                              bool isStandalone,
                                              bool hasStandalone, Stack& stack,
                                              CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(
                std::string(version), std::string(encoding), hasEncoding,
                isStandalone, hasStandalone, false));
        }

        ONYX_INLINE void doctypeAction(StringType doctypeText, Stack& stack,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Doctype>(std::string(doctypeText)));
        }

        ONYX_INLINE void openAction(
            StringType tagName, bool isSelfClosing,
            std::vector<StringType>& attributeNames,
            std::vector<std::pair<StringType, bool>>& attributeValues,
            Stack& stack, CursorType& cursor) {
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
        }

        ONYX_INLINE void closeAction(StringType tagName, Stack& stack,
                                     CursorType& cursor) {
            stack.pop_back();
        }

        ONYX_INLINE void initStack(std::vector<StackType>& stack) {
            stack.push_back(root);
        }

        ONYX_INLINE bool equalStackElementToTag(StackType& el,
                                                StringType& tag) {
            return el->getTagName() == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }
    };

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());

    DomStringParserPolicy policy{std::move(parseDryRun(input))};

    skipWhitespace(pos);

    parseBody<NonValidatingConfig, DomStringParserPolicy>(pos, policy);

    if (policy.root->getChildrenCount() == 1) {
        Node* newRoot =
            policy.root->removeChild(policy.root->getChildren()[0]).release();
        policy.root = std::move(newRoot);
    }

    return ParseResult{std::move(policy.arena), policy.root};
}

ParseResult<PagedArena> DomParser::parse(std::istream& input) {
    struct DomStreamParserPolicy {
        PagedArena arena;

        using CursorType = StreamCursor;
        using StringType = CursorType::StringType;
        using StackType = Node*;
        using Stack = std::vector<StackType>;

        Node* root = arena.allocate<tags::EmptyNode>();

        ONYX_INLINE void textAction(StringType&& text, bool requiresExpansion,
                                    Stack& stack, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(
                requiresExpansion ? text::expandEntities(text) : std::move(text)));
        }

        ONYX_INLINE void commentAction(StringType&& commentText, Stack& stack,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Comment>(std::move(commentText)));
        }

        ONYX_INLINE void cdataAction(StringType&& cdataText, Stack& stack,
                                     CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::CData>(std::move(cdataText)));
        }

        ONYX_INLINE void instructionAction(StringType&& tagName,
                                           StringType&& processingInstruction,
                                           Stack& stack, CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(
                std::move(tagName), processingInstruction));
        }

        ONYX_INLINE void xmlDeclarationAction(StringType&& version,
                                              StringType&& encoding,
                                              bool hasEncoding,
                                              bool isStandalone,
                                              bool hasStandalone, Stack& stack,
                                              CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(
                std::move(version), std::move(encoding), hasEncoding,
                isStandalone, hasStandalone, false));
        }

        ONYX_INLINE void doctypeAction(StringType&& doctypeText, Stack& stack,
                                       CursorType& cursor) {
            stack.back()->addChild(
                arena.allocate<tags::Doctype>(std::move(doctypeText)));
        }

        ONYX_INLINE void openAction(
            StringType&& tagName, bool isSelfClosing,
            std::vector<StringType>& attributeNames,
            std::vector<std::pair<StringType, bool>>& attributeValues,
            Stack& stack, CursorType& cursor) {
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
        }

        ONYX_INLINE void closeAction(StringType&& tagName, Stack& stack,
                                     CursorType& cursor) {
            stack.pop_back();
        }

        ONYX_INLINE void initStack(std::vector<StackType>& stack) {
            stack.push_back(root);
        }

        ONYX_INLINE bool equalStackElementToTag(StackType& el,
                                                StringType& tag) {
            return el->getTagName() == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }
    };

    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);

    DomStreamParserPolicy policy{};

    skipWhitespace(pos);

    parseBody<ValidatingConfig, DomStreamParserPolicy>(pos, policy);

    if (policy.root->getChildrenCount() == 1) {
        Node* newRoot =
            policy.root->removeChild(policy.root->getChildren()[0]).release();
        policy.root = std::move(newRoot);
    }

    return ParseResult{std::move(policy.arena), policy.root};
}
}  // namespace onyx::dynamic::parser