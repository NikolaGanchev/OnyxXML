#pragma once

#include <cstdint>
#include <istream>
#include <string_view>
#include <utility>

#include "../text.h"
#include "is_cursor.h"
#include "string_cursor.h"

#if defined(_MSC_VER)
#define ONYX_NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define ONYX_NOINLINE __attribute__((noinline))
#else
#define ONYX_NOINLINE
#endif

#if defined(_MSC_VER)
#define ONYX_INLINE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define ONYX_INLINE __attribute__((always_inline)) inline
#else
#define ONYX_INLINE inline
#endif

namespace onyx::dynamic::parser {

bool isWhitespace(const char pos);

template <typename Cursor>
ONYX_NOINLINE void skipWhitespace(Cursor& pos)
    requires(isCursor<Cursor>)
{
    while (pos.current() != '\0' && isWhitespace(pos.current())) {
        pos.advance();
    }
}

template <typename Cursor>
ONYX_NOINLINE uint32_t handleUnicodeChar(Cursor& c)
    requires(isCursor<Cursor>)
{
    return text::getUnicodeCodepoint(c);
}

template <typename Cursor>
ONYX_NOINLINE bool isNameStartChar(Cursor& ch)
    requires(isCursor<Cursor>)
{
    if ((unsigned char)ch.current() < 128) [[likely]] {
        return (ch.current() >= 'A' && ch.current() <= 'Z') ||
               (ch.current() >= 'a' && ch.current() <= 'z') ||
               ch.current() == '_' || ch.current() == ':';
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
ONYX_NOINLINE bool isNameChar(Cursor& ch)
    requires(isCursor<Cursor>)
{
    if ((unsigned char)ch.current() < 128) [[likely]] {
        return (ch.current() >= 'A' && ch.current() <= 'Z') ||
               (ch.current() >= 'a' && ch.current() <= 'z') ||
               (ch.current() >= '0' && ch.current() <= '9') ||
               ch.current() == '_' || ch.current() == ':' ||
               ch.current() == '-' || ch.current() == '.';
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

/**
 * @brief Same as isNameStartChar but does not recognize ':'
 *
 * @tparam Cursor
 */
template <typename Cursor>
ONYX_NOINLINE bool isNCNameStartChar(Cursor& ch)
    requires(isCursor<Cursor>)
{
    if ((unsigned char)ch.current() < 128) [[likely]] {
        return (ch.current() >= 'A' && ch.current() <= 'Z') ||
               (ch.current() >= 'a' && ch.current() <= 'z') ||
               ch.current() == '_';
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

/**
 * @brief Same as isNameChar but does not recognize ':'
 *
 * @tparam Cursor
 */
template <typename Cursor>
ONYX_NOINLINE bool isNCNameChar(Cursor& ch)
    requires(isCursor<Cursor>)
{
    if ((unsigned char)ch.current() < 128) [[likely]] {
        return (ch.current() >= 'A' && ch.current() <= 'Z') ||
               (ch.current() >= 'a' && ch.current() <= 'z') ||
               (ch.current() >= '0' && ch.current() <= '9') ||
               ch.current() == '_' || ch.current() == '-' ||
               ch.current() == '.';
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

bool isDigit(char c);

template <typename Cursor>
ONYX_NOINLINE typename Cursor::StringType readName(Cursor& pos)
    requires(isCursor<Cursor>)
{
    pos.beginCapture();
    pos.swapDefault();
    if (!isNameStartChar(pos)) {
        pos.swapDefault();
        return typename Cursor::StringType();
    }
    pos.advance();
    while (isNameChar(pos)) {
        pos.advance();
    }
    pos.swapDefault();

    return pos.getCaptured();
}

template <typename Cursor>
ONYX_NOINLINE typename Cursor::StringType readNCName(Cursor& pos)
    requires(isCursor<Cursor>)
{
    pos.beginCapture();
    pos.swapDefault();
    if (!isNCNameStartChar(pos)) {
        pos.swapDefault();
        return typename Cursor::StringType();
    }
    pos.advance();
    while (isNCNameChar(pos)) {
        pos.advance();
    }
    pos.swapDefault();

    return pos.getCaptured();
}

/**
 * @brief A QName is a name of the sort NCName : NCName
 *
 * @tparam Cursor
 */
template <typename Cursor>
ONYX_NOINLINE typename Cursor::StringType readQName(Cursor& pos)
    requires(isCursor<Cursor>)
{
    pos.beginCapture();
    pos.swapDefault();
    // Read first ncname
    if (!isNCNameStartChar(pos)) {
        pos.swapDefault();
        return typename Cursor::StringType();
    }
    pos.advance();
    while (isNCNameChar(pos)) {
        pos.advance();
    }
    // read second nc name
    if (pos.current() == ':') {
        pos.advance();
        if (!isNCNameStartChar(pos)) {
            pos.swapDefault();
            return typename Cursor::StringType();
        }
        pos.advance();
        while (isNCNameChar(pos)) {
            pos.advance();
        }
    }
    pos.swapDefault();

    return pos.getCaptured();
}
}  // namespace onyx::dynamic::parser