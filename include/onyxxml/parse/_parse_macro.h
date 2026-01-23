#pragma once

#include <utility>
#include <vector>

#include "helpers.h"
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

#define INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, character, \
                                         exceptionString)          \
    if (validate && *pos != character) {                           \
        throw std::invalid_argument(exceptionString);              \
    }                                                              \
    pos++;

#define PARSE_BODY(validate)                                                   \
    bool firstTag = true;                                                      \
    while (*pos != '\0') {                                                     \
        /* Invariant - always at the start of either a tag or a sequence of    \
         * text */                                                             \
        if (*pos != '<') {                                                     \
            pos.beginCapture();                                                \
            bool hasEntities = false;                                          \
            bool end = false;                                                  \
            while (&pos != '<') {                                              \
                if (&pos == '\0') {                                            \
                    while (*pos != '\0') {                                     \
                        if (validate && !isWhitespace(*pos))                   \
                            throw std::invalid_argument(                       \
                                "Invalid end after tag open");                 \
                        pos++;                                                 \
                    }                                                          \
                    end = true;                                                \
                    break;                                                     \
                }                                                              \
                if (&pos == '&') hasEntities = true;                           \
                ++pos;                                                         \
            }                                                                  \
            if (end) break;                                                    \
            StringType text = pos.getCaptured();                               \
            pos.bringToCapture();                                              \
                                                                               \
            TEXT_ACTION(text, hasEntities, pos);                               \
                                                                               \
            continue;                                                          \
        }                                                                      \
                                                                               \
        /* Invariant - always in tag name */                                   \
        pos++;                                                                 \
                                                                               \
        if (validate && *pos == '\0') {                                        \
            throw std::invalid_argument("Premature end of document");          \
        }                                                                      \
                                                                               \
        if (*pos == '?') {                                                     \
            /* Invariant - at processing directive */                          \
            pos++;                                                             \
            if (validate && *pos == '\0') {                                    \
                throw std::invalid_argument("Premature end of document");      \
            }                                                                  \
            StringType tagName = readName(pos);                                \
            if (validate && tagName.empty()) {                                 \
                throw std::invalid_argument("Invalid tag name");               \
            }                                                                  \
            pos.advance(tagName.size());                                       \
            /* Invariant - after tag name */                                   \
            if (tagName.size() == 3 && tolower(tagName[0]) == 'x' &&           \
                tolower(tagName[1]) == 'm' && tolower(tagName[2]) == 'l') {    \
                if (!firstTag) {                                               \
                    throw std::invalid_argument(                               \
                        "XML declaration is only allowed at the first "        \
                        "position in the prologue");                           \
                } else {                                                       \
                    /* Invariant - right after xml tag */                      \
                    bool hasVersion = false;                                   \
                    bool hasEncoding = false;                                  \
                    bool hasStandalone = false;                                \
                    StringType version, encoding, standalone;                  \
                                                                               \
                    /* Walk through all pseudo-attributes in the xml decl */   \
                    while (*pos != '\0' && *pos != '?') {                      \
                        /* skip whitespace */                                  \
                        skipWhitespace(pos);                                   \
                        if (*pos == '?') {                                     \
                            break;                                             \
                        }                                                      \
                        if (*pos == '\0') {                                    \
                            throw std::invalid_argument(                       \
                                "Premature end of document");                  \
                        }                                                      \
                                                                               \
                        /* read attribute name */                              \
                        StringType attrName = readName(pos);                   \
                        if (validate && attrName.empty()) {                    \
                            throw std::invalid_argument(                       \
                                "Invalid XML declaration attribute name");     \
                        }                                                      \
                        pos.advance(attrName.size());                          \
                                                                               \
                        /* expect '=' */                                       \
                        skipWhitespace(pos);                                   \
                        if (validate && *pos != '=') {                         \
                            throw std::invalid_argument(                       \
                                "No '=' after XML declaration attribute "      \
                                "name");                                       \
                        }                                                      \
                        pos++;                                                 \
                        if (*pos == '\0') {                                    \
                            throw std::invalid_argument(                       \
                                "Premature end of document");                  \
                        }                                                      \
                                                                               \
                        skipWhitespace(pos);                                   \
                        if (validate && (*pos != '"' && *pos != '\'')) {       \
                            throw std::invalid_argument(                       \
                                "XML declaration attribute value not quoted"); \
                        }                                                      \
                        char quote = *pos;                                     \
                        pos++;                                                 \
                                                                               \
                        pos.beginCapture();                                    \
                        while (&pos != '\0' && &pos != '?' && &pos != quote) { \
                            ++pos;                                             \
                        }                                                      \
                        if (validate &&                                        \
                            (&pos == '?' || &pos == '\0' || &pos != quote)) {  \
                            throw std::invalid_argument(                       \
                                "Unterminated XML declaration attribute "      \
                                "value");                                      \
                        }                                                      \
                        StringType val = pos.getCaptured();                    \
                        pos.bringToCapture();                                  \
                        pos++; /* skip closing quote */                        \
                        if (*pos == '\0') {                                    \
                            throw std::invalid_argument(                       \
                                "Premature end of document");                  \
                        }                                                      \
                                                                               \
                        if (attrName == "version") {                           \
                            version = val;                                     \
                            hasVersion = true;                                 \
                        } else if (attrName == "encoding") {                   \
                            encoding = val;                                    \
                            hasEncoding = true;                                \
                        } else if (attrName == "standalone") {                 \
                            standalone = val;                                  \
                            hasStandalone = true;                              \
                        } else if (validate) {                                 \
                            throw std::invalid_argument(                       \
                                std::string(                                   \
                                    "Invalid XML declaration attribute '") +   \
                                std::string(attrName) + "'");                  \
                        }                                                      \
                    }                                                          \
                    /* Invariant - pos at ?*/                                  \
                    if (pos.peek(1) != '>') {                                  \
                        throw std::invalid_argument(                           \
                            "Unclosed XML declaration");                       \
                    }                                                          \
                    pos++;                                                     \
                    if (*pos == '\0') {                                        \
                        throw std::invalid_argument(                           \
                            "Premature end of document");                      \
                    }                                                          \
                    pos++;                                                     \
                                                                               \
                    /* enforce presence and legality */                        \
                    if (!hasVersion) {                                         \
                        throw std::invalid_argument(                           \
                            "XML declaration must include version");           \
                    }                                                          \
                    if (validate && version != "1.0" && version != "1.1") {    \
                        throw std::invalid_argument(                           \
                            "Unsupported XML version, must be '1.0' or "       \
                            "'1.1'");                                          \
                    }                                                          \
                    /* encoding is optional in 1.0, but if present must match  \
                     * NMTOKEN */                                              \
                    if (hasEncoding && validate) {                             \
                        /* simple check: no spaces, starts with letter */      \
                        if (encoding.empty() || !isalpha(encoding[0]))         \
                            throw std::invalid_argument(                       \
                                "Invalid encoding in XML declaration");        \
                    }                                                          \
                    /* standalone defaults to "no" if not present */           \
                    if (!hasStandalone) {                                      \
                        standalone = "no";                                     \
                    } else if (validate && standalone != "yes" &&              \
                               standalone != "no") {                           \
                        throw std::invalid_argument(                           \
                            "Invalid standalone value, must be 'yes' or "      \
                            "'no'");                                           \
                    }                                                          \
                    bool isStandalone = (standalone[0] == 'y');                \
                    if (encoding.size() == 0) encoding = "UTF-8";              \
                                                                               \
                    XML_DECLARATION_ACTION(version, encoding, hasEncoding,     \
                                           isStandalone, hasStandalone, pos);  \
                    continue;                                                  \
                }                                                              \
            }                                                                  \
            if (validate && (*pos != ' ' || pos.peek(1) == '\0')) {            \
                throw std::invalid_argument(                                   \
                    "No space between processing instruction target and "      \
                    "processing instruction content");                         \
            }                                                                  \
            pos++;                                                             \
                                                                               \
            pos.beginCapture();                                                \
                                                                               \
            while (!(&pos == '?' && pos.capturePeek(1) != '\0' &&              \
                     pos.capturePeek(1) == '>')) {                             \
                ++pos;                                                         \
                if (validate && &pos == '\0')                                  \
                    throw std::invalid_argument(                               \
                        "Invalid processing instruction without ending");      \
            }                                                                  \
                                                                               \
            StringType processingInstruction = pos.getCaptured();              \
                                                                               \
            pos.bringToCapture();                                              \
            pos.advance(2);                                                    \
                                                                               \
            /* Invariant - just after processing instruction end */            \
            INSTRUCTION_ACTION(tagName, processingInstruction, pos);           \
            firstTag = false;                                                  \
            continue;                                                          \
        } else if (*pos == '!') {                                              \
            pos++;                                                             \
            if (validate && *pos == '\0') {                                    \
                throw std::invalid_argument("Premature end of <! tag");        \
            }                                                                  \
                                                                               \
            if (*pos == '-') {                                                 \
                pos++;                                                         \
                INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, '-',           \
                                                 "Premature end of comment");  \
                /* Invariant - right after <!-- of comment */                  \
                if (validate && *pos == '\0') {                                \
                    throw std::invalid_argument("Premature end of document");  \
                }                                                              \
                pos.beginCapture();                                            \
                                                                               \
                while (!(&pos == '-' && pos.capturePeek(1) != '\0' &&          \
                         pos.capturePeek(1) == '-')) {                         \
                    ++pos;                                                     \
                    if (validate && &pos == '\0')                              \
                        throw std::invalid_argument(                           \
                            "Invalid comment without ending");                 \
                }                                                              \
                                                                               \
                StringType commentText = pos.getCaptured();                    \
                                                                               \
                pos.captureAdvance(2);                                         \
                                                                               \
                /* Invariant - either at error state or at > */                \
                if (validate && &pos != '>')                                   \
                    throw std::invalid_argument(                               \
                        "-- inside of comment not allowed");                   \
                pos.bringToCapture();                                          \
                pos++;                                                         \
                COMMENT_ACTION(commentText, pos);                              \
                firstTag = false;                                              \
                continue;                                                      \
            } else if (*pos == '[') {                                          \
                pos++;                                                         \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'C', "Premature end of CDATA section");     \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'D', "Premature end of CDATA section");     \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'A', "Premature end of CDATA section");     \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'T', "Premature end of CDATA section");     \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'A', "Premature end of CDATA section");     \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, '[', "Premature end of CDATA section");     \
                /* Invariant - right after <![CDATA[ */                        \
                if (validate && *pos == '\0') {                                \
                    throw std::invalid_argument("Premature end of document");  \
                }                                                              \
                pos.beginCapture();                                            \
                                                                               \
                while (!(&pos == ']' && pos.capturePeek(1) != '\0' &&          \
                         pos.capturePeek(1) == ']' &&                          \
                         pos.capturePeek(2) != '\0' &&                         \
                         pos.capturePeek(2) == '>')) {                         \
                    ++pos;                                                     \
                    if (validate && &pos == '\0')                              \
                        throw std::invalid_argument(                           \
                            "Invalid CDATA without ending");                   \
                }                                                              \
                                                                               \
                StringType cdataText = pos.getCaptured();                      \
                                                                               \
                pos.bringToCapture();                                          \
                pos.advance(3);                                                \
                CDATA_ACTION(cdataText, pos);                                  \
                firstTag = false;                                              \
                continue;                                                      \
            } else if (*pos == 'D') {                                          \
                pos++;                                                         \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'O', "Premature end of DOCTYPE section");   \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'C', "Premature end of DOCTYPE section");   \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'T', "Premature end of DOCTYPE section");   \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'Y', "Premature end of DOCTYPE section");   \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'P', "Premature end of DOCTYPE section");   \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, 'E', "Premature end of DOCTYPE section");   \
                INCREMENT_POS_IF_EQUALS_OR_THROW(                              \
                    validate, pos, ' ', "Premature end of DOCTYPE section");   \
                                                                               \
                pos.beginCapture();                                            \
                                                                               \
                while (&pos != '>') {                                          \
                    ++pos;                                                     \
                    if (validate && &pos == '\0')                              \
                        throw std::invalid_argument(                           \
                            "Invalid DOCTYPE without ending");                 \
                }                                                              \
                                                                               \
                StringType doctypeText = pos.getCaptured();                    \
                                                                               \
                pos.bringToCapture();                                          \
                pos++;                                                         \
                DOCTYPE_ACTION(doctypeText, pos);                              \
                firstTag = false;                                              \
                continue;                                                      \
            } else if (validate) {                                             \
                throw std::invalid_argument("Tag name cannot contain '!'");    \
            }                                                                  \
        }                                                                      \
                                                                               \
        bool isClosing = (*pos == '/');                                        \
                                                                               \
        if (isClosing) pos++;                                                  \
                                                                               \
        if (validate && *pos == '\0') {                                        \
            throw std::invalid_argument("Premature end of document");          \
        }                                                                      \
                                                                               \
        /* Invariant - pos always at tag name start */                         \
        StringType tagName = readName(pos);                                    \
        if (validate && tagName.empty()) {                                     \
            throw std::invalid_argument("Invalid tag name");                   \
        }                                                                      \
        pos.advance(tagName.size());                                           \
                                                                               \
        bool couldHaveAttributes = isWhitespace(*pos) && !isClosing;           \
                                                                               \
        /* Invariant - pos always after tag name end */                        \
        skipWhitespace(pos);                                                   \
                                                                               \
        if (validate && *pos == '\0') {                                        \
            throw std::invalid_argument("Premature end of document");          \
        }                                                                      \
                                                                               \
        if (couldHaveAttributes) {                                             \
            /* Invariant - either at start of attribute or at > */             \
            while (*pos != '>' && *pos != '/') {                               \
                StringType attributeName = readName(pos);                      \
                if (attributeName.empty()) {                                   \
                    throw std::invalid_argument("Invalid non-closing tag");    \
                }                                                              \
                pos.advance(attributeName.size());                             \
                                                                               \
                /* Invariant - after attribute name */                         \
                skipWhitespace(pos);                                           \
                                                                               \
                /* Invariant - at = or error state */                          \
                if (validate && (*pos == '\0' || *pos != '=')) {               \
                    throw std::invalid_argument("No = after attribute");       \
                }                                                              \
                pos++;                                                         \
                /* Invariant - after attribute = */                            \
                skipWhitespace(pos);                                           \
                char attributeQuote = '\0';                                    \
                if (validate && *pos == '\0') {                                \
                    throw std::invalid_argument("Premature end at attribute"); \
                }                                                              \
                                                                               \
                if (*pos == '\'' || *pos == '\"') {                            \
                    attributeQuote = *pos;                                     \
                } else if (validate) {                                         \
                    throw std::invalid_argument(                               \
                        "No quote (\" or \') after attribute =");              \
                }                                                              \
                pos++;                                                         \
                                                                               \
                /* Invariant - after attribute value opening quote */          \
                pos.beginCapture();                                            \
                bool hasEntities = false;                                      \
                while (&pos != attributeQuote) {                               \
                    if (validate && &pos == '\0')                              \
                        throw std::invalid_argument(                           \
                            "Improperly closed attribute value");              \
                    if (&pos == '&') hasEntities = true;                       \
                    ++pos;                                                     \
                }                                                              \
                StringType attributeValue = pos.getCaptured();                 \
                pos.bringToCapture();                                          \
                                                                               \
                /* Invariant - pos is at closing quote */                      \
                pos++;                                                         \
                                                                               \
                /* Invariant - pos is just after closing quote */              \
                /* Allows non-standard <tagName name="value"/> where the       \
                 * backslash is exactly after the quote */                     \
                if (validate && !isWhitespace(*pos) && *pos != '>' &&          \
                    *pos != '/') {                                             \
                    throw std::invalid_argument(                               \
                        "No whitespace after attribute closing quote");        \
                }                                                              \
                                                                               \
                ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities,   \
                                 pos);                                         \
                                                                               \
                /* Continues to either >, /> or another attribute */           \
                skipWhitespace(pos);                                           \
                if (validate && *pos == '\0') {                                \
                    throw std::invalid_argument(                               \
                        "Premature end after attribute");                      \
                }                                                              \
            }                                                                  \
        }                                                                      \
                                                                               \
        bool isSelfClosing = false;                                            \
        /* Invariant - pos is always at / or > */                              \
        if (*pos == '>') {                                                     \
            pos++;                                                             \
        } else if (*pos == '/') {                                              \
            if (validate && isClosing) {                                       \
                throw std::invalid_argument(                                   \
                    "Trying to double-close closing tag");                     \
            }                                                                  \
            pos++;                                                             \
            if (validate && *pos != '>') {                                     \
                throw std::invalid_argument(                                   \
                    "Invalid tag close - must have > after /");                \
            }                                                                  \
            isSelfClosing = true;                                              \
            pos++;                                                             \
        } else if (validate) {                                                 \
            throw std::invalid_argument("No tag close for tag " +              \
                                        std::string(tagName));                 \
        }                                                                      \
                                                                               \
        /* Invariant - pos always after valid tag close */                     \
        if (!isClosing) {                                                      \
            /* Invariant - top stack node is always current parent */          \
            /* Invariant - top stack node is always current parent */          \
            firstTag = false;                                                  \
            OPEN_ACTION(tagName, pos, isSelfClosing);                          \
        } else {                                                               \
            /* Invariant - when closing node, the current parent (stack top)   \
             * must be of the node type */                                     \
            CLOSE_ACTION(tagName, pos);                                        \
        }                                                                      \
    }
}  // namespace onyx::dynamic::parser