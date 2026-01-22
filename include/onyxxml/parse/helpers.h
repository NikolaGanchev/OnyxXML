#pragma once

#include <cstdint>
#include <istream>
#include <string_view>
#include <utility>

#include "../text.h"
#include "is_cursor.h"
#include "string_cursor.h"

namespace onyx::dynamic::parser {
bool isWhitespace(const char pos);

template <typename Cursor>
void skipWhitespace(Cursor& pos)
    requires(isCursor<Cursor>)
{
    while (*pos != '\0' && isWhitespace(*pos)) {
        pos++;
    }
}

template <typename Cursor>
uint32_t handleUnicodeChar(Cursor& c)
    requires(isCursor<Cursor>)
{
    return text::getUnicodeCodepoint(c);
}

template <typename Cursor>
bool isNameStartChar(Cursor& ch)
    requires(isCursor<Cursor>)
{
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

template <typename Cursor>
bool isNameChar(Cursor& ch)
    requires(isCursor<Cursor>)
{
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

template <typename Cursor>
typename Cursor::StringType readName(Cursor& pos)
    requires(isCursor<Cursor>)
{
    pos.beginCapture();
    pos.swapDefault();
    if (!isNameStartChar(pos)) {
        pos.swapDefault();
        return std::string_view();
    }
    pos++;
    while (isNameChar(pos)) {
        pos++;
    }
    pos.swapDefault();

    return pos.getCaptured();
}
}  // namespace onyx::dynamic::parser