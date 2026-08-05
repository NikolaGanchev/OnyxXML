#pragma once

#include <stdexcept>
#include <utility>
#include <vector>

#include "helpers.h"
#include "is_cursor.h"
#include "is_parse_policy.h"
#include "is_parser_config.h"

namespace onyx::dynamic::parser {

template <bool validate, typename CursorType>
ONYX_INLINE void readOrThrow(CursorType& pos, std::string_view expected,
                             const char* exceptionString)
    requires(isCursor<CursorType>)
{
    if (validate) {
        if (!pos.consumeIfMatches(expected)) {
            throw std::invalid_argument(exceptionString);
        }
    } else {
        pos.advance(expected.size());
    }
}

template <typename Config, typename Policy>
ONYX_INLINE void parseBody(typename Policy::CursorType& pos, Policy& policy)
    /* In GCC 15.1 and older MSVC versions, having
        `isCursor<CursorType>, isParserPolicy<Policy>`
        with a comma instead of `&&` actually causes an internal compiler error.
        Discovered via trial and error.
    */
    requires(isCursor<typename Policy::CursorType> && isParserPolicy<Policy> &&
             isParserConfig<Config>)
{
    using StringType = typename Policy::StringType;
    using StackType = typename Policy::StackType;
    bool firstTag = true;
    bool foundXmlDeclaration = false;
    bool foundDoctype = false;
    struct EmptyStruct {};
    std::vector<StringType> attributeNames;
    std::vector<std::pair<StringType, bool>> attributeValues;
    std::vector<StackType> stack;
    policy.initStack(stack);
    while (pos.current() != '\0') {
        /* Invariant - always at the start of either a tag or a sequence of
         * text */
        if (pos.current() != '<') {
            pos.beginCapture();
            bool hasEntities = false;
            bool end = false;
            while (pos.captureCurrent() != '<') {
                if (pos.captureCurrent() == '\0') {
                    while (pos.current() != '\0') {
                        if constexpr (Config::validate) {
                            if (!isWhitespace(pos.current())) {
                                throw std::invalid_argument(
                                    "Invalid end after tag open");
                            }
                        }
                        pos.advance();
                    }
                    end = true;
                    break;
                }
                if (pos.captureCurrent() == '&') hasEntities = true;
                pos.captureAdvance();
            }
            if (end) break;
            StringType text = pos.getCaptured();
            pos.bringToCapture();

            policy.textAction(std::move(text), hasEntities, stack, pos);

            continue;
        }

        /* Invariant - always in tag name */
        pos.advance();

        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        if (pos.current() == '?') {
            /* Invariant - at processing directive */
            pos.advance();
            if constexpr (Config::validate) {
                if (pos.current() == '\0') {
                    throw std::invalid_argument("Premature end of document");
                }
            }
            StringType tagName = readName(pos);
            if constexpr (Config::validate) {
                if (tagName.empty()) {
                    throw std::invalid_argument("Invalid tag name");
                }
            }
            pos.advance(tagName.size());
            /* Invariant - after tag name */
            if (tagName.size() == 3 && tolower(tagName[0]) == 'x' &&
                tolower(tagName[1]) == 'm' && tolower(tagName[2]) == 'l') {
                if (!firstTag) {
                    throw std::invalid_argument(
                        "XML declaration is only allowed at the first "
                        "position in the prologue");
                } else {
                    if (foundXmlDeclaration) {
                        throw std::invalid_argument(
                            "Multiple XML declarations found");
                    } else {
                        foundXmlDeclaration = true;
                    }
                    /* Invariant - right after xml tag */
                    bool hasVersion = false;
                    bool hasEncoding = false;
                    bool hasStandalone = false;
                    StringType version, encoding, standalone;

                    /* Walk through all pseudo-attributes in the xml decl */
                    while (pos.current() != '\0' && pos.current() != '?') {
                        /* skip whitespace */
                        skipWhitespace(pos);
                        if (pos.current() == '?') {
                            break;
                        }
                        if (pos.current() == '\0') {
                            throw std::invalid_argument(
                                "Premature end of document");
                        }

                        /* read attribute name */
                        StringType attrName = readName(pos);
                        if constexpr (Config::validate) {
                            if (attrName.empty()) {
                                throw std::invalid_argument(
                                    "Invalid XML declaration attribute name");
                            }
                        }
                        pos.advance(attrName.size());

                        /* expect '=' */
                        skipWhitespace(pos);
                        if constexpr (Config::validate) {
                            if (pos.current() != '=') {
                                throw std::invalid_argument(
                                    "No '=' after XML declaration attribute "
                                    "name");
                            }
                        }
                        pos.advance();
                        if (pos.current() == '\0') {
                            throw std::invalid_argument(
                                "Premature end of document");
                        }

                        skipWhitespace(pos);
                        if constexpr (Config::validate) {
                            if ((pos.current() != '"' &&
                                 pos.current() != '\'')) {
                                throw std::invalid_argument(
                                    "XML declaration attribute value not "
                                    "quoted");
                            }
                        }
                        char quote = pos.current();
                        pos.advance();

                        pos.beginCapture();
                        while (pos.captureCurrent() != '\0' &&
                               pos.captureCurrent() != '?' &&
                               pos.captureCurrent() != quote) {
                            pos.captureAdvance();
                        }
                        if constexpr (Config::validate) {
                            if ((pos.captureCurrent() == '?' ||
                                 pos.captureCurrent() == '\0' ||
                                 pos.captureCurrent() != quote)) {
                                throw std::invalid_argument(
                                    "Unterminated XML declaration attribute "
                                    "value");
                            }
                        }
                        StringType val = pos.getCaptured();
                        pos.bringToCapture();
                        pos.advance(); /* skip closing quote */
                        if (pos.current() == '\0') {
                            throw std::invalid_argument(
                                "Premature end of document");
                        }

                        if (attrName == "version") {
                            if (hasVersion) {
                                throw std::invalid_argument(
                                    "XML Declaration 'version' declared more "
                                    "than once");
                            }
                            version = val;
                            hasVersion = true;
                        } else if (attrName == "encoding") {
                            if (hasEncoding) {
                                throw std::invalid_argument(
                                    "XML Declaration 'encoding' declared more "
                                    "than once");
                            }
                            encoding = val;
                            hasEncoding = true;
                            if (!hasVersion) {
                                throw std::invalid_argument(
                                    "XML Declaration cannot declare 'encoding' "
                                    "before 'version'");
                            }
                            if (hasStandalone) {
                                throw std::invalid_argument(
                                    "XML Declaration cannot declare "
                                    "'standalone' before 'encoding' when "
                                    "'encoding' is present");
                            }
                        } else if (attrName == "standalone") {
                            if (hasStandalone) {
                                throw std::invalid_argument(
                                    "XML Declaration 'standalone' declared "
                                    "more than once");
                            }
                            standalone = val;
                            hasStandalone = true;
                            if (!hasVersion) {
                                throw std::invalid_argument(
                                    "XML Declaration cannot declare "
                                    "'standalone' "
                                    "before 'version'");
                            }
                        } else {
                            if constexpr (Config::validate) {
                                throw std::invalid_argument(
                                    std::string(
                                        "Invalid XML declaration attribute '") +
                                    std::string(attrName) + "'");
                            }
                        }
                    }
                    /* Invariant - pos at ?*/
                    if (pos.peek(1) != '>') {
                        throw std::invalid_argument("Unclosed XML declaration");
                    }
                    pos.advance();
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end of document");
                    }
                    pos.advance();

                    /* enforce presence and legality */
                    if (!hasVersion) {
                        throw std::invalid_argument(
                            "XML declaration must include version");
                    }
                    if constexpr (Config::validate) {
                        if (version != "1.0" && version != "1.1") {
                            throw std::invalid_argument(
                                "Unsupported XML version, must be '1.0' or "
                                "'1.1'");
                        }
                    }
                    /* encoding is optional in 1.0, but if present must match
                     * NMTOKEN */
                    if constexpr (Config::validate) {
                        if (hasEncoding) {
                            /* simple check: no spaces, starts with letter */
                            if (encoding.empty() || !isalpha(encoding[0]))
                                throw std::invalid_argument(
                                    "Invalid encoding in XML declaration");
                        }
                    }
                    /* standalone defaults to "no" if not present */
                    if (!hasStandalone) {
                        standalone = "no";
                    } else {
                        if constexpr (Config::validate) {
                            if (standalone != "yes" && standalone != "no") {
                                throw std::invalid_argument(
                                    "Invalid standalone value, must be 'yes' "
                                    "or "
                                    "'no'");
                            }
                        }
                    }
                    bool isStandalone = (standalone[0] == 'y');
                    if (encoding.size() == 0) encoding = "UTF-8";

                    policy.xmlDeclarationAction(
                        std::move(version), std::move(encoding), hasEncoding,
                        isStandalone, hasStandalone, stack, pos);
                    continue;
                }
            }
            if constexpr (Config::validate) {
                if ((pos.current() != ' ' || pos.peek(1) == '\0')) {
                    throw std::invalid_argument(
                        "No space between processing instruction target and "
                        "processing instruction content");
                }
            }
            pos.advance();

            pos.beginCapture();

            while (!(pos.captureCurrent() == '?' &&
                     pos.capturePeek(1) != '\0' && pos.capturePeek(1) == '>')) {
                pos.captureAdvance();
                if constexpr (Config::validate) {
                    if (pos.captureCurrent() == '\0')
                        throw std::invalid_argument(
                            "Invalid processing instruction without ending");
                }
            }

            StringType processingInstruction = pos.getCaptured();

            pos.bringToCapture();
            pos.advance(2);

            /* Invariant - just after processing instruction end */
            policy.instructionAction(std::move(tagName),
                                     std::move(processingInstruction), stack,
                                     pos);
            firstTag = false;
            continue;
        } else if (pos.current() == '!') {
            pos.advance();
            if constexpr (Config::validate) {
                if (pos.current() == '\0') {
                    throw std::invalid_argument("Premature end of <! tag");
                }
            }

            if (pos.current() == '-') {
                pos.advance();
                if constexpr (Config::validate) {
                    if (pos.current() != '-') {
                        throw std::invalid_argument("Premature end of comment");
                    }
                }
                pos.advance();
                /* Invariant - right after <!-- of comment */
                if constexpr (Config::validate) {
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end of document");
                    }
                }
                pos.beginCapture();

                while (!(pos.captureCurrent() == '-' &&
                         pos.capturePeek(1) != '\0' &&
                         pos.capturePeek(1) == '-')) {
                    pos.captureAdvance();
                    if constexpr (Config::validate) {
                        if (pos.captureCurrent() == '\0') {
                            throw std::invalid_argument(
                                "Invalid comment without ending");
                        }
                    }
                }

                StringType commentText = pos.getCaptured();

                pos.captureAdvance(2);

                /* Invariant - either at error state or at > */
                if constexpr (Config::validate) {
                    if (pos.captureCurrent() != '>') {
                        throw std::invalid_argument(
                            "-- inside of comment not allowed");
                    }
                }
                pos.bringToCapture();
                pos.advance();
                policy.commentAction(std::move(commentText), stack, pos);
                firstTag = false;
                continue;
            } else if (pos.current() == '[') {
                pos.advance();
                readOrThrow<Config::validate>(pos, "CDATA[",
                                              "Premature end of CDATA section");
                /* Invariant - right after <![CDATA[ */
                if constexpr (Config::validate) {
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end of document");
                    }
                }
                pos.beginCapture();

                while (!(
                    pos.captureCurrent() == ']' && pos.capturePeek(1) != '\0' &&
                    pos.capturePeek(1) == ']' && pos.capturePeek(2) != '\0' &&
                    pos.capturePeek(2) == '>')) {
                    pos.captureAdvance();
                    if constexpr (Config::validate) {
                        if (pos.captureCurrent() == '\0')
                            throw std::invalid_argument(
                                "Invalid CDATA without ending");
                    }
                }

                StringType cdataText = pos.getCaptured();

                pos.bringToCapture();
                pos.advance(3);
                policy.cdataAction(std::move(cdataText), stack, pos);
                firstTag = false;
                continue;
            } else if (pos.current() == 'D') {
                pos.advance();

                readOrThrow<Config::validate>(
                    pos, "OCTYPE ", "Premature end of DOCTYPE section");

                if (foundDoctype) {
                    throw std::invalid_argument(
                        "Multiple Document Type Declarations found");
                } else {
                    foundDoctype = true;
                }

                if (!firstTag) {
                    throw std::invalid_argument(
                        "Document Type Declaration is only allowed before all "
                        "XML elements except the XML declaration");
                }

                pos.beginCapture();

                while (pos.captureCurrent() != '>') {
                    pos.captureAdvance();
                    if constexpr (Config::validate) {
                        if (pos.captureCurrent() == '\0') {
                            throw std::invalid_argument(
                                "Invalid DOCTYPE without ending");
                        }
                    }
                }

                StringType doctypeText = pos.getCaptured();

                pos.bringToCapture();
                pos.advance();
                policy.doctypeAction(std::move(doctypeText), stack, pos);
                firstTag = false;
                continue;
            } else {
                if constexpr (Config::validate) {
                    throw std::invalid_argument("Tag name cannot contain '!'");
                }
            }
        }

        bool isClosing = (pos.current() == '/');

        if (isClosing) pos.advance();
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        /* Invariant - pos always at tag name start */
        StringType tagName = readName(pos);
        if constexpr (Config::validate) {
            if (tagName.empty()) {
                throw std::invalid_argument("Invalid tag name");
            }
        }
        pos.advance(tagName.size());

        bool couldHaveAttributes = isWhitespace(pos.current()) && !isClosing;

        /* Invariant - pos always after tag name end */
        skipWhitespace(pos);
        if constexpr (Config::validate) {
            if (pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
        }

        if (couldHaveAttributes) {
            /* Invariant - either at start of attribute or at > */
            while (pos.current() != '>' && pos.current() != '/') {
                StringType attributeName = readName(pos);
                if (attributeName.empty()) {
                    throw std::invalid_argument("Invalid non-closing tag");
                }
                pos.advance(attributeName.size());

                /* Invariant - after attribute name */
                skipWhitespace(pos);

                /* Invariant - at = or error state */
                if constexpr (Config::validate) {
                    if ((pos.current() == '\0' || pos.current() != '=')) {
                        throw std::invalid_argument("No = after attribute");
                    }
                }
                pos.advance();
                /* Invariant - after attribute = */
                skipWhitespace(pos);
                char attributeQuote = '\0';
                if constexpr (Config::validate) {
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end at attribute");
                    }
                }

                if (pos.current() == '\'' || pos.current() == '\"') {
                    attributeQuote = pos.current();
                } else {
                    if constexpr (Config::validate) {
                        throw std::invalid_argument(
                            "No quote (\" or \') after attribute =");
                    }
                }
                pos.advance();

                /* Invariant - after attribute value opening quote */
                pos.beginCapture();
                bool hasEntities = false;
                while (pos.captureCurrent() != attributeQuote) {
                    if constexpr (Config::validate) {
                        if (pos.captureCurrent() == '\0') {
                            throw std::invalid_argument(
                                "Improperly closed attribute value");
                        }
                    }
                    if (pos.captureCurrent() == '&') hasEntities = true;
                    pos.captureAdvance();
                }
                StringType attributeValue = pos.getCaptured();
                pos.bringToCapture();

                /* Invariant - pos is at closing quote */
                pos.advance();

                /* Invariant - pos is just after closing quote */
                /* Allows non-standard <tagName name="value"/> where the
                 * backslash is exactly after the quote */
                if constexpr (Config::validate) {
                    if (!isWhitespace(pos.current()) && pos.current() != '>' &&
                        pos.current() != '/') {
                        throw std::invalid_argument(
                            "No whitespace after attribute closing quote");
                    }
                }

                if constexpr (Config::validate &&
                              Config::validateDuplicateAttributes) {
                    for (size_t i = 0; i < attributeNames.size(); i++) {
                        if (attributeNames[i] == attributeName) {
                            throw std::invalid_argument(
                                "Duplicate attribute name");
                        }
                    }
                }

                if constexpr (Config::validate) {
                    if (attributeNames.size() >= Config::maxAttributeCount) {
                        throw std::invalid_argument(
                            "Tag has too many attributes");
                    }
                }
                attributeNames.push_back(std::move(attributeName));
                attributeValues.push_back(
                    std::make_pair(std::move(attributeValue), hasEntities));

                /* Continues to either >, /> or another attribute */
                skipWhitespace(pos);
                if constexpr (Config::validate) {
                    if (pos.current() == '\0') {
                        throw std::invalid_argument(
                            "Premature end after attribute");
                    }
                }
            }
        }

        bool isSelfClosing = false;
        /* Invariant - pos is always at / or > */
        if (pos.current() == '>') {
            pos.advance();
        } else if (pos.current() == '/') {
            if constexpr (Config::validate) {
                if (isClosing) {
                    throw std::invalid_argument(
                        "Trying to double-close closing tag");
                }
            }
            pos.advance();
            if constexpr (Config::validate) {
                if (pos.current() != '>') {
                    throw std::invalid_argument(
                        "Invalid tag close - must have > after /");
                }
            }
            isSelfClosing = true;
            pos.advance();
        } else {
            if constexpr (Config::validate) {
                throw std::invalid_argument("No tag close for tag " +
                                            std::string(tagName));
            }
        }

        /* Invariant - pos always after valid tag close */
        if (!isClosing) {
            /* Invariant - top stack node is always current parent */
            firstTag = false;
            policy.openAction(std::move(tagName), isSelfClosing, attributeNames,
                              attributeValues, stack, pos);
            attributeNames.resize(0);
            attributeValues.resize(0);
        } else {
            /* Invariant - when closing node, the current parent (stack top)
             * must be of the node type */
            if (!policy.equalStackElementToTag(stack.back(), tagName)) {
                throw std::invalid_argument("Closing unopened tag");
            }
            if (stack.size() == 1) {
                throw std::invalid_argument("Closing non-existent tags");
            }
            policy.closeAction(std::move(tagName), stack, pos);
        }
    }

    if (stack.size() != 1 || !policy.isStackRoot(stack[0])) {
        throw std::invalid_argument("Unclosed tags left");
    }
}
}  // namespace onyx::dynamic::parser