#include <string>
#include <variant>
#include <limits>

#include "xpath/functions.h"
#include <charconv>

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

                // std::chars_format::fixed with no precision specified automatically
                // produces the shortest representation that uniquely distinguishes the number.
                auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), arg, std::chars_format::fixed);

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
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return ch != ' ';
    }));

    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return ch != ' ';
    }).base(), str.end());
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
                auto [ptr, ec] = std::from_chars(copy.data(), copy.data() + copy.size(), result, std::chars_format::fixed);
 
                if (ptr != copy.data() + copy.size()) {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                return result;
            }

            if constexpr (std::is_same_v<T, std::vector<Node*>>) {
                std::string strVal = string(obj); 

                trim(strVal);
            
                double result = std::numeric_limits<double>::quiet_NaN();
                auto [ptr, ec] = std::from_chars(strVal.data(), strVal.data() + strVal.size(), result, std::chars_format::fixed);

                if (ptr != strVal.data() + strVal.size()) {
                    return std::numeric_limits<double>::quiet_NaN();
                }

                return result;
            }
        },
        obj.value);
};
};  // namespace onyx::dynamic::xpath::functions