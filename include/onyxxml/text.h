#pragma once

#include <optional>

#include "node.h"
#include "parse/is_cursor.h"

namespace onyx::dynamic {
namespace text {
/**
 * @brief Escape a string from XML injection causing elements.
 * By default only escapes the crucial XML injection characters: <, >, ", ', &.
 * It can also escape unicode sequences if the escapeMultiByte argument is set
 * to true. Unicode sequences can be unsafe in environments that do not support
 * UTF-8 and can be used to create injections. Unicode escaping escapes any
 * unicode sequence to an XML/HTML entity. Due to the runtime cost and overall
 * low risk this is false by default.
 *
 * @param str
 * @param escapeMultiByte Whether Unicode characters should be escaped
 * @return std::string
 */
std::string escape(const std::string& str, bool escapeMultiByte = false);

/**
 * @brief Only escapes unicode sequences if escapeMultiByte is true. Otherwise
 * returns a copy of the string.
 *
 * @param escapeMultiByte
 * @return std::string
 */
std::string escapeMultiByte(const std::string& str, bool escapeMultiByte);

/**
 * @brief For a given unicode codepoint returns the appropriate XML/HTML entity.
 *
 * @param codepoint
 * @return std::string
 */
std::string numericEntity(uint32_t codepoint);

/**
 * @brief Get the Unicode codepoint for a sequence of unicode characters. If the
 * sequence is invalid, returns 0. Does not modify the original pointer.
 *
 * @param read
 * @return uint32_t The codepoint if the sequence is valid, otherwise 0.
 */
uint32_t getUnicodeCodepoint(const char* read);

/**
 * @brief Get the Unicode codepoint for a sequence of unicode characters. If the
 * sequence is invalid, returns 0. Modifies the original cursor.
 *
 * @param read
 * @return uint32_t The codepoint if the sequence is valid, otherwise 0.
 */
template <typename Cursor>
uint32_t getUnicodeCodepoint(Cursor& c)
    requires(parser::isCursor<Cursor>)
{
    uint32_t codepoint = 0;

    if (c.current() == '\0') return 0;

    // 1-byte ASCII character (0xxxxxxx)
    if (static_cast<unsigned char>(c.current()) < 128) {
        codepoint = static_cast<unsigned char>(c.current());
    }
    // 2-byte sequence (110xxxxx 10xxxxxx)
    else if ((static_cast<unsigned char>(c.current()) >> 5) == 0x6) {
        if (static_cast<unsigned char>(c.peek(1)) != '\0') {
            codepoint =
                ((static_cast<unsigned char>(c.current()) & 0x1F) << 6) |
                (static_cast<unsigned char>(c.peek(1)) & 0x3F);

            c.advance(1);
        }
    }
    // 3-byte sequence (1110xxxx 10xxxxxx 10xxxxxx)
    else if ((static_cast<unsigned char>(c.current()) >> 4) == 0xE) {
        if (static_cast<unsigned char>(c.peek(2)) != '\0') {
            codepoint =
                ((static_cast<unsigned char>(c.current()) & 0x0F) << 12) |
                ((static_cast<unsigned char>(c.peek(1)) & 0x3F) << 6) |
                (static_cast<unsigned char>(c.peek(2)) & 0x3F);

            c.advance(2);
        }
    }
    // 4-byte sequence (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
    else if ((static_cast<unsigned char>(c.current()) >> 3) == 0x1E) {
        if (static_cast<unsigned char>(c.peek(3)) != '\0') {
            codepoint =
                ((static_cast<unsigned char>(c.current()) & 0x07) << 18) |
                ((static_cast<unsigned char>(c.peek(1)) & 0x3F) << 12) |
                ((static_cast<unsigned char>(c.peek(2)) & 0x3F) << 6) |
                (static_cast<unsigned char>(c.peek(3)) & 0x3F);

            c.advance(3);
        }
    }

    return codepoint;
}

/**
 * @brief Escape a string from XML injection causing elements.
 * This function does not provide the escape elements, they must be passed in
 * escapeTable. It can also escape unicode sequences if the escapeMultiByte
 * argument is set to true. Unicode sequences can be unsafe in environments that
 * do not support UTF-8 and can be used to create injections. Unicode escaping
 * escapes any unicode sequence to an XML/HTML entity. Due to the runtime cost
 * and overall low risk this is false by default. For maximum performance, the
 * escapeTable should be constexpr.
 *
 *
 * @param str
 * @param escapeTable
 * @param escapeMultiByte Whether Unicode characters should be escaped
 * @return std::string
 */
std::string escape(const std::string& str,
                   const std::array<const char*, 128>& escapeTable,
                   bool escapeMultiByte = false);

/**
 * @brief Escapes all occurrences the given sequence with entities in the whole
 * string.
 *
 * @param str
 * @param sequence
 * @return std::string
 */
std::string escapeSequence(const std::string& str, const char* sequence);

/**
 * @brief Replaces all occurrences the given sequence with the replace sequence
 * in the whole string.
 *
 * @param str
 * @param sequence
 * @param replaceSequence
 * @return std::string
 */
std::string replaceSequence(const std::string& str, const char* sequence,
                            std::string_view replaceSequence);

/**
 * @brief Replaces all occurrences the given sequences with the replace
 * sequences in the whole string. Order of replacement is based on the ordering
 * of the map.
 *
 * @param str
 * @param dictionary
 * @return std::string
 */
std::string replaceSequences(
    const std::string& str,
    const std::vector<std::pair<std::string_view, std::string_view>>&
        dictionary);

/**
 * @brief Normalizes XML entities and EOL in a text string. Any character except
 * '\r' in the eolTable becomes eolChar.
 * '\r' becomes eolChar no matter the table, thus it can be included or not, and
 * if followed by '\n', becomes a single eolChar.
 *
 * This function does everything for performance reasons.
 *
 * @param input
 * @param eolTable
 * @return std::string
 */
std::string expandEntitiesAndNormalizeEol(std::string_view input,
                                          const std::array<bool, 128>& eolTable,
                                          unsigned char eolChar);

/**
 * @brief Expand attributes in text
 *
 * @param input
 * @return std::string
 */
std::string expandText(std::string_view input);

/**
 * @brief Expands XML entities in an attribute, including turning whitespace
 * characters into ' ' (whitespace).
 *
 * Note: The XML spec requires that '\r\n'
 * turn into '\n' before attribute values are normalized. This means that:
 * '\r', '\n', '\t' and '\r\n' all transform to ' ' (whitespace).
 *
 * @param input
 * @return std::string
 */
std::string expandAttributeValue(std::string_view input);

/**
 * @brief Expands EOL in a text string, according to XML rules, turning '\r\n'
 * and '\r' into '\n'.
 *
 * @param input
 * @return std::string
 */
std::string expandEOLOnly(std::string_view input);

/**
 * @brief Transcodes a string from an encoding to UTF-8. Uses iconv.
 * If a string is transcoded, it will have been validated.
 *
 * @param str
 * @param from The encoding from which to transcode, in uppercase
 * @return std::optional<std::string> Returns the string if it is transcoded
 */
std::optional<std::string> transcodeToUtf8(std::string_view str,
                                           const std::string& from);
}  // namespace text
}  // namespace onyx::dynamic