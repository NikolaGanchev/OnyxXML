#include "xpath/functions.h"

#include <array>
#include <charconv>
#include <limits>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "attribute.h"
#include "node.h"
#include "nodes/attribute_view_node.h"
#include "nodes/namespace_view_node.h"
#include "nodes/processing_instruction_node.h"
#include "parse/helpers.h"
#include "parse/string_cursor.h"

namespace onyx::dynamic::xpath::functions {

std::string string(const XPathObject& obj) {
    return std::visit(
        [](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            }

            if constexpr (std::is_same_v<T, double>) {
                // https://www.w3.org/TR/1999/REC-xpath-19991116/#function-string
                if (std::isnan(arg)) {
                    return "NaN";
                }

                if (std::isinf(arg)) {
                    return (arg > 0) ? "Infinity" : "-Infinity";
                }

                if (arg == 0.0) {
                    return "0";
                }

                std::array<char, 512> buf;

                // std::chars_format::fixed with no precision specified
                // automatically produces the shortest representation that
                // uniquely distinguishes the number.
                auto [ptr, ec] =
                    std::to_chars(buf.data(), buf.data() + buf.size(), arg,
                                  std::chars_format::fixed);

                if (ec != std::errc()) {
                    throw std::runtime_error("Cannot convert double to string");
                }

                std::string result(buf.data(), ptr);

                bool isInteger = (std::trunc(arg) == arg);

                if (isInteger) {
                    size_t dotPosition = result.find('.');
                    if (dotPosition != std::string::npos) {
                        result.resize(dotPosition);
                    }
                }

                return result;
            }

            if constexpr (std::is_same_v<T, bool>) {
                return arg ? "true" : "false";
            }

            if constexpr (std::is_same_v<T, std::vector<Node*>>) {
                if (arg.empty()) return "";
                // This takes the first node of the nodeset
                // By spec, it should be the first node by document order
                // This invariant must be kept
                return arg[0]->getStringValue();
            }
        },
        obj.value);
};

bool boolean(const XPathObject& obj) {
    return std::visit(
        [](auto&& arg) -> bool {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, bool>) {
                return arg;
            }

            if constexpr (std::is_same_v<T, double>) {
                return arg != 0 && !std::isnan(arg);
            }

            if constexpr (std::is_same_v<T, std::string>) {
                return !arg.empty();
            }

            if constexpr (std::is_same_v<T, std::vector<Node*>>) {
                return !arg.empty();
            }
        },
        obj.value);
};

// https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring
void trim(std::string& str) {
    str.erase(str.begin(),
              std::find_if(str.begin(), str.end(), [](unsigned char ch) {
                  return !parser::isWhitespace(ch);
              }));

    str.erase(
        std::find_if(str.rbegin(), str.rend(),
                     [](unsigned char ch) { return !parser::isWhitespace(ch); })
            .base(),
        str.end());
}

double number(const XPathObject& obj) {
    return std::visit(
        [&obj](auto&& arg) -> double {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, double>) {
                return arg;
            }

            if constexpr (std::is_same_v<T, bool>) {
                return arg ? 1.0 : 0.0;
            }

            if constexpr (std::is_same_v<T, std::string>) {
                std::string copy = arg;
                trim(copy);

                double result = std::numeric_limits<double>::quiet_NaN();
                auto [ptr, ec] =
                    std::from_chars(copy.data(), copy.data() + copy.size(),
                                    result, std::chars_format::fixed);

                if (ptr != copy.data() + copy.size()) {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                return result;
            }

            if constexpr (std::is_same_v<T, std::vector<Node*>>) {
                std::string strVal = string(obj);

                trim(strVal);

                double result = std::numeric_limits<double>::quiet_NaN();
                auto [ptr, ec] = std::from_chars(
                    strVal.data(), strVal.data() + strVal.size(), result,
                    std::chars_format::fixed);

                if (ptr != strVal.data() + strVal.size()) {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                return result;
            }
        },
        obj.value);
};

double round(double num) {
    if (std::isnan(num)) {
        return num;
    }

    // "If the argument is less than zero, but greater than or equal to -0.5,
    // then negative zero is returned."
    if (num >= -0.5 && num <= 0.0 && std::signbit(num)) {
        return -0.0;
    }

    return std::floor(num + 0.5);
}

std::string substring(const std::string& str, double start, double length) {
    start = functions::round(start);
    length = functions::round(length);

    double limit = start + length;

    // Per the XPath spec, if start or limit is NaN, return empty string
    if (std::isnan(start) || std::isnan(limit)) {
        return "";
    }

    // XPath dictates 1-based character positions
    double lower = std::max(1.0, start);
    double upper = limit;

    if (lower >= upper) {
        return "";
    }

    size_t startCharIndex = static_cast<size_t>(lower);
    size_t endCharIndex = (upper > std::numeric_limits<size_t>::max())
                              ? std::numeric_limits<size_t>::max()
                              : static_cast<size_t>(upper);

    parser::StringCursor cursor(str);
    size_t currentChar = 1;

    // Advance cursor to the start character position
    while (!cursor.isEOF() && currentChar < startCharIndex) {
        if (text::getUnicodeCodepoint(cursor) == 0 && cursor.isEOF()) {
            break;
        }
        cursor.advance(1);
        currentChar++;
    }

    if (cursor.isEOF()) {
        return "";
    }

    const char* byteStart = cursor.ptr;

    // Advance cursor to the end character position
    while (!cursor.isEOF() && currentChar < endCharIndex) {
        if (text::getUnicodeCodepoint(cursor) == 0 && cursor.isEOF()) {
            break;
        }
        cursor.advance(1);
        currentChar++;
    }

    const char* byteEnd = cursor.ptr;

    return std::string(byteStart, byteEnd - byteStart);
}

std::string stringBefore(const std::string& str1, const std::string& str2) {
    std::string::size_type i = str1.find(str2);

    if (i == std::string::npos) {
        return "";
    }

    return str1.substr(0, i);
}

std::string stringAfter(const std::string& str1, const std::string& str2) {
    std::string::size_type i = str1.find(str2);

    if (i == std::string::npos) {
        return std::string("");
    }

    return str1.substr(i + str2.length());
}

std::string translate(const std::string& str1, const std::string& str2,
                      const std::string& str3) {
    std::vector<std::string_view> replacements;
    parser::StringCursor c3(str3);
    while (!c3.isEOF()) {
        const char* start = c3.ptr;
        if (text::getUnicodeCodepoint(c3) == 0 && c3.isEOF()) {
            break;
        }
        c3.advance(1);
        replacements.emplace_back(start, c3.ptr - start);
    }

    // Map codepoint in str2 to a replacement byte slice in str3
    // An empty std::string_view represents character deletion.
    std::unordered_map<uint32_t, std::string_view> map;
    parser::StringCursor c2(str2);
    size_t charIndex = 0;

    while (!c2.isEOF()) {
        uint32_t codepoint = text::getUnicodeCodepoint(c2);
        if (codepoint == 0 && c2.isEOF()) {
            break;
        }
        c2.advance(1);

        // First occurrence determines the replacement
        if (map.find(codepoint) == map.end()) {
            if (charIndex < replacements.size()) {
                map[codepoint] = replacements[charIndex];
            } else {
                // Delete a character
                map[codepoint] = std::string_view();
            }
        }
        charIndex++;
    }

    std::string result;
    result.reserve(str1.size());

    parser::StringCursor c1(str1);
    while (!c1.isEOF()) {
        const char* start = c1.ptr;
        uint32_t codepoint = text::getUnicodeCodepoint(c1);
        if (codepoint == 0 && c1.isEOF()) {
            break;
        }
        c1.advance(1);
        const char* end = c1.ptr;

        auto it = map.find(codepoint);
        if (it == map.end()) {
            // If not in str2, preserve original character slice
            result.append(start, end - start);
        } else {
            // If in str2, append mapped slice
            // If the slice is empty, the character is dropped
            result.append(it->second);
        }
    }

    return result;
}

std::string normalizeSpace(const std::string& str) {
    std::string res;

    parser::StringCursor pos(str.data());

    while (pos.current() != '\0' && parser::isWhitespace(pos.current())) {
        pos.advance();
    }

    pos.beginCapture();

    while (pos.capturePeek(0) != '\0') {
        if (parser::isWhitespace(pos.capturePeek(0))) {
            res += pos.getCaptured();

            while (pos.capturePeek(0) != '\0' &&
                   parser::isWhitespace(pos.capturePeek(0)))
                pos.captureAdvance(1);

            if (pos.capturePeek(0) != '\0') {
                res += ' ';
            }

            pos.bringToCapture();
            continue;
        }
        pos.captureAdvance(1);
    }

    if (pos.getCaptured() != "") {
        res += pos.getCaptured();
    }

    return res;
}

double stringLength(const std::string& str) {
    double count = 0.0;
    parser::StringCursor cursor(str);

    while (!cursor.isEOF()) {
        uint32_t cp = text::getUnicodeCodepoint(cursor);
        if (cp == 0 && cursor.isEOF()) {
            break;
        }
        cursor.advance(1);
        count++;
    }

    return count;
}

std::string name(const XPathObject& obj) {
    const std::vector<Node*>& arg = obj.asNodeset();
    if (arg.empty()) return "";

    Node* node = arg[0];

    switch (node->getXPathType()) {
        case Node::XPathType::ELEMENT: {
            std::optional<std::string_view> prefix = node->getNamespacePrefix();
            if (!prefix.has_value()) return node->getTagName();

            return std::string(prefix.value()) + ":" + node->getTagName();
        }
        case Node::XPathType::ATTRIBUTE: {
            return static_cast<AttributeViewNode*>(node)
                ->getReferencedAttribute()
                .getName();
        }
        default:
            return localName(obj);
    }
}

std::string localName(const XPathObject& obj) {
    const std::vector<Node*>& arg = obj.asNodeset();
    if (arg.empty()) return "";

    Node* node = arg[0];

    switch (node->getXPathType()) {
        case Node::XPathType::ELEMENT: {
            return node->getTagName();
        }
        case Node::XPathType::ATTRIBUTE: {
            return std::string(static_cast<AttributeViewNode*>(node)
                                   ->getReferencedAttribute()
                                   .getNCNameWithoutNamespace());
        }
        case Node::XPathType::NAMESPACE: {
            return std::string(
                static_cast<NamespaceViewNode*>(node)->getPrefix());
        }
        case Node::XPathType::PROCESSING_INSTRUCTION: {
            return static_cast<tags::ProcessingInstruction*>(node)->getTarget();
        }
        default:
            return "";
    }
}
};  // namespace onyx::dynamic::xpath::functions