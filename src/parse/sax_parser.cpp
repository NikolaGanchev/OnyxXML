#include "parse/sax_parser.h"

#include "parse/parser.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"

namespace onyx::dynamic::parser {

SaxParser::SaxParser(SaxListener& listener) : listener(listener) {}

void SaxParser::parse(std::string_view input) {
    struct StringSaxParserPolicy {
        SaxListener& listener;
        std::vector<std::string_view> stack;
        std::vector<std::string_view> attributeNames;
        std::vector<std::pair<std::string_view, bool>> attributeValues;

        using CursorType = StringCursor;
        using StringType = CursorType::StringType;

        ONYX_INLINE void textAction(StringType text, bool hasEntities,
                                    CursorType& cursor) {
            this->listener.onText(hasEntities ? text::expandEntities(text)
                                              : std::string(text));
        }

        ONYX_INLINE void commentAction(StringType commentText,
                                       CursorType& cursor) {
            this->listener.onComment(std::string(commentText));
        }

        ONYX_INLINE void cdataAction(StringType cdataText, CursorType& cursor) {
            this->listener.onCData(std::string(cdataText));
        }

        ONYX_INLINE void instructionAction(StringType tagName,
                                           StringType processingInstruction,
                                           CursorType& cursor) {
            this->listener.onInstruction(std::string(tagName),
                                         std::string(processingInstruction));
        }

        ONYX_INLINE void attributeAction(StringType attributeName,
                                         StringType attributeValue,
                                         bool hasEntities, CursorType& cursor) {
            attributeNames.push_back(attributeName);
            attributeValues.push_back(
                std::make_pair(attributeValue, hasEntities));
        }

        ONYX_INLINE void xmlDeclarationAction(
            StringType version, StringType encoding, bool hasEncoding,
            bool isStandalone, bool hasStandalone, CursorType& cursor) {
            this->listener.onXMLDeclaration(std::string(version),
                                            std::string(encoding), hasEncoding,
                                            isStandalone, hasStandalone);
        }

        ONYX_INLINE void doctypeAction(StringType doctypeText,
                                       CursorType& cursor) {
            this->listener.onDoctype(std::string(doctypeText));
        }

        ONYX_INLINE void openAction(StringType tagName, bool isSelfClosing,
                                    CursorType& cursor) {
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
            attributeNames.clear();
            attributeValues.clear();
            this->listener.onTagOpen(std::string(tagName), isSelfClosing,
                                     std::move(attributes));
        }

        ONYX_INLINE void closeAction(StringType tagName, CursorType& cursor) {
            std::string_view thisNode = stack.back();
            if (thisNode != tagName) {
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

    std::string_view root = ".empty";

    StringSaxParserPolicy policy{this->listener};

    policy.stack.push_back(root);

    skipWhitespace(pos);

    this->listener.onStart();

    try {
        parseBody<true, StringType, StringCursor, StringSaxParserPolicy>(
            pos, policy);
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    if (policy.stack.size() != 1 || policy.stack[0] != root) {
        std::invalid_argument e("Unclosed tags left");
        this->listener.onException(e);
    }

    this->listener.onEnd();
}

void SaxParser::parse(std::istream& input) {
    struct StreamSaxParserPolicy {
        SaxListener& listener;
        std::vector<std::string> stack;
        std::vector<std::string> attributeNames;
        std::vector<std::pair<std::string, bool>> attributeValues;

        using CursorType = StreamCursor;
        using StringType = StreamCursor::StringType;

        ONYX_INLINE void textAction(StringType&& text, bool hasEntities,
                                    CursorType& cursor) {
            this->listener.onText(hasEntities ? text::expandEntities(text)
                                              : std::move(text));
        }

        ONYX_INLINE void commentAction(StringType&& commentText,
                                       CursorType& cursor) {
            this->listener.onComment(std::move(commentText));
        }

        ONYX_INLINE void cdataAction(StringType&& cdataText,
                                     CursorType& cursor) {
            this->listener.onCData(std::move(cdataText));
        }

        ONYX_INLINE void instructionAction(StringType&& tagName,
                                           StringType&& processingInstruction,
                                           CursorType& cursor) {
            this->listener.onInstruction(std::move(tagName),
                                         std::move(processingInstruction));
        }

        ONYX_INLINE void attributeAction(StringType&& attributeName,
                                         StringType&& attributeValue,
                                         bool hasEntities, CursorType& cursor) {
            attributeNames.push_back(std::move(attributeName));
            attributeValues.push_back(
                std::make_pair(std::move(attributeValue), hasEntities));
        }

        ONYX_INLINE void xmlDeclarationAction(
            StringType&& version, StringType&& encoding, bool hasEncoding,
            bool isStandalone, bool hasStandalone, CursorType& cursor) {
            this->listener.onXMLDeclaration(std::move(version),
                                            std::move(encoding), hasEncoding,
                                            isStandalone, hasStandalone);
        }

        ONYX_INLINE void doctypeAction(StringType&& doctypeText,
                                       CursorType& cursor) {
            this->listener.onDoctype(std::move(doctypeText));
        }

        ONYX_INLINE void openAction(StringType&& tagName, bool isSelfClosing,
                                    CursorType& cursor) {
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
            attributeNames.clear();
            attributeValues.clear();
            this->listener.onTagOpen(std::move(tagName), isSelfClosing,
                                     std::move(attributes));
        }

        ONYX_INLINE void closeAction(StringType&& tagName, CursorType& cursor) {
            std::string_view thisNode = stack.back();
            if (thisNode != tagName) {
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

    std::string_view root = ".empty";

    StreamSaxParserPolicy policy{this->listener};

    policy.stack.push_back(std::string(root));

    skipWhitespace(pos);

    this->listener.onStart();
    try {
        parseBody<true, StringType, StreamCursor, StreamSaxParserPolicy>(
            pos, policy);
    } catch (std::exception& e) {
        this->listener.onException(e);
    }

    if (policy.stack.size() != 1 || policy.stack[0] != root) {
        std::invalid_argument e("Unclosed tags left");
        this->listener.onException(e);
    }

    this->listener.onEnd();
}
}  // namespace onyx::dynamic::parser