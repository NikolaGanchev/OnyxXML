#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/doctype_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"

namespace onyx::compile::ctags {
/**
 * @brief A compile time DOCTYPE struct.
 *
 * @tparam Str
 */
template <CompileString Str>
struct DOCTYPE {
    /**
     * @brief The compile-time size of the DOCTYPE string. Does not account for
     * '\0'
     *
     * @return size_t
     */
    static consteval size_t size() {
        return std::string_view(Str.value).size() + 10 +
               1;  // added size for '<!DOCTYPE ' (10) and > (1)
    }

    /**
     * @brief The DOCTYPE string; evaluated at compile-time. Does not do any
     * escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval std::array<char, size() + 1> serialize() {
        std::array<char, size() + 1> result = {};
        size_t index =
            CompileStringUtils::placeStringInArray(result, "<!DOCTYPE ", 0);
        index =
            CompileStringUtils::placeStringInArray(result, Str.value, index);
        index = CompileStringUtils::placeStringInArray(result, ">", index);
        index = CompileStringUtils::placeStringInArray(result, "\0", index);
        return result;
    }

    /**
     * @brief Construct a dynamic Doctype Node from a compile-time DOCTYPE
     * struct.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        return std::make_unique<onyx::dynamic::tags::Doctype>(Str);
    }
};
}  // namespace onyx::compile::ctags