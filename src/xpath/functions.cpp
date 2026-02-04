#include "xpath/functions.h"

#include <array>
#include <charconv>
#include <limits>
#include <string>
#include <variant>

#include "parse/helpers.h"

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

    if (std::isnan(start) || std::isnan(limit)) {
        return "";
    }

    double lower = std::max(1.0, start);
    double upper = std::min(static_cast<double>(str.length()) + 1.0, limit);

    if (lower >= upper) {
        return "";
    }

    size_t startIndex = static_cast<size_t>(lower - 1.0);
    size_t count = static_cast<size_t>(upper - lower);

    return str.substr(startIndex, count);
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

    return str1.substr(i + 1);
}

std::string translate(const std::string& str1, const std::string& str2,
                      const std::string& str3) {
    std::stringstream result;

    // Values
    // -1: Character not found in str2
    // -2: Character found in str2 but no corresponding char in str3
    // 0..255: The replacement character
    std::vector<int> map(256, -1);

    for (size_t i = 0; i < str2.length(); ++i) {
        unsigned char key = static_cast<unsigned char>(str2[i]);

        // XPath spec: "If a character occurs more than once in the second
        // argument string, then the first occurrence determines the replacement
        // character." We only set the rule if this character hasn't been
        // processed yet
        if (map[key] == -1) {
            if (i < str3.length()) {
                map[key] = static_cast<unsigned char>(str3[i]);
            } else {
                map[key] = -2;
            }
        }
    }

    for (char c : str1) {
        unsigned char index = static_cast<unsigned char>(c);
        int action = map[index];

        if (action == -1) {
            result << c;
        } else if (action == -2) {
        } else {
            result << static_cast<char>(action);
        }
    }

    return result.str();
}

std::string normalizeSpace(const std::string& str) {
    std::string res;

    parser::StringCursor pos(str.data());

    while (*pos != '\0' && parser::isWhitespace(*pos)) pos++;

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
};  // namespace onyx::dynamic::xpath::functions