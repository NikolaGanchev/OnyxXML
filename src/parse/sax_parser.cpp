#include "parse/sax_parser.h"

#include <algorithm>
#include <limits>
#include <string>
#include <string_view>

#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"
#include "text.h"

namespace onyx::dynamic::parser {

SaxParser::SaxParser(SaxListener& listener) : listener(listener) {}

struct ValidatingConfig {
    constexpr static bool validate = true;
    constexpr static bool validateDuplicateAttributes = true;
    constexpr static size_t maxAttributeCount =
        std::numeric_limits<size_t>::max();
};

void SaxParser::parse(std::string_view input, std::string encoding) {
    struct StringSaxParserPolicy {
        SaxListener& listener;
        std::string_view root = ".empty";
        std::string encoding;
        std::string_view input;
        std::optional<std::string> transcodedString;
        bool alreadyTranscoded = false;

        using CursorType = StringCursor;
        using StringType = std::string;
        using StackType = std::string;
        using Stack = std::vector<StackType>;

        ONYX_INLINE void textAction(StringType&& text, Stack& stack,
                                    CursorType& cursor) {
            this->listener.onText(std::move(text));
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

        ONYX_INLINE void openAction(StringType&& tagName, bool isSelfClosing,
                                    std::vector<StringType>& attributeNames,
                                    std::vector<StringType>& attributeValues,
                                    Stack& stack, CursorType& cursor) {
            std::vector<Attribute> attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(std::move(attributeNames[i]),
                                        std::move(attributeValues[i]));
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
            this->listener.onTagClose(std::move(tagName));
        }

        ONYX_INLINE void initStack(std::vector<StackType>& stack) {
            stack.push_back(std::string(root));
        }

        ONYX_INLINE bool equalStackElementToTag(StackType& el,
                                                CursorType::StringType& tag) {
            return el == tag;
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

        ONYX_INLINE bool foundEncoding(
            CursorType::StringType&& discoveredEncoding, CursorType& cursor,
            bool& validateUTF8) {
            if (!encoding.empty()) {
                if (discoveredEncoding != encoding) {
                    throw std::invalid_argument(
                        "Declared encoding does not match given encoding");
                }

                return true;
            }

            encoding = std::move(discoveredEncoding);

            if (alreadyTranscoded) {
                return true;
            }

            std::optional<std::string> res =
                text::transcodeToUtf8(input, encoding);

            if (!res.has_value()) return true;

            transcodedString = std::move(res);

            // It is only safe to stop parsing because it is guarantee no events
            // could have been triggered before the XML declaration except the
            // onStart event
            return false;
        }
    };

    std::optional<std::string> transcodedString;
    if (!encoding.empty()) {
        transcodedString = text::transcodeToUtf8(input, encoding);

        if (transcodedString.has_value()) {
            input = transcodedString->data();
        }
    }

    using StringType = StringCursor::StringType;
    StringCursor pos(input);

    StringSaxParserPolicy policy{
        .listener = this->listener, .encoding = encoding, .input = input};

    this->listener.onStart();

    try {
        parseBody<ValidatingConfig, StringSaxParserPolicy>(
            pos, policy, !transcodedString.has_value());
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    if (encoding.empty()) {
        transcodedString = std::move(policy.transcodedString);
    }
    bool hasFoundEncoding = encoding.empty() && transcodedString.has_value();
    if (hasFoundEncoding) {
        StringCursor posTranscoded(transcodedString->data());

        StringSaxParserPolicy policyTranscoded = {
            .listener = this->listener,
            .encoding = policy.encoding,
            .input = transcodedString->data(),
            .alreadyTranscoded = true};

        skipWhitespace(posTranscoded);

        try {
            parseBody<ValidatingConfig, StringSaxParserPolicy>(
                posTranscoded, policyTranscoded, false);
        } catch (std::exception& e) {
            this->listener.onException(e);
        }
    }

    this->listener.onEnd();
}

void SaxParser::parse(std::istream& input, std::string encoding) {
    struct StreamSaxParserPolicy {
        SaxListener& listener;
        std::vector<std::string> stack;
        std::string_view root = ".empty";
        std::string_view encoding;

        using CursorType = StreamCursor;
        using StringType = StreamCursor::StringType;
        using StackType = std::string;
        using Stack = std::vector<StackType>;

        ONYX_INLINE void textAction(StringType&& text, Stack& stack,
                                    CursorType& cursor) {
            this->listener.onText(std::move(text));
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

        ONYX_INLINE void openAction(StringType&& tagName, bool isSelfClosing,
                                    std::vector<StringType>& attributeNames,
                                    std::vector<StringType>& attributeValues,
                                    Stack& stack, CursorType& cursor) {
            std::vector<Attribute> attributes;
            for (int i = 0; i < attributeNames.size(); i++) {
                attributes.emplace_back(attributeNames[i],
                                        std::move(attributeValues[i]));
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
            this->listener.onTagClose(std::move(tagName));
        }

        ONYX_INLINE void initStack(std::vector<StackType>& stack) {
            stack.push_back(std::string(root));
        }

        ONYX_INLINE bool equalStackElementToTag(StackType& el,
                                                CursorType::StringType& tag) {
            return el == tag;
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
            if (!encoding.empty()) {
                if (discoveredEncoding != encoding) {
                    throw std::invalid_argument(
                        "Declared encoding does not match given encoding");
                }
            }

            validateUTF8 = !cursor.setInputEncoding(discoveredEncoding);

            return true;
        }
    };

    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);

    bool validateUTF8 = true;
    if (!encoding.empty()) {
        validateUTF8 = !pos.setInputEncoding(encoding);
    }

    StreamSaxParserPolicy policy{.listener = this->listener,
                                 .encoding = encoding};

    skipWhitespace(pos);

    this->listener.onStart();
    try {
        parseBody<ValidatingConfig, StreamSaxParserPolicy>(pos, policy,
                                                           validateUTF8);
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    this->listener.onEnd();
}
}  // namespace onyx::dynamic::parser