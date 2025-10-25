#include "parse/helpers.h"
#include "text.h"



namespace onyx::dynamic::parser {
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
    if (((unsigned char)*ch >> 5) == 0x6) {  // 2-byte sequence
        if (*(ch + 1) != '\0') {
            ch += 1;
        }
    } else if (((unsigned char)*ch >> 4) == 0xE) {  // 3-byte sequence

        if (*(ch + 1) != '\0' && *(ch + 2) != '\0') {
            ch += 2;
        }
    } else if (((unsigned char)*ch >> 3) == 0x1E) {  // 4-byte sequence

        if (*(ch + 1) != '\0' && *(ch + 2) != '\0' && *(ch + 3) != '\0') {
            ch += 3;
        }
    }
    return codepoint;
}

bool isNameStartChar(const char*& ch) {
    if ((unsigned char)*ch < 128) [[likely]] {
        return (*ch >= 'A' && *ch <= 'Z') || (*ch >= 'a' && *ch <= 'z') ||
               *ch == '_' || *ch == ':';
    }
    uint32_t codepoint = handleUnicodeChar(ch);
    return (codepoint >= 0xC0 && codepoint <= 0xD6) ||
           (codepoint >= 0xD8 && codepoint <= 0xF6) ||
           (codepoint >= 0xF8 && codepoint <= 0x2FF) ||
           (codepoint >= 0x370 && codepoint <= 0x37D) ||
           (codepoint >= 0x37F && codepoint <= 0x1FFF) ||
           (codepoint >= 0x200C && codepoint <= 0x200D) ||
           (codepoint >= 0x2070 && codepoint <= 0x218F) ||
           (codepoint >= 0x2C00 && codepoint <= 0x2FEF) ||
           (codepoint >= 0x3001 && codepoint <= 0xD7FF) ||
           (codepoint >= 0xF900 && codepoint <= 0xFDCF) ||
           (codepoint >= 0xFDF0 && codepoint <= 0xFFFD) ||
           (codepoint >= 0x10000 && codepoint <= 0xEFFFF);
}

bool isNameChar(const char*& ch) {
    if ((unsigned char)*ch < 128) [[likely]] {
        return (*ch >= 'A' && *ch <= 'Z') || (*ch >= 'a' && *ch <= 'z') ||
               (*ch >= '0' && *ch <= '9') || *ch == '_' || *ch == ':' ||
               *ch == '-' || *ch == '.';
    }
    uint32_t codepoint = handleUnicodeChar(ch);
    return codepoint == 0xB7 || (codepoint >= 0xC0 && codepoint <= 0xD6) ||
           (codepoint >= 0xD8 && codepoint <= 0xF6) ||
           (codepoint >= 0xF8 && codepoint <= 0x2FF) ||
           (codepoint >= 0x370 && codepoint <= 0x37D) ||
           (codepoint >= 0x37F && codepoint <= 0x1FFF) ||
           (codepoint >= 0x200C && codepoint <= 0x200D) ||
           (codepoint >= 0x2070 && codepoint <= 0x218F) ||
           (codepoint >= 0x2C00 && codepoint <= 0x2FEF) ||
           (codepoint >= 0x3001 && codepoint <= 0xD7FF) ||
           (codepoint >= 0xF900 && codepoint <= 0xFDCF) ||
           (codepoint >= 0xFDF0 && codepoint <= 0xFFFD) ||
           (codepoint >= 0x10000 && codepoint <= 0xEFFFF) ||
           (codepoint >= 0x0300 && codepoint <= 0x036F) ||
           (codepoint >= 0x203F && codepoint <= 0x2040);
}

std::string_view readName(const char* pos) {
    const char* localPos = pos;
    if (!isNameStartChar(localPos)) return std::string_view();
    localPos++;
    while (isNameChar(localPos)) {
        localPos++;
    }

    return std::string_view(pos, localPos - pos);
}
}