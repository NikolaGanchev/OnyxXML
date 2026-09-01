#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <utility>

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
 * unicode sequence to an XML entity. Due to the runtime cost and overall
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
 * @brief For a given unicode codepoint returns the appropriate XML entity.
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
 * escapes any unicode sequence to an XML entity. Due to the runtime cost
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

/**
 * @brief Transforms an ASCII string to uppercase independently of any locale.
 *
 * @param stringToUppercase
 * @return std::string
 */
inline void transformAsciiToUpper(std::string& stringToUppercase) {
    for (size_t i = 0; i < stringToUppercase.size(); i++) {
        unsigned char c = stringToUppercase[i];
        stringToUppercase[i] = (c >= 'a' && c <= 'z') ? c - 32 : c;
    }
}

/**
 * @brief Converts an ASCII string to uppercase independently of any locale.
 *
 * @param stringToUppercase
 * @return std::string
 */
inline std::string asciiToUpper(std::string_view stringToUppercase) {
    std::string res;
    res.reserve(stringToUppercase.size());
    for (unsigned char c : stringToUppercase) {
        res.push_back((c >= 'a' && c <= 'z') ? c - 32 : c);
    }
    return res;
}

/**
 * @brief Encodes results of XML autodetection
 *
 */
enum class XmlEncodingAutodetectionResult {
    /**
     * @brief The end of the file was encountered before detection could be
     * done.
     *
     */
    END_OF_FILE,
    /**
     * @brief The encoding detection is certain. This means that a BOM was
     * discovered.
     *
     */
    ENCODING_DETECTED,
    /**
     * @brief The encoding detection can only narrow down to a family.
     *
     */
    FAMILY_DETECTED,
    /**
     * @brief No encoding was detected.
     *
     */
    UNKNOWN
};

/**
 * @brief Automatically detects an encoding. This is done using the first 4
 * bytes of the cursor.
 *
 * If there are less than 4 bytes, an empty string is returned with
 * XmlEncodingAutodetectionResult::END_OF_FILE.
 *
 * If a Byte Order Mark (BOM) is matched, the encoding is returned along with
 * XmlEncodingAutodetectionResult::ENCODING_DETECTED. The Cursor is set to the
 * first byte after the BOM.
 *
 * Otherwise, the first 4 bytes are used to detect a family on encodings. This
 * function picks an encoding for every detected family and returns it. It is
 * guaranteed the picked encoding is correct for at least the XML declaration,
 * which must be read to find the exact encoding. However, it may not be correct
 * for the entire document. The cursor is not moved.
 * XmlEncodingAutodetectionResult::FAMILY_DETECTED is returned.
 *
 * The following default encodings have been chosen for families (families are
 * described as per the XML spec):
 *
 * UCS-4BE or other big endian encoding with a 32-bit code unit and ASCII
 * characters encoded as ASCII values -> UCS-4BE
 *
 * UCS-4LE or other little endian encoding with a 32-bit code unit and ASCII
 * characters encoded as ASCII values -> UCS-4LE
 *
 * UTF-16BE or big-endian ISO-10646-UCS-2 or other encoding with a 16-bit code
 * unit in big-endian order and ASCII characters encoded as ASCII values ->
 * UTF-16BE
 *
 * UTF-16LE or little-endian ISO-10646-UCS-2 or other encoding with a 16-bit
 * code unit in little-endian order and ASCII characters encoded as ASCII values
 * -> UTF-16LE
 *
 * UTF-8, ISO 646, ASCII, some part of ISO 8859, Shift-JIS, EUC, or any other
 * 7-bit, 8-bit, or mixed-width encoding which ensures that the characters of
 * ASCII have their normal positions, width, and values -> UTF-8
 *
 * EBCDIC (in some flavour) -> IBM037
 *
 * Unusual octet orders of UCS-4 encodings are not supported and throw an
 * exception if discovered in either the BOM or as the first characters.
 *
 * Otherwise returns "UTF-8" with XmlEncodingAutodetectionResult::UNKNOWN.
 * @tparam Cursor
 */
template <typename Cursor>
std::pair<std::string, XmlEncodingAutodetectionResult> autodetectXmlEncoding(
    Cursor& pos)
    requires(parser::isCursor<Cursor>)
{
    if (pos.isEOF(3))
        return std::make_pair("", XmlEncodingAutodetectionResult::END_OF_FILE);

    uint32_t marker =
        (static_cast<uint32_t>(static_cast<unsigned char>(pos.peek(0))) << 24) |
        (static_cast<uint32_t>(static_cast<unsigned char>(pos.peek(1))) << 16) |
        (static_cast<uint32_t>(static_cast<unsigned char>(pos.peek(2))) << 8) |
        static_cast<uint32_t>(static_cast<unsigned char>(pos.peek(3)));

    switch (marker) {
        case 0x0000FEFF:
            pos.advance(4);
            pos.beginCapture();
            return std::make_pair(
                "UCS-4BE", XmlEncodingAutodetectionResult::ENCODING_DETECTED);
        case 0xFFFE0000:
            pos.advance(4);
            pos.beginCapture();
            return std::make_pair(
                "UCS-4LE", XmlEncodingAutodetectionResult::ENCODING_DETECTED);
        case 0x0000FFFE:
        case 0x00003C00:
            throw std::invalid_argument(
                "Autodetection detected unsupported UCS-4 unusual octet order "
                "(2143)");
        case 0xFEFF0000:
        case 0x003C0000:
            throw std::invalid_argument(
                "Autodetection detected unsupported UCS-4 unusual octet order "
                "(3412)");
        case 0x0000003C:
            return std::make_pair(
                "UCS-4BE", XmlEncodingAutodetectionResult::FAMILY_DETECTED);
        case 0x3C000000:
            return std::make_pair(
                "UCS-4LE", XmlEncodingAutodetectionResult::FAMILY_DETECTED);
        case 0x003C003F:
            return std::make_pair(
                "UTF-16BE", XmlEncodingAutodetectionResult::FAMILY_DETECTED);
        case 0x3C003F00:
            return std::make_pair(
                "UTF-16LE", XmlEncodingAutodetectionResult::FAMILY_DETECTED);
        case 0x3C3F786D:
            return std::make_pair(
                "UTF-8", XmlEncodingAutodetectionResult::FAMILY_DETECTED);
        case 0x4C6FA794:
            return std::make_pair(
                "IBM037", XmlEncodingAutodetectionResult::FAMILY_DETECTED);
        default: {
            if ((marker & 0xFFFF0000) == 0xFEFF0000) {
                pos.advance(2);
                pos.beginCapture();
                return std::make_pair(
                    "UTF-16BE",
                    XmlEncodingAutodetectionResult::ENCODING_DETECTED);
            } else if ((marker & 0xFFFF0000) == 0xFFFE0000) {
                pos.advance(2);
                pos.beginCapture();
                return std::make_pair(
                    "UTF-16LE",
                    XmlEncodingAutodetectionResult::ENCODING_DETECTED);
            } else if ((marker & 0xFFFFFF00) == 0xEFBBBF00) {
                pos.advance(3);
                pos.beginCapture();
                return std::make_pair(
                    "UTF-8", XmlEncodingAutodetectionResult::ENCODING_DETECTED);
            }

            break;
        }
    }

    return std::make_pair("UTF-8", XmlEncodingAutodetectionResult::UNKNOWN);
}
}  // namespace text
}  // namespace onyx::dynamic