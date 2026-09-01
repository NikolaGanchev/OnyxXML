#include "parse/sax_parser.h"

#include <limits>
#include <string>
#include <string_view>

#include "parse/basic_autodetection_parser_policy.h"
#include "parse/common_parser_configs.h"
#include "parse/encoding_controller.h"
#include "parse/encoding_string_state.h"
#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"
#include "parse/string_view_read_buffer.h"
#include "text.h"

namespace onyx::dynamic::parser {

SaxParser::SaxParser(SaxListener& listener) : listener(listener) {}
struct StringSaxParserPolicy {
    SaxListener& listener;
    std::string_view root = ".empty";
    EncodingController& ec;
    EncodingStringState& inputState;

    using CursorType = StringCursor;
    using StringType = std::string;
    using StackType = std::pair<std::string, std::string>;
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
                                          bool hasEncoding, bool isStandalone,
                                          bool hasStandalone, Stack& stack,
                                          CursorType& cursor) {
        this->listener.onXMLDeclaration(std::move(version), std::move(encoding),
                                        hasEncoding, isStandalone,
                                        hasStandalone);
    }

    ONYX_INLINE void doctypeAction(StringType&& doctypeText, Stack& stack,
                                   CursorType& cursor) {
        this->listener.onDoctype(std::move(doctypeText));
    }

    ONYX_INLINE void openAction(StringType&& namespacePrefix,
                                StringType&& tagName, bool isSelfClosing,
                                std::vector<StringType>& attributeNames,
                                std::vector<StringType>& attributeValues,
                                Stack& stack, CursorType& cursor) {
        std::vector<Attribute> attributes;
        for (int i = 0; i < attributeNames.size(); i++) {
            attributes.emplace_back(std::move(attributeNames[i]),
                                    std::move(attributeValues[i]));
        }

        if (!isSelfClosing) {
            stack.push_back({namespacePrefix, tagName});
        }
        this->listener.onTagOpen(std::move(namespacePrefix), std::move(tagName),
                                 isSelfClosing, std::move(attributes));
    }

    ONYX_INLINE void closeAction(StringType&& namespacePrefix,
                                 StringType&& tagName, Stack& stack,
                                 CursorType& cursor) {
        stack.pop_back();
        this->listener.onTagClose(std::move(namespacePrefix),
                                  std::move(tagName));
    }

    ONYX_INLINE void initStack(std::vector<StackType>& stack) {
        stack.push_back({"", std::string(root)});
    }

    ONYX_INLINE bool equalStackElementToTag(
        StackType& el, CursorType::StringType& namespacePrefix,
        CursorType::StringType& tag) {
        return el.first == namespacePrefix && el.second == tag;
    }

    ONYX_INLINE bool isStackRoot(StackType& stackElement) {
        return stackElement.first == "" && stackElement.second == root;
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

    ONYX_INLINE bool foundEncoding(CursorType::StringType&& discoveredEncoding,
                                   CursorType& cursor, bool& validateUTF8) {
        EncodingController::ParserAction action =
            ec.foundEncoding(discoveredEncoding, true, true);

        if (action == EncodingController::ParserAction::RESTART) {
            return false;
        }

        validateUTF8 = ec.validate;
        // It is only safe to stop parsing because it is guarantee no events
        // could have been triggered before the XML declaration except the
        // onStart event
        return true;
    }
};

struct StringSaxAutodetectionPolicy
    : BasicAutodetectionParserPolicy<StreamCursor, StreamCursor::StringType,
                                     std::string, std::vector<std::string>> {
    EncodingController& ec;

    using CursorType = StreamCursor;
    using StringType = StreamCursor::StringType;
    using StackType = std::string;
    using Stack = std::vector<StackType>;

    ONYX_INLINE StringType transformText(CursorType::StringType&& text,
                                         TextTransformationMode ttm) {
        if (ttm == TextTransformationMode::UPPERCASE) {
            return text::asciiToUpper(text);
        }

        return std::string(text);
    }

    ONYX_INLINE bool foundEncoding(CursorType::StringType&& discoveredEncoding,
                                   CursorType& cursor, bool& validateUTF8) {
        ec.foundEncoding(discoveredEncoding, true, true);

        return false;
    }
};

struct StreamSaxParserPolicy {
    SaxListener& listener;
    std::vector<std::string> stack;
    std::string_view root = ".empty";
    EncodingController& ec;

    using CursorType = StreamCursor;
    using StringType = StreamCursor::StringType;
    using StackType = std::pair<std::string, std::string>;
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
                                          bool hasEncoding, bool isStandalone,
                                          bool hasStandalone, Stack& stack,
                                          CursorType& cursor) {
        this->listener.onXMLDeclaration(std::move(version), std::move(encoding),
                                        hasEncoding, isStandalone,
                                        hasStandalone);
    }

    ONYX_INLINE void doctypeAction(StringType&& doctypeText, Stack& stack,
                                   CursorType& cursor) {
        this->listener.onDoctype(std::move(doctypeText));
    }

    ONYX_INLINE void openAction(StringType&& namespaceName,
                                StringType&& tagName, bool isSelfClosing,
                                std::vector<StringType>& attributeNames,
                                std::vector<StringType>& attributeValues,
                                Stack& stack, CursorType& cursor) {
        std::vector<Attribute> attributes;
        for (int i = 0; i < attributeNames.size(); i++) {
            attributes.emplace_back(attributeNames[i],
                                    std::move(attributeValues[i]));
        }

        if (!isSelfClosing) {
            stack.push_back({namespaceName, tagName});
        }
        this->listener.onTagOpen(std::move(namespaceName), std::move(tagName),
                                 isSelfClosing, std::move(attributes));
    }

    ONYX_INLINE void closeAction(StringType&& namespaceName,
                                 StringType&& tagName, Stack& stack,
                                 CursorType& cursor) {
        stack.pop_back();
        this->listener.onTagClose(std::move(namespaceName), std::move(tagName));
    }

    ONYX_INLINE void initStack(std::vector<StackType>& stack) {
        stack.push_back({"", std::string(root)});
    }

    ONYX_INLINE bool equalStackElementToTag(
        StackType& el, CursorType::StringType& namespacePrefix,
        CursorType::StringType& tag) {
        return el.first == namespacePrefix && el.second == tag;
    }

    ONYX_INLINE bool isStackRoot(StackType& stackElement) {
        return stackElement.first == "" && stackElement.second == root;
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

    ONYX_INLINE bool foundEncoding(CursorType::StringType&& discoveredEncoding,
                                   CursorType& cursor, bool& validateUTF8) {
        ec.foundEncoding(discoveredEncoding, false, false);

        validateUTF8 = ec.validate;
        return true;
    }
};

void SaxParser::parse(std::string_view input, std::string encoding) {
    EncodingStringState inputState(input);

    SaxListener& listener = this->listener;
    auto parseLocal = [&inputState, &listener,
                       &encoding](EncodingController& ec) {
        using StringType = StringCursor::StringType;
        StringCursor pos(inputState.getInput());

        if (encoding == "autodetect") {
            // Consume the BOM so it isn't perceived as top level text
            text::autodetectXmlEncoding(pos);
        }

        StringSaxParserPolicy policy{
            .listener = listener, .ec = ec, .inputState = inputState};

        if (ec.parseCallCount == 1) {
            listener.onStart();
        }

        try {
            parseBody<ValidatingConfig, StringSaxParserPolicy>(pos, policy,
                                                               ec.validate);
        } catch (std::exception& e) {
            listener.onException(e);
        }

        if (!ec.waitingParseDueToRestart) {
            listener.onEnd();
        }
    };

    auto transcode = [&inputState](EncodingController& ec) -> bool {
        return inputState.transcodeToUTF8(ec.encoding);
    };

    auto autodetectEncoding = [&inputState](EncodingController& ec)
        -> std::pair<std::string, text::XmlEncodingAutodetectionResult> {
        StringCursor pos(inputState.getInput());

        return text::autodetectXmlEncoding(pos);
    };

    auto transcodeAndSpeculativeParse =
        [&inputState](EncodingController& ec) -> void {
        impl::StringViewReadBuffer<char> buf(inputState.getInput());
        std::istream input(&buf);
        StreamCursor pos(input);

        StringSaxAutodetectionPolicy policy{.ec = ec};

        // We are starting from the beginning, so a BOM that must have been
        // consumed may be available. This function will do that.
        text::autodetectXmlEncoding(pos);
        // Needs to be called after possibly skipping the BOM
        ec.validate = !pos.setInputEncoding(ec.encoding);
        skipWhitespace(pos);

        parseBody<EncodingAutodetectionConfig, StringSaxAutodetectionPolicy>(
            pos, policy, ec.validate);
    };

    EncodingController encodingController(parseLocal, transcode,
                                          autodetectEncoding,
                                          transcodeAndSpeculativeParse);
    encodingController.begin(encoding);

    encodingController.triggerParseIfWaiting();
}

void SaxParser::parse(std::istream& input, std::string encoding) {
    using StringType = StreamCursor::StringType;
    StreamCursor pos(input);

    SaxListener& listener = this->listener;
    auto parseLocalBase =
        [&pos, &listener]<typename ParseLocalConfig>(EncodingController& ec) {
            StreamSaxParserPolicy policy{.listener = listener, .ec = ec};

            skipWhitespace(pos);

            listener.onStart();
            try {
                parseBody<ParseLocalConfig, StreamSaxParserPolicy>(pos, policy,
                                                                   ec.validate);
            } catch (std::exception& e) {
                listener.onException(e);
            }

            listener.onEnd();
        };

    auto parseLocal = [&parseLocalBase](EncodingController& ec) {
        parseLocalBase.template operator()<ValidatingConfig>(ec);
    };

    auto transcode = [&pos](EncodingController& ec) -> bool {
        return pos.setInputEncoding(ec.encoding);
    };

    auto autodetectEncoding = [&pos](EncodingController& ec)
        -> std::pair<std::string, text::XmlEncodingAutodetectionResult> {
        return text::autodetectXmlEncoding(pos);
    };

    auto transcodeAndSpeculativeParse =
        [&parseLocalBase](EncodingController& ec) -> void {
        ec.triggerTranscode();
        parseLocalBase.template operator()<EncodingAutodetectionConfig>(ec);
    };

    EncodingController encodingController(parseLocal, transcode,
                                          autodetectEncoding,
                                          transcodeAndSpeculativeParse);
    encodingController.begin(encoding);
}
}  // namespace onyx::dynamic::parser