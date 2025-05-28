#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/text_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"

namespace Templater::compile::ctags {
/**
 * @brief A compile time Text struct.
 *
 * @tparam Str
 */
template <CompileString Str>
struct Text {
    /**
     * @brief The compile-time size of the Text string. Does not account for
     * '\0';
     *
     * @return size_t
     */
    static consteval size_t size() {
        return std::string_view(Str.value).size();
    }

    /**
     * @brief The Text string; evaluated at compile-time. Does not do any
     * escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval std::array<char, size() + 1> serialize() {
        std::array<char, size() + 1> result = {};
        size_t index =
            CompileStringUtils::placeStringInArray(result, Str.value, 0);
        index = CompileStringUtils::placeStringInArray(result, "\0", index);
        return result;
    }

    /**
     * @brief Construct a dynamic Text Node from a compile time Text struct.
     *
     * @return std::unique_ptr<Templater::dynamic::Node>
     */
    static std::unique_ptr<Templater::dynamic::Node> dynamicTree() {
        return std::make_unique<Templater::dynamic::tags::Text>(Str);
    }
};
}  // namespace Templater::compile::ctags