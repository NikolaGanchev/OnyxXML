#include "dom_parser.h"
#include <vector>
#include "../nodes/generic_node.h"
#include "../nodes/empty_node.h"
#include "../nodes/text_node.h"
#include "../nodes/comment_node.h"
#include "../nodes/processing_instruction_node.h"
#include "../nodes/cdata_node.h"
#include "../nodes/xml_declaration_node.h"
#include "../nodes/doctype_node.h"
#include "text.h"
#include <utility>

namespace Templater::dynamic::parser {
    ParseResult::ParseResult(): arena{0}, root{nullptr} {}

    ParseResult::ParseResult(Arena arena, Node* root): arena{std::move(arena)}, root{root} {}

    ParseResult::ParseResult(ParseResult&& other): arena{std::move(other.arena)} {
        this->root = other.root;
        other.root = nullptr;
    }

    ParseResult& ParseResult::operator=(ParseResult&& other) {
        this->arena = std::move(other.arena);
        this->root = other.root;
        other.root = nullptr;

        return *this;
    }

    bool isWhitespace(const char pos) {
        return (pos == ' ' || pos == '\t' || pos == '\r' || pos == '\n');
    }

    const char* skipWhitespace(const char* pos) {
        while (isWhitespace(*pos)) {
            pos++;
        }

        return pos;
    }

    uint32_t handleUnicodeChar(const char*& ch) {
        uint32_t codepoint = text::getUnicodeCodepoint(ch);
        if (((unsigned char)*ch >> 5) == 0x6) { // 2-byte sequence
            if (*(ch + 1) != '\0') {
                ch += 1;
            }
        } else if (((unsigned char)*ch >> 4) == 0xE) { // 3-byte sequence
        
            if (*(ch + 1) != '\0' && *(ch + 2) != '\0') {
                ch += 2;
            }
        } else if (((unsigned char)*ch >> 3) == 0x1E) { // 4-byte sequence
        
            if (*(ch + 1) != '\0' && *(ch + 2) != '\0' && *(ch + 3) != '\0') {
                ch += 3;
            }
        }
        return codepoint;
    }

    bool isNameStartChar(const char*& ch) {
        if ((unsigned char)*ch < 128) [[likely]] {
            return (*ch >= 'A' && *ch <= 'Z') || (*ch >= 'a' && *ch <= 'z') || *ch == '_' || *ch == ':';
            
        }
        uint32_t codepoint = handleUnicodeChar(ch);
        return (codepoint >= 0xC0 && codepoint <= 0xD6)
                || (codepoint >= 0xD8 && codepoint <= 0xF6)
                || (codepoint >= 0xF8 && codepoint <= 0x2FF)
                || (codepoint >= 0x370 && codepoint <= 0x37D)
                || (codepoint >= 0x37F && codepoint <= 0x1FFF)
                || (codepoint >= 0x200C && codepoint <= 0x200D)
                || (codepoint >= 0x2070 && codepoint <= 0x218F)
                || (codepoint >= 0x2C00 && codepoint <= 0x2FEF)
                || (codepoint >= 0x3001 && codepoint <= 0xD7FF)
                || (codepoint >= 0xF900 && codepoint <= 0xFDCF)
                || (codepoint >= 0xFDF0 && codepoint <= 0xFFFD)
                || (codepoint >= 0x10000 && codepoint <= 0xEFFFF);
    }  

    bool isNameChar(const char*& ch) {
        if ((unsigned char)*ch < 128) [[likely]] {
            return (*ch >= 'A' && *ch <= 'Z') 
                || (*ch >= 'a' && *ch <= 'z') 
                || (*ch >= '0' && *ch <= '9')
                || *ch == '_' 
                || *ch == ':'
                || *ch == '-'
                || *ch == '.';
            
        }
        uint32_t codepoint = handleUnicodeChar(ch);
        return codepoint == 0xB7
                || (codepoint >= 0xC0 && codepoint <= 0xD6)
                || (codepoint >= 0xD8 && codepoint <= 0xF6)
                || (codepoint >= 0xF8 && codepoint <= 0x2FF)
                || (codepoint >= 0x370 && codepoint <= 0x37D)
                || (codepoint >= 0x37F && codepoint <= 0x1FFF)
                || (codepoint >= 0x200C && codepoint <= 0x200D)
                || (codepoint >= 0x2070 && codepoint <= 0x218F)
                || (codepoint >= 0x2C00 && codepoint <= 0x2FEF)
                || (codepoint >= 0x3001 && codepoint <= 0xD7FF)
                || (codepoint >= 0xF900 && codepoint <= 0xFDCF)
                || (codepoint >= 0xFDF0 && codepoint <= 0xFFFD)
                || (codepoint >= 0x10000 && codepoint <= 0xEFFFF)
                || (codepoint >= 0x0300 && codepoint <= 0x036F)
                || (codepoint >= 0x203F && codepoint <= 0x2040);
    }       

    std::string_view readName(const char* pos) {
        const char* localPos = pos;
        if (!isNameStartChar(localPos)) return std::string_view();
        localPos++;
        while (isNameChar(localPos)) {
            localPos++;
        }

        return std::string_view(pos, localPos-pos);
    }

#define INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, character, exceptionString) \
    if (validate && *pos != character) {              \
        throw std::invalid_argument(exceptionString); \
    }                                                 \
    pos++;
 
#define PARSE_BODY(validate)                                                                                                                            \
    bool firstTag = true;                                                                                                                               \
    while(*pos != '\0') {                                                                                                                               \
            /* Invariant - always at the start of either a tag or a sequence of text */                                                                 \
            if (*pos != '<') {                                                                                                                          \
                const char* localPos = pos;                                                                                                             \
                bool hasEntities = false;                                                                                                               \
                bool end = false;                                                                                                                       \
                while (*localPos != '<') {                                                                                                              \
                    if (*localPos == '\0') {                                                                                                            \
                        while (*pos != '\0') {                                                                                                          \
                            if (validate && !isWhitespace(*pos)) throw std::invalid_argument("Invalid end after tag open");                             \
                            pos++;                                                                                                                      \
                        }                                                                                                                               \
                        end = true;                                                                                                                     \
                        break;                                                                                                                          \
                    }                                                                                                                                   \
                    if (*localPos == '&') hasEntities = true;                                                                                           \
                    localPos++;                                                                                                                         \
                }                                                                                                                                       \
                if (end) break;                                                                                                                         \
                std::string_view text(pos, localPos - pos);                                                                                             \
                pos = localPos;                                                                                                                         \
                                                                                                                                                        \
                TEXT_ACTION(text, hasEntities, pos);                                                                                                    \
                                                                                                                                                        \
                continue;                                                                                                                               \
            }                                                                                                                                           \
                                                                                                                                                        \
            /* Invariant - always in tag name */                                                                                                        \
            pos++;                                                                                                                                      \
                                                                                                                                                        \
            if (validate && *pos == '\0') {                                                                                                             \
                throw std::invalid_argument("Premature end of document");                                                                               \
            }                                                                                                                                           \
                                                                                                                                                        \
            if (*pos == '?') {                                                                                                                          \
                /* Invariant - at processing directive */                                                                                               \
                pos++;                                                                                                                                  \
                if (validate && *pos == '\0') {                                                                                                         \
                    throw std::invalid_argument("Premature end of document");                                                                           \
                }                                                                                                                                       \
                std::string_view tagName = readName(pos);                                                                                               \
                if (validate && tagName.empty()) {                                                                                                      \
                    throw std::invalid_argument("Invalid tag name");                                                                                    \
                }                                                                                                                                       \
                pos += tagName.size();                                                                                                                  \
                /* Invariant - after tag name */                                                                                                        \
                if (tagName.size() == 3 && tolower(tagName[0]) == 'x' && tolower(tagName[1]) == 'm' && tolower(tagName[2]) == 'l') {                    \
                    if (!firstTag) {                                                                                                                    \
                        throw std::invalid_argument("XML declaration is only allowed at the first position in the prologue");                           \
                    } else {                                                                                                                            \
                        /* Invariant - right after xml tag */                                                                                           \
                        bool hasVersion = false;                                                                                                        \
                        bool hasEncoding = false;                                                                                                       \
                        bool hasStandalone = false;                                                                                                     \
                        std::string_view version, encoding, standalone;                                                                                 \
                                                                                                                                                        \
                        /* Walk through all pseudo-attributes in the xml decl */                                                                        \
                        while (*pos != '\0' && *pos != '?') {                                                                                           \
                            /* skip whitespace */                                                                                                       \
                            pos = skipWhitespace(pos);                                                                                                  \
                            if (*pos == '?') {                                                                                                          \
                                break;                                                                                                                  \
                            }                                                                                                                           \
                            if (*pos == '\0') {                                                                                                         \
                                throw std::invalid_argument("Premature end of document");                                                               \
                            }                                                                                                                           \
                                                                                                                                                        \
                            /* read attribute name */                                                                                                   \
                            std::string_view attrName = readName(pos);                                                                                  \
                            if (validate && attrName.empty()) {                                                                                         \
                                throw std::invalid_argument("Invalid XML declaration attribute name");                                                  \
                            }                                                                                                                           \
                            pos += attrName.size();                                                                                                     \
                                                                                                                                                        \
                            /* expect '=' */                                                                                                            \
                            pos = skipWhitespace(pos);                                                                                                  \
                            if (validate && *pos != '=') {                                                                                              \
                                throw std::invalid_argument("No '=' after XML declaration attribute name");                                             \
                            }                                                                                                                           \
                            pos++;                                                                                                                      \
                            if (*pos == '\0') {                                                                                                         \
                                throw std::invalid_argument("Premature end of document");                                                               \
                            }                                                                                                                           \
                                                                                                                                                        \
                            pos = skipWhitespace(pos);                                                                                                  \
                            if (validate && (*pos != '"' && *pos != '\''))  {                                                                           \
                                throw std::invalid_argument("XML declaration attribute value not quoted");                                              \
                            }                                                                                                                           \
                            char quote = *pos++;                                                                                                        \
                                                                                                                                                        \
                            const char* valStart = pos;                                                                                                 \
                            while (*pos != '\0' && *pos != '?' && *pos != quote) {                                                                      \
                                pos++;                                                                                                                  \
                            }                                                                                                                           \
                            if (validate && (*pos == '?' || *pos == '\0' || *pos != quote)) {                                                           \
                                throw std::invalid_argument("Unterminated XML declaration attribute value");                                            \
                            }                                                                                                                           \
                            std::string_view val(valStart, (size_t) (pos - valStart));                                                                  \
                            pos++;  /* skip closing quote */                                                                                            \
                            if (*pos == '\0') {                                                                                                         \
                                throw std::invalid_argument("Premature end of document");                                                               \
                            }                                                                                                                           \
                                                                                                                                                        \
                            if (attrName == "version") {                                                                                                \
                                version = val;                                                                                                          \
                                hasVersion = true;                                                                                                      \
                            }                                                                                                                           \
                            else if (attrName == "encoding") {                                                                                          \
                                encoding = val;                                                                                                         \
                                hasEncoding = true;                                                                                                     \
                            }                                                                                                                           \
                            else if (attrName == "standalone") {                                                                                        \
                                standalone = val;                                                                                                       \
                                hasStandalone = true;                                                                                                   \
                            }                                                                                                                           \
                            else if (validate) {                                                                                                        \
                                throw std::invalid_argument(std::string("Invalid XML declaration attribute '") + std::string(attrName) + "'");          \
                            }                                                                                                                           \
                        }                                                                                                                               \
                        /* Invariant - pos at ?*/                                                                                                       \
                        if (*(pos+1) != '>') {                                                                                                          \
                                throw std::invalid_argument("Unclosed XML declaration");                                                                \
                        }                                                                                                                               \
                        pos++;                                                                                                                          \
                        if (*pos == '\0') {                                                                                                             \
                            throw std::invalid_argument("Premature end of document");                                                                   \
                        }                                                                                                                               \
                        pos++;                                                                                                                          \
                                                                                                                                                        \
                        /* enforce presence and legality */                                                                                             \
                        if (!hasVersion) {                                                                                                              \
                            throw std::invalid_argument("XML declaration must include version");                                                        \
                        }                                                                                                                               \
                        if (validate && version != "1.0" && version != "1.1") {                                                                         \
                            throw std::invalid_argument("Unsupported XML version, must be '1.0' or '1.1'");                                             \
                        }                                                                                                                               \
                        /* encoding is optional in 1.0, but if present must match NMTOKEN */                                                            \
                        if (hasEncoding && validate) {                                                                                                  \
                            /* simple check: no spaces, starts with letter */                                                                           \
                            if (encoding.empty() || !isalpha(encoding[0]))                                                                              \
                                throw std::invalid_argument("Invalid encoding in XML declaration");                                                     \
                        }                                                                                                                               \
                        /* standalone defaults to "no" if not present */                                                                                \
                        if (!hasStandalone) {                                                                                                           \
                            standalone = "no";                                                                                                          \
                        } else if (validate && standalone != "yes" && standalone != "no") {                                                             \
                            throw std::invalid_argument("Invalid standalone value, must be 'yes' or 'no'");                                             \
                        }                                                                                                                               \
                        bool isStandalone = (standalone[0] == 'y');                                                                                     \
                        if (encoding.size() == 0) encoding = "UTF-8";                                                                                   \
                                                                                                                                                        \
                        XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, hasStandalone, pos);                                       \
                        continue;                                                                                                                       \
                    }                                                                                                                                   \
                }                                                                                                                                       \
                if (validate && (*pos != ' ' || *(pos+1) == '\0')) {                                                                                    \
                    throw std::invalid_argument("No space between processing instruction target and processing instruction content");                   \
                }                                                                                                                                       \
                pos++;                                                                                                                                  \
                                                                                                                                                        \
                const char* localPos = pos;                                                                                                             \
                                                                                                                                                        \
                while (!(*localPos == '?' && *(localPos + 1) != '\0' && *(localPos + 1) == '>')) {                                                      \
                    localPos++;                                                                                                                         \
                    if (validate && *localPos == '\0') throw std::invalid_argument("Invalid processing instruction without ending");                    \
                }                                                                                                                                       \
                                                                                                                                                        \
                std::string_view processingInstruction(pos, localPos-pos);                                                                              \
                                                                                                                                                        \
                pos = localPos + 2;                                                                                                                     \
                                                                                                                                                        \
                /* Invariant - just after processing instruction end */                                                                                 \
                INSTRUCTION_ACTION(tagName, processingInstruction, pos);                                                                                \
                firstTag = false;                                                                                                                       \
                continue;                                                                                                                               \
            } else if (*pos == '!') {                                                                                                                   \
                pos++;                                                                                                                                  \
                if (validate && *pos == '\0') {                                                                                                         \
                    throw std::invalid_argument("Premature end of <! tag");                                                                             \
                }                                                                                                                                       \
                                                                                                                                                        \
                if (*pos == '-') {                                                                                                                      \
                    pos++;                                                                                                                              \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, '-', "Premature end of comment");                                                   \
                    /* Invariant - right after <!-- of comment */                                                                                       \
                    if (validate && *pos == '\0') {                                                                                                     \
                        throw std::invalid_argument("Premature end of document");                                                                       \
                    }                                                                                                                                   \
                    const char* localPos = pos;                                                                                                         \
                                                                                                                                                        \
                    while (!(*localPos == '-' && *(localPos + 1) != '\0' && *(localPos + 1) == '-')) {                                                  \
                        localPos++;                                                                                                                     \
                        if (validate && *localPos == '\0') throw std::invalid_argument("Invalid comment without ending");                               \
                    }                                                                                                                                   \
                                                                                                                                                        \
                    std::string_view commentText(pos, localPos-pos);                                                                                    \
                                                                                                                                                        \
                    localPos += 2;                                                                                                                      \
                                                                                                                                                        \
                    /* Invariant - either at error state or at > */                                                                                     \
                    if (validate && *localPos != '>') throw std::invalid_argument("-- inside of comment not allowed");                                  \
                    pos = localPos;                                                                                                                     \
                    pos++;                                                                                                                              \
                    COMMENT_ACTION(commentText, pos);                                                                                                   \
                    firstTag = false;                                                                                                                   \
                    continue;                                                                                                                           \
                } else if (*pos == '[') {                                                                                                               \
                    pos++;                                                                                                                              \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'C', "Premature end of CDATA section");                                             \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'D', "Premature end of CDATA section");                                             \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'A', "Premature end of CDATA section");                                             \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'T', "Premature end of CDATA section");                                             \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'A', "Premature end of CDATA section");                                             \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, '[', "Premature end of CDATA section");                                             \
                    /* Invariant - right after <![CDATA[ */                                                                                             \
                    if (validate && *pos == '\0') {                                                                                                     \
                        throw std::invalid_argument("Premature end of document");                                                                       \
                    }                                                                                                                                   \
                    const char* localPos = pos;                                                                                                         \
                                                                                                                                                        \
                    while (!(*localPos == ']'                                                                                                           \
                                && *(localPos + 1) != '\0' && *(localPos + 1) == ']'                                                                    \
                                && *(localPos + 2) != '\0' && *(localPos + 2) == '>')) {                                                                \
                        localPos++;                                                                                                                     \
                        if (validate && *localPos == '\0') throw std::invalid_argument("Invalid CDATA without ending");                                 \
                    }                                                                                                                                   \
                                                                                                                                                        \
                    std::string_view cdataText(pos, localPos-pos);                                                                                      \
                                                                                                                                                        \
                    pos = localPos + 3;                                                                                                                 \
                    CDATA_ACTION(cdataText, pos);                                                                                                       \
                    firstTag = false;                                                                                                                   \
                    continue;                                                                                                                           \
                } else if (*pos == 'D') {                                                                                                               \
                    pos++;                                                                                                                              \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'O', "Premature end of DOCTYPE section");                                           \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'C', "Premature end of DOCTYPE section");                                           \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'T', "Premature end of DOCTYPE section");                                           \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'Y', "Premature end of DOCTYPE section");                                           \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'P', "Premature end of DOCTYPE section");                                           \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, 'E', "Premature end of DOCTYPE section");                                           \
                    INCREMENT_POS_IF_EQUALS_OR_THROW(validate, pos, ' ', "Premature end of DOCTYPE section");                                           \
                                                                                                                                                        \
                    const char* localPos = pos;                                                                                                         \
                                                                                                                                                        \
                    while (*localPos != '>') {                                                                                                          \
                        localPos++;                                                                                                                     \
                        if (validate && *localPos == '\0') throw std::invalid_argument("Invalid DOCTYPE without ending");                               \
                    }                                                                                                                                   \
                                                                                                                                                        \
                    std::string_view doctypeText(pos, localPos-pos);                                                                                    \
                                                                                                                                                        \
                    pos = localPos;                                                                                                                     \
                    pos++;                                                                                                                              \
                    DOCTYPE_ACTION(doctypeText, pos);                                                                                                   \
                    firstTag = false;                                                                                                                   \
                    continue;                                                                                                                           \
                } else if (validate) {                                                                                                                  \
                    throw std::invalid_argument("Tag name cannot contain '!'");                                                                         \
                }                                                                                                                                       \
            }                                                                                                                                           \
                                                                                                                                                        \
            bool isClosing = (*pos == '/');                                                                                                             \
                                                                                                                                                        \
            if (isClosing) pos++;                                                                                                                       \
                                                                                                                                                        \
            if (validate && *pos == '\0') {                                                                                                             \
                throw std::invalid_argument("Premature end of document");                                                                               \
            }                                                                                                                                           \
                                                                                                                                                        \
            /* Invariant - pos always at tag name start */                                                                                              \
            std::string_view tagName = readName(pos);                                                                                                   \
            if (validate && tagName.empty()) {                                                                                                          \
                throw std::invalid_argument("Invalid tag name");                                                                                        \
            }                                                                                                                                           \
            pos += tagName.size();                                                                                                                      \
                                                                                                                                                        \
            bool couldHaveAttributes = isWhitespace(*pos) && !isClosing;                                                                                \
                                                                                                                                                        \
            /* Invariant - pos always after tag name end */                                                                                             \
            pos = skipWhitespace(pos);                                                                                                                  \
                                                                                                                                                        \
            if (validate && *pos == '\0') {                                                                                                             \
                throw std::invalid_argument("Premature end of document");                                                                               \
            }                                                                                                                                           \
                                                                                                                                                        \
            if (couldHaveAttributes) {                                                                                                                  \
                /* Invariant - either at start of attribute or at > */                                                                                  \
                while (*pos != '>' && *pos != '/') {                                                                                                    \
                    std::string_view attributeName = readName(pos);                                                                                     \
                    if (attributeName.empty()) {                                                                                                        \
                        throw std::invalid_argument("Invalid non-closing tag");                                                                         \
                    }                                                                                                                                   \
                    pos += attributeName.size();                                                                                                        \
                                                                                                                                                        \
                    /* Invariant - after attribute name */                                                                                              \
                    pos = skipWhitespace(pos);                                                                                                          \
                                                                                                                                                        \
                    /* Invariant - at = or error state */                                                                                               \
                    if (validate && (*pos == '\0' || *pos != '=')) {                                                                                    \
                        throw std::invalid_argument("No = after attribute");                                                                            \
                    }                                                                                                                                   \
                    pos++;                                                                                                                              \
                    /* Invariant - after attribute = */                                                                                                 \
                    pos = skipWhitespace(pos);                                                                                                          \
                    char attributeQuote = '\0';                                                                                                         \
                    if (validate && *pos == '\0') {                                                                                                     \
                        throw std::invalid_argument("Premature end at attribute");                                                                      \
                    }                                                                                                                                   \
                                                                                                                                                        \
                    if (*pos == '\'' || *pos == '\"') {                                                                                                 \
                        attributeQuote = *pos;                                                                                                          \
                    } else if (validate) {                                                                                                              \
                        throw std::invalid_argument("No quote (\" or \') after attribute =");                                                           \
                    }                                                                                                                                   \
                    pos++;                                                                                                                              \
                                                                                                                                                        \
                    /* Invariant - after attribute value opening quote */                                                                               \
                    const char* localPos = pos;                                                                                                         \
                    bool hasEntities = false;                                                                                                           \
                    while (*localPos != attributeQuote) {                                                                                               \
                        if (validate && *localPos == '\0') throw std::invalid_argument("Improperly closed attribute value");                            \
                        if (*localPos == '&') hasEntities = true;                                                                                       \
                        localPos++;                                                                                                                     \
                    }                                                                                                                                   \
                    std::string_view attributeValue(pos, localPos - pos);                                                                               \
                    pos = localPos;                                                                                                                     \
                                                                                                                                                        \
                    /* Invariant - pos is at closing quote */                                                                                           \
                    pos++;                                                                                                                              \
                                                                                                                                                        \
                    /* Invariant - pos is just after closing quote */                                                                                   \
                    /* Allows non-standard <tagName name="value"/> where the backslash is exactly after the quote */                                    \
                    if (validate && !isWhitespace(*pos) && *pos != '>' && *pos != '/') {                                                                \
                        throw std::invalid_argument("No whitespace after attribute closing quote");                                                     \
                    }                                                                                                                                   \
                                                                                                                                                        \
                    ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos);                                                                  \
                                                                                                                                                        \
                    /* Continues to either >, /> or another attribute */                                                                                \
                    pos = skipWhitespace(pos);                                                                                                          \
                    if (validate && *pos == '\0') {                                                                                                     \
                        throw std::invalid_argument("Premature end after attribute");                                                                   \
                    }                                                                                                                                   \
                }                                                                                                                                       \
            }                                                                                                                                           \
                                                                                                                                                        \
            bool isSelfClosing = false;                                                                                                                 \
            /* Invariant - pos is always at / or > */                                                                                                   \
            if (*pos == '>') {                                                                                                                          \
                pos++;                                                                                                                                  \
            } else if (*pos == '/') {                                                                                                                   \
                if (validate && isClosing) {                                                                                                            \
                    throw std::invalid_argument("Trying to double-close closing tag");                                                                  \
                }                                                                                                                                       \
                pos++;                                                                                                                                  \
                if (validate && *pos != '>') {                                                                                                          \
                    throw std::invalid_argument("Invalid tag close - must have > after /");                                                             \
                }                                                                                                                                       \
                isSelfClosing = true;                                                                                                                   \
                pos++;                                                                                                                                  \
            } else if (validate) {                                                                                                                      \
                throw std::invalid_argument("No tag close for tag " + std::string(tagName));                                                                                            \
            }                                                                                                                                           \
                                                                                                                                                        \
            /* Invariant - pos always after valid tag close */                                                                                          \
            if (!isClosing) {                                                                                                                           \
                /* Invariant - top stack node is always current parent */                                                                               \
                /* Invariant - top stack node is always current parent */                                                                               \
                firstTag = false;                                                                                                                       \
                OPEN_ACTION(tagName, pos, isSelfClosing);                                                                                               \
            } else {                                                                                                                                    \
                /* Invariant - when closing node, the current parent (stack top) must be of the node type */                                            \
                CLOSE_ACTION(tagName, pos);                                                                                                             \
            }                                                                                                                                           \
        }

    Arena DomParser::parseDryRun(std::string_view input) {
        std::vector<size_t> stack;

        size_t id = 0;

        const char* pos = input.data();

        Arena::Builder builder;

        std::hash<std::string_view> hasher;

        // Root
        builder.preallocate<tags::EmptyNode>();

        std::string_view root = ".empty";

        stack.push_back(hasher(root));

        pos = skipWhitespace(pos);

        #define TEXT_ACTION(text, hasEntities, pos) builder.preallocate<tags::Text>();

        #define COMMENT_ACTION(commentText, pos) builder.preallocate<tags::Comment>();

        #define CDATA_ACTION(cdataText, pos) builder.preallocate<tags::CData>();

        #define XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, hasStandalone, pos) builder.preallocate<tags::XmlDeclaration>();

        #define INSTRUCTION_ACTION(tagName, processingInstruction, pos) builder.preallocate<tags::ProcessingInstruction>();

        #define ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos) 

        #define DOCTYPE_ACTION(doctypeText, pos) builder.preallocate<tags::Doctype>();

        #define OPEN_ACTION(tagName, pos, isSelfClosing)           \
            builder.preallocate<tags::GenericNode>();              \
            if (!isSelfClosing) {                                  \
                stack.push_back(hasher(tagName));                  \
            }

        #define CLOSE_ACTION(tagName, pos)                                 \
            size_t thisNode = stack.back();                                \
            if (thisNode != hasher(tagName)) {                             \
                throw std::invalid_argument("Closing unopened tag");       \
            }                                                              \
            if (stack.size() == 1) {                                       \
                throw std::invalid_argument("Closing non-existent tags");  \
            }                                                              \
            stack.pop_back();

        PARSE_BODY(true);
        
        #undef TEXT_ACTION
        #undef COMMENT_ACTION
        #undef CDATA_ACTION
        #undef INSTRUCTION_ACTION
        #undef ATTRIBUTE_ACTION
        #undef OPEN_ACTION
        #undef XML_DECLARATION_ACTION
        #undef DOCTYPE_ACTION
        #undef CLOSE_ACTION

        // Invariant - stack may only contain the root
        if (stack.size() != 1 || stack[0] != hasher(root)) {
            throw std::invalid_argument("Unclosed tags left");
        }

        return builder.build(); // first argument is temporarily nullptr until further improvements
    }

    ParseResult DomParser::parse(std::string_view input) {
        std::vector<Node*> stack;
        std::vector<std::string_view> attributeNames;
        std::vector<std::pair<std::string_view, bool>> attributeValues;

        const char* pos = input.data();

        Arena arena = parseDryRun(input);

        Node* root = arena.allocate<tags::EmptyNode>();

        stack.push_back(root);

        pos = skipWhitespace(pos);

        #define TEXT_ACTION(text, hasEntities, pos) stack.back()->addChild(\
            arena.allocate<tags::Text>(\
                hasEntities ? \
                    text::expandEntities(text): \
                    std::string(text)));

        #define COMMENT_ACTION(commentText, pos) stack.back()->addChild(arena.allocate<tags::Comment>(std::string(commentText)));

        #define CDATA_ACTION(cdataText, pos) stack.back()->addChild(arena.allocate<tags::CData>(std::string(cdataText)));

        #define INSTRUCTION_ACTION(tagName, processingInstruction, pos) \
                stack.back()->addChild(arena.allocate<tags::ProcessingInstruction>(std::string(tagName), std::string(processingInstruction)));

        #define ATTRIBUTE_ACTION(attributeName, attributeValue, hasEntities, pos)   attributeNames.push_back(attributeName);\
                                                                                    attributeValues.push_back(std::make_pair(attributeValue, hasEntities)); 
                                                                        
        #define XML_DECLARATION_ACTION(version, encoding, hasEncoding, isStandalone, hasStandalone, pos) \
            stack.back()->addChild(arena.allocate<tags::XmlDeclaration>(\
                            std::string(version), std::string(encoding), hasEncoding, isStandalone, hasStandalone, false));
        
        #define DOCTYPE_ACTION(doctypeText, pos) stack.back()->addChild(arena.allocate<tags::Doctype>(std::string(doctypeText)));

        #define OPEN_ACTION(tagName, pos, isSelfClosing)                                                            \
            Node* newNode = arena.allocate<tags::GenericNode>(std::string(tagName), isSelfClosing);                 \
                                                                                                                    \
            auto& attributes = newNode->attributes;                                                                 \
            for (int i = 0; i < attributeNames.size(); i++) {                                                       \
                attributes.emplace_back(std::string(attributeNames[i]),                                             \
                    attributeValues[i].second ? text::expandEntities(attributeValues[i].first): std::string(attributeValues[i].first));  \
            }                                                                                                       \
                                                                                                                    \
            stack.back()->addChild(newNode);                                                                        \
            if (!isSelfClosing) {                                                                                   \
                stack.push_back(newNode);                                                                           \
            }                                                                                                       \
            attributeNames.clear();                                                                                 \
            attributeValues.clear();

        #define CLOSE_ACTION(tagName, pos)                                          \
            Node* thisNode = stack.back();                                          \
            stack.pop_back();

        PARSE_BODY(false);
        
        #undef TEXT_ACTION
        #undef COMMENT_ACTION
        #undef CDATA_ACTION
        #undef INSTRUCTION_ACTION
        #undef ATTRIBUTE_ACTION
        #undef OPEN_ACTION
        #undef XML_DECLARATION_ACTION
        #undef DOCTYPE_ACTION
        #undef CLOSE_ACTION

        // Invariant - stack may only contain the root
        if (stack.size() != 1 || stack[0] != root) {
            throw std::invalid_argument("Unclosed tags left");
        }

        if (root->getChildrenCount() == 1) {
            Node* newRoot = root->removeChild(root->getChildren()[0]).release();
            root = std::move(newRoot);
        }

        return ParseResult{std::move(arena), root};
    }
}

#undef INCREMENT_POS_IF_EQUALS_OR_THROW