#pragma once

#include <utility>
#include <vector>

#include "helpers.h"
#include "is_cursor.h"
#include "is_parse_policy.h"
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

template <bool validate, typename CursorType>
ONYX_INLINE void incrementPosIfEqualsOrThrow(CursorType& pos, char character,
                                             const char* exceptionString)
    requires(isCursor<CursorType>)
{
    if (validate && pos.current() != character) {
        throw std::invalid_argument(exceptionString);
    }
    pos.advance();
}

template <bool validate, typename StringType, typename CursorType,
          typename Policy>
ONYX_INLINE void parseBody(CursorType& pos, Policy& policy)
    /* In GCC 15.1 and older MSVC versions, having
        `isCursor<CursorType>, isParserPolicy<Policy>`
        with a comma instead of `&&` actually causes an internal compiler error.
        Discovered via trial and error.
    */
    requires(isCursor<CursorType> && isParserPolicy<Policy>)
{
    bool firstTag = true;
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
                        if (validate && !isWhitespace(pos.current()))
                            throw std::invalid_argument(
                                "Invalid end after tag open");
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

            policy.textAction(std::move(text), hasEntities, pos);

            continue;
        }

        /* Invariant - always in tag name */
        pos.advance();

        if (validate && pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
        }

        if (pos.current() == '?') {
            /* Invariant - at processing directive */
            pos.advance();
            if (validate && pos.current() == '\0') {
                throw std::invalid_argument("Premature end of document");
            }
            StringType tagName = readName(pos);
            if (validate && tagName.empty()) {
                throw std::invalid_argument("Invalid tag name");
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
                        if (validate && attrName.empty()) {
                            throw std::invalid_argument(
                                "Invalid XML declaration attribute name");
                        }
                        pos.advance(attrName.size());

                        /* expect '=' */
                        skipWhitespace(pos);
                        if (validate && pos.current() != '=') {
                            throw std::invalid_argument(
                                "No '=' after XML declaration attribute "
                                "name");
                        }
                        pos.advance();
                        if (pos.current() == '\0') {
                            throw std::invalid_argument(
                                "Premature end of document");
                        }

                        skipWhitespace(pos);
                        if (validate &&
                            (pos.current() != '"' && pos.current() != '\'')) {
                            throw std::invalid_argument(
                                "XML declaration attribute value not quoted");
                        }
                        char quote = pos.current();
                        pos.advance();

                        pos.beginCapture();
                        while (pos.captureCurrent() != '\0' &&
                               pos.captureCurrent() != '?' &&
                               pos.captureCurrent() != quote) {
                            pos.captureAdvance();
                        }
                        if (validate && (pos.captureCurrent() == '?' ||
                                         pos.captureCurrent() == '\0' ||
                                         pos.captureCurrent() != quote)) {
                            throw std::invalid_argument(
                                "Unterminated XML declaration attribute "
                                "value");
                        }
                        StringType val = pos.getCaptured();
                        pos.bringToCapture();
                        pos.advance(); /* skip closing quote */
                        if (pos.current() == '\0') {
                            throw std::invalid_argument(
                                "Premature end of document");
                        }

                        if (attrName == "version") {
                            version = val;
                            hasVersion = true;
                        } else if (attrName == "encoding") {
                            encoding = val;
                            hasEncoding = true;
                        } else if (attrName == "standalone") {
                            standalone = val;
                            hasStandalone = true;
                        } else if (validate) {
                            throw std::invalid_argument(
                                std::string(
                                    "Invalid XML declaration attribute '") +
                                std::string(attrName) + "'");
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
                    if (validate && version != "1.0" && version != "1.1") {
                        throw std::invalid_argument(
                            "Unsupported XML version, must be '1.0' or "
                            "'1.1'");
                    }
                    /* encoding is optional in 1.0, but if present must match
                     * NMTOKEN */
                    if (hasEncoding && validate) {
                        /* simple check: no spaces, starts with letter */
                        if (encoding.empty() || !isalpha(encoding[0]))
                            throw std::invalid_argument(
                                "Invalid encoding in XML declaration");
                    }
                    /* standalone defaults to "no" if not present */
                    if (!hasStandalone) {
                        standalone = "no";
                    } else if (validate && standalone != "yes" &&
                               standalone != "no") {
                        throw std::invalid_argument(
                            "Invalid standalone value, must be 'yes' or "
                            "'no'");
                    }
                    bool isStandalone = (standalone[0] == 'y');
                    if (encoding.size() == 0) encoding = "UTF-8";

                    policy.xmlDeclarationAction(
                        std::move(version), std::move(encoding), hasEncoding,
                        isStandalone, hasStandalone, pos);
                    continue;
                }
            }
            if (validate && (pos.current() != ' ' || pos.peek(1) == '\0')) {
                throw std::invalid_argument(
                    "No space between processing instruction target and "
                    "processing instruction content");
            }
            pos.advance();

            pos.beginCapture();

            while (!(pos.captureCurrent() == '?' &&
                     pos.capturePeek(1) != '\0' && pos.capturePeek(1) == '>')) {
                pos.captureAdvance();
                if (validate && pos.captureCurrent() == '\0')
                    throw std::invalid_argument(
                        "Invalid processing instruction without ending");
            }

            StringType processingInstruction = pos.getCaptured();

            pos.bringToCapture();
            pos.advance(2);

            /* Invariant - just after processing instruction end */
            policy.instructionAction(std::move(tagName),
                                     std::move(processingInstruction), pos);
            firstTag = false;
            continue;
        } else if (pos.current() == '!') {
            pos.advance();
            if (validate && pos.current() == '\0') {
                throw std::invalid_argument("Premature end of <! tag");
            }

            if (pos.current() == '-') {
                pos.advance();
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, '-', "Premature end of comment");
                /* Invariant - right after <!-- of comment */
                if (validate && pos.current() == '\0') {
                    throw std::invalid_argument("Premature end of document");
                }
                pos.beginCapture();

                while (!(pos.captureCurrent() == '-' &&
                         pos.capturePeek(1) != '\0' &&
                         pos.capturePeek(1) == '-')) {
                    pos.captureAdvance();
                    if (validate && pos.captureCurrent() == '\0')
                        throw std::invalid_argument(
                            "Invalid comment without ending");
                }

                StringType commentText = pos.getCaptured();

                pos.captureAdvance(2);

                /* Invariant - either at error state or at > */
                if (validate && pos.captureCurrent() != '>')
                    throw std::invalid_argument(
                        "-- inside of comment not allowed");
                pos.bringToCapture();
                pos.advance();
                policy.commentAction(std::move(commentText), pos);
                firstTag = false;
                continue;
            } else if (pos.current() == '[') {
                pos.advance();
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'C', "Premature end of CDATA section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'D', "Premature end of CDATA section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'A', "Premature end of CDATA section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'T', "Premature end of CDATA section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'A', "Premature end of CDATA section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, '[', "Premature end of CDATA section");
                /* Invariant - right after <![CDATA[ */
                if (validate && pos.current() == '\0') {
                    throw std::invalid_argument("Premature end of document");
                }
                pos.beginCapture();

                while (!(
                    pos.captureCurrent() == ']' && pos.capturePeek(1) != '\0' &&
                    pos.capturePeek(1) == ']' && pos.capturePeek(2) != '\0' &&
                    pos.capturePeek(2) == '>')) {
                    pos.captureAdvance();
                    if (validate && pos.captureCurrent() == '\0')
                        throw std::invalid_argument(
                            "Invalid CDATA without ending");
                }

                StringType cdataText = pos.getCaptured();

                pos.bringToCapture();
                pos.advance(3);
                policy.cdataAction(std::move(cdataText), pos);
                firstTag = false;
                continue;
            } else if (pos.current() == 'D') {
                pos.advance();
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'O', "Premature end of DOCTYPE section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'C', "Premature end of DOCTYPE section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'T', "Premature end of DOCTYPE section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'Y', "Premature end of DOCTYPE section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'P', "Premature end of DOCTYPE section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, 'E', "Premature end of DOCTYPE section");
                incrementPosIfEqualsOrThrow<validate, CursorType>(
                    pos, ' ', "Premature end of DOCTYPE section");

                pos.beginCapture();

                while (pos.captureCurrent() != '>') {
                    pos.captureAdvance();
                    if (validate && pos.captureCurrent() == '\0')
                        throw std::invalid_argument(
                            "Invalid DOCTYPE without ending");
                }

                StringType doctypeText = pos.getCaptured();

                pos.bringToCapture();
                pos.advance();
                policy.doctypeAction(std::move(doctypeText), pos);
                firstTag = false;
                continue;
            } else if (validate) {
                throw std::invalid_argument("Tag name cannot contain '!'");
            }
        }

        bool isClosing = (pos.current() == '/');

        if (isClosing) pos.advance();

        if (validate && pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
        }

        /* Invariant - pos always at tag name start */
        StringType tagName = readName(pos);
        if (validate && tagName.empty()) {
            throw std::invalid_argument("Invalid tag name");
        }
        pos.advance(tagName.size());

        bool couldHaveAttributes = isWhitespace(pos.current()) && !isClosing;

        /* Invariant - pos always after tag name end */
        skipWhitespace(pos);

        if (validate && pos.current() == '\0') {
            throw std::invalid_argument("Premature end of document");
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
                if (validate &&
                    (pos.current() == '\0' || pos.current() != '=')) {
                    throw std::invalid_argument("No = after attribute");
                }
                pos.advance();
                /* Invariant - after attribute = */
                skipWhitespace(pos);
                char attributeQuote = '\0';
                if (validate && pos.current() == '\0') {
                    throw std::invalid_argument("Premature end at attribute");
                }

                if (pos.current() == '\'' || pos.current() == '\"') {
                    attributeQuote = pos.current();
                } else if (validate) {
                    throw std::invalid_argument(
                        "No quote (\" or \') after attribute =");
                }
                pos.advance();

                /* Invariant - after attribute value opening quote */
                pos.beginCapture();
                bool hasEntities = false;
                while (pos.captureCurrent() != attributeQuote) {
                    if (validate && pos.captureCurrent() == '\0')
                        throw std::invalid_argument(
                            "Improperly closed attribute value");
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
                if (validate && !isWhitespace(pos.current()) &&
                    pos.current() != '>' && pos.current() != '/') {
                    throw std::invalid_argument(
                        "No whitespace after attribute closing quote");
                }

                policy.attributeAction(std::move(attributeName),
                                       std::move(attributeValue), hasEntities,
                                       pos);

                /* Continues to either >, /> or another attribute */
                skipWhitespace(pos);
                if (validate && pos.current() == '\0') {
                    throw std::invalid_argument(
                        "Premature end after attribute");
                }
            }
        }

        bool isSelfClosing = false;
        /* Invariant - pos is always at / or > */
        if (pos.current() == '>') {
            pos.advance();
        } else if (pos.current() == '/') {
            if (validate && isClosing) {
                throw std::invalid_argument(
                    "Trying to double-close closing tag");
            }
            pos.advance();
            if (validate && pos.current() != '>') {
                throw std::invalid_argument(
                    "Invalid tag close - must have > after /");
            }
            isSelfClosing = true;
            pos.advance();
        } else if (validate) {
            throw std::invalid_argument("No tag close for tag " +
                                        std::string(tagName));
        }

        /* Invariant - pos always after valid tag close */
        if (!isClosing) {
            /* Invariant - top stack node is always current parent */
            firstTag = false;
            policy.openAction(std::move(tagName), isSelfClosing, pos);
        } else {
            /* Invariant - when closing node, the current parent (stack top)
             * must be of the node type */
            policy.closeAction(std::move(tagName), pos);
        }
    }
}
}  // namespace onyx::dynamic::parser