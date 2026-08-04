#include "parse/sax_parser.h"

#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"


namespace onyx::dynamic::parser {

SaxParser::SaxParser(SaxListener& listener) : listener(listener) {}

struct ValidatingConfig {
    constexpr static bool validate = true;
};

void SaxParser::parse(std::string_view input) {
    struct StringSaxParserPolicy {
        SaxListener& listener;
        std::string_view root = ".empty";

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;
        using StackType = std::string_view;
        using Stack = std::vector<StackType>;

        ONYX_INLINE void textAction(StringType text, bool hasEntities,
                                    Stack& stack, CursorType& cursor) {
            this->listener.onText(hasEntities ? text::expandEntities(text)
                                              : std::string(text));
        }

        ONYX_INLINE void commentAction(StringType commentText, Stack& stack,
                                       CursorType& cursor) {
            this->listener.onComment(std::string(commentText));
        }

        ONYX_INLINE void cdataAction(StringType cdataText, Stack& stack,
                                     CursorType& cursor) {
            this->listener.onCData(std::string(cdataText));
        }

        ONYX_INLINE void instructionAction(StringType tagName,
                                           StringType processingInstruction,
                                           Stack& stack, CursorType& cursor) {
            this->listener.onInstruction(std::string(tagName),
                                         std::string(processingInstruction));
        }

        ONYX_INLINE void xmlDeclarationAction(StringType version,
                                              StringType encoding,
                                              bool hasEncoding,
                                              bool isStandalone,
                                              bool hasStandalone, Stack& stack,
                                              CursorType& cursor) {
            this->listener.onXMLDeclaration(std::string(version),
                                            std::string(encoding), hasEncoding,
                                            isStandalone, hasStandalone);
        }

        ONYX_INLINE void doctypeAction(StringType doctypeText, Stack& stack,
                                       CursorType& cursor) {
            this->listener.onDoctype(std::string(doctypeText));
        }

        ONYX_INLINE void openAction(
            StringType tagName, bool isSelfClosing,
            std::vector<StringType>& attributeNames,
            std::vector<std::pair<StringType, bool>>& attributeValues,
            Stack& stack, CursorType& cursor) {
            std::vector<Attribute> attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(
                    std::string(attributeNames[i]),
                    attributeValues[i].second
                        ? text::expandEntities(attributeValues[i].first)
                        : std::string(attributeValues[i].first));
            }

            if (!isSelfClosing) {
                stack.push_back(tagName);
            }
            this->listener.onTagOpen(std::string(tagName), isSelfClosing,
                                     std::move(attributes));
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
            return el == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }
    };

    using StringType = StringCursor::StringType;
    StringCursor pos(input.data());

    StringSaxParserPolicy policy{this->listener};

    skipWhitespace(pos);

    this->listener.onStart();

    try {
        parseBody<ValidatingConfig, StringSaxParserPolicy>(pos, policy);
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    this->listener.onEnd();
}

void SaxParser::parse(std::istream& input) {
    struct StreamSaxParserPolicy {
        SaxListener& listener;
        std::vector<std::string> stack;
        std::string_view root = ".empty";

        using CursorType = StreamCursor;
        using StringType = StreamCursor::StringType;
        using StackType = std::string;
        using Stack = std::vector<StackType>;

        ONYX_INLINE void textAction(StringType&& text, bool hasEntities,
                                    Stack& stack, CursorType& cursor) {
            this->listener.onText(hasEntities ? text::expandEntities(text)
                                              : std::move(text));
        }

        ONYX_INLINE void commentAction(StringType&& commentText, Stack& stack,
                                       CursorType& cursor) {
            this->listener.onComment(std::move(commentText));
        }

        ONYX_INLINE void cdataAction(StringType&& cdataText, Stack& stack,
                                     CursorType& cursor) {
            this->listener.onCData(std::move(cdataText));
        }

        ONYX_INLINE void instructionAction(StringType&& tagName,
                                           StringType&& processingInstruction,
                                           Stack& stack, CursorType& cursor) {
            this->listener.onInstruction(std::move(tagName),
                                         std::move(processingInstruction));
        }

        ONYX_INLINE void xmlDeclarationAction(StringType&& version,
                                              StringType&& encoding,
                                              bool hasEncoding,
                                              bool isStandalone,
                                              bool hasStandalone, Stack& stack,
                                              CursorType& cursor) {
            this->listener.onXMLDeclaration(std::move(version),
                                            std::move(encoding), hasEncoding,
                                            isStandalone, hasStandalone);
        }

        ONYX_INLINE void doctypeAction(StringType&& doctypeText, Stack& stack,
                                       CursorType& cursor) {
            this->listener.onDoctype(std::move(doctypeText));
        }

        ONYX_INLINE void openAction(
            StringType&& tagName, bool isSelfClosing,
            std::vector<StringType>& attributeNames,
            std::vector<std::pair<StringType, bool>>& attributeValues,
            Stack& stack, CursorType& cursor) {
            std::vector<Attribute> attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(
                    attributeNames[i],
                    attributeValues[i].second
                        ? text::expandEntities(attributeValues[i].first)
                        : std::move(attributeValues[i].first));
            }

            if (!isSelfClosing) {
                stack.push_back(tagName);
            }
            this->listener.onTagOpen(std::move(tagName), isSelfClosing,
                                     std::move(attributes));
        }

        ONYX_INLINE void closeAction(StringType&& tagName, Stack& stack,
                                     CursorType& cursor) {
            stack.pop_back();
        }

        ONYX_INLINE void initStack(std::vector<StackType>& stack) {
            stack.push_back(std::string(root));
        }

        ONYX_INLINE bool equalStackElementToTag(StackType& el,
                                                StringType& tag) {
            return el == tag;
        }

        ONYX_INLINE bool isStackRoot(StackType& stackElement) {
            return stackElement == root;
        }
    };

    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);

    StreamSaxParserPolicy policy{this->listener};

    skipWhitespace(pos);

    this->listener.onStart();
    try {
        parseBody<ValidatingConfig, StreamSaxParserPolicy>(pos, policy);
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    this->listener.onEnd();
}
}  // namespace onyx::dynamic::parser