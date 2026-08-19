#include "parse/dom_parser.h"

#include <cstddef>
#include <limits>
#include <string_view>

#include "arena.h"
#include "nodes/cdata_node.h"
#include "nodes/comment_node.h"
#include "nodes/doctype_node.h"
#include "nodes/empty_node.h"
#include "nodes/generic_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/text_node.h"
#include "nodes/xml_declaration_node.h"
#include "paged_arena.h"
#include "parse/encoding_controller.h"
#include "parse/encoding_string_state.h"
#include "parse/helpers.h"
#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"
#include "parse/text_transformation_mode.h"
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

std::pair<Arena, EncodingStringState> DomParser::parseDryRun(
    std::string_view input, std::string encoding) {
    struct DomDryRunParserPolicy {
        Arena::Builder builder;
        std::string_view root = ".empty";
        EncodingController& ec;
        EncodingStringState& inputState;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;
        using StackType = std::string_view;
        using Stack = std::vector<StackType>;

        ONYX_INLINE void textAction(StringType text, Stack& stack,
                                    CursorType& cursor) {
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

        ONYX_INLINE void openAction(StringType tagName, bool isSelfClosing,
                                    std::vector<StringType>& attributeNames,
                                    std::vector<StringType>& attributeValues,
                                    std::vector<StackType>& stack,
                                    CursorType& cursor) {
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
                                                CursorType::StringType& tag) {
            return el == tag;
        }

        ONYX_INLINE bool isStackRoot(StringType& stackElement) {
            return stackElement == root;
        }

        ONYX_INLINE StringType transformText(CursorType::StringType&& text,
                                             TextTransformationMode ttm) {
            return text;
        }

        ONYX_INLINE bool foundEncoding(
            CursorType::StringType&& discoveredEncoding, CursorType& cursor,
            bool& validateUTF8) {
            std::string enc = text::asciiToUpper(discoveredEncoding);
            EncodingController::ParserAction action =
                ec.foundEncoding(enc, true);

            if (action == EncodingController::ParserAction::RESTART) {
                return false;
            }

            validateUTF8 = ec.validate;
            return true;
        }
    };

    EncodingStringState inputState(input);
    Arena arena = Arena::Builder().build();

    auto parseLocal = [&inputState, &arena](EncodingController& ec) {
        using StringType = StringCursor::StringType;
        StringCursor pos(inputState.getInput());

        DomDryRunParserPolicy policy{.ec = ec, .inputState = inputState};

        policy.builder.preallocate<tags::EmptyNode>();

        skipWhitespace(pos);

        parseBody<DryRunConfig, DomDryRunParserPolicy>(pos, policy,
                                                       ec.validate);

        arena = std::move(policy.builder.build());
    };

    auto transcode = [&inputState](EncodingController& ec) -> bool {
        return inputState.transcodeToUTF8(ec.encoding);
    };

    EncodingController encodingController(parseLocal, transcode);
    encodingController.begin(encoding);

    encodingController.triggerParseIfWaiting();

    return std::make_pair(std::move(arena), std::move(inputState));
}

ParseResult<Arena> DomParser::parse(std::string_view input,
                                    std::string encoding) {
    struct DomStringParserPolicy {
        Arena arena;

        using CursorType = StringCursor;
        using StringType = std::string;
        using StackType = Node*;
        using Stack = std::vector<StackType>;

        Node* root = arena.allocate<tags::EmptyNode>();

        ONYX_INLINE void textAction(StringType&& text, Stack& stack,
                                    CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(std::move(text)));
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
                std::move(tagName), std::move(processingInstruction)));
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

        ONYX_INLINE void openAction(StringType&& tagName, bool isSelfClosing,
                                    std::vector<StringType>& attributeNames,
                                    std::vector<StringType>& attributeValues,
                                    Stack& stack, CursorType& cursor) {
            Node* newNode = arena.allocate<tags::GenericNode>(
                std::move(tagName), isSelfClosing);

            auto& attributes = newNode->attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(std::move(attributeNames[i]),
                                        std::move(attributeValues[i]));
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
                                                CursorType::StringType& tag) {
            return el->getTagName() == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }

        ONYX_INLINE StringType transformText(CursorType::StringType&& text,
                                             TextTransformationMode ttm) {
            switch (ttm) {
                case TextTransformationMode::TEXT:
                    return text::expandText(text);
                case TextTransformationMode::ATTRIBUTE:
                    return text::expandAttributeValue(text);
                case TextTransformationMode::EOL_ONLY:
                    return text::expandEOLOnly(text);
                case TextTransformationMode::NONE:
                    return std::string(text);
                case TextTransformationMode::UPPERCASE:
                    return text::asciiToUpper(text);
            }
            return std::string(text);
        }

        ONYX_INLINE bool foundEncoding(CursorType::StringType&& text,
                                       CursorType& cursor, bool& validateUTF8) {
            return true;
        }
    };

    std::pair<Arena, EncodingStringState> dryRunResult =
        std::move(parseDryRun(input, encoding));

    using StringType = StringCursor::StringType;
    StringCursor pos(dryRunResult.second.getInput());

    DomStringParserPolicy policy{std::move(dryRunResult.first)};

    skipWhitespace(pos);

    parseBody<NonValidatingConfig, DomStringParserPolicy>(pos, policy);

    if (policy.root->getChildrenCount() == 1) {
        Node* newRoot =
            policy.root->removeChild(policy.root->getChildren()[0]).release();
        policy.root = std::move(newRoot);
    }

    return ParseResult{std::move(policy.arena), policy.root};
}

ParseResult<PagedArena> DomParser::parse(std::istream& input,
                                         std::string encoding) {
    struct DomStreamParserPolicy {
        PagedArena arena;
        EncodingController& ec;

        using CursorType = StreamCursor;
        using StringType = CursorType::StringType;
        using StackType = Node*;
        using Stack = std::vector<StackType>;

        Node* root = arena.allocate<tags::EmptyNode>();

        ONYX_INLINE void textAction(StringType&& text, Stack& stack,
                                    CursorType& cursor) {
            stack.back()->addChild(arena.allocate<tags::Text>(std::move(text)));
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

        ONYX_INLINE void openAction(StringType&& tagName, bool isSelfClosing,
                                    std::vector<StringType>& attributeNames,
                                    std::vector<StringType>& attributeValues,
                                    Stack& stack, CursorType& cursor) {
            Node* newNode = arena.allocate<tags::GenericNode>(
                std::move(tagName), isSelfClosing);

            auto& attributes = newNode->attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(std::move(attributeNames[i]),
                                        std::move(attributeValues[i]));
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
                                                CursorType::StringType& tag) {
            return el->getTagName() == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }

        ONYX_INLINE StringType transformText(CursorType::StringType&& text,
                                             TextTransformationMode ttm) {
            switch (ttm) {
                case TextTransformationMode::TEXT:
                    return text::expandText(std::move(text));
                case TextTransformationMode::ATTRIBUTE:
                    return text::expandAttributeValue(std::move(text));
                case TextTransformationMode::EOL_ONLY:
                    return text::expandEOLOnly(std::move(text));
                case TextTransformationMode::NONE:
                    return text;
                case TextTransformationMode::UPPERCASE:
                    text::transformAsciiToUpper(text);
                    return text;
            }
            return text;
        }

        ONYX_INLINE bool foundEncoding(
            CursorType::StringType&& discoveredEncoding, CursorType& cursor,
            bool& validateUTF8) {
            ec.foundEncoding(discoveredEncoding, false);

            validateUTF8 = ec.validate;
            return true;
        }
    };

    PagedArena arena;
    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);
    Node* root;

    auto parseLocal = [&pos, &arena, &root](EncodingController& ec) {
        DomStreamParserPolicy policy{.ec = ec};

        skipWhitespace(pos);

        parseBody<ValidatingConfig, DomStreamParserPolicy>(pos, policy,
                                                           ec.validate);

        if (policy.root->getChildrenCount() == 1) {
            Node* newRoot =
                policy.root->removeChild(policy.root->getChildren()[0])
                    .release();
            policy.root = newRoot;
        }

        arena = std::move(policy.arena);
        root = policy.root;
    };

    auto transcode = [&pos](EncodingController& ec) -> bool {
        return pos.setInputEncoding(ec.encoding);
    };

    EncodingController encodingController(parseLocal, transcode);
    encodingController.begin(encoding);

    return ParseResult{std::move(arena), root};
}
}  // namespace onyx::dynamic::parser