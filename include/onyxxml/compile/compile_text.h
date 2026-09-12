#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/text_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"

namespace onyx::compile::ctags {
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

    static consteval std::size_t placeholderCount() { return 0; }

    /**
     * @brief The Text string; evaluated at compile-time. Does not do any
     * escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval EvaluatedDocument<size() + 1, 0> serialize() {
        EvaluatedDocument<size() + 1, 0> result = {};
        evaluate(result);
        return result;
    }

    /**
     * @brief Evaluates the Text into an existing EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        CompileStringUtils::placeStringInEvaluatedDocument(result, Str.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, "\0");
    }

    /**
     * @brief Construct a dynamic Text Node from a compile time Text struct.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        return std::make_unique<onyx::dynamic::tags::Text>(Str);
    }
};
}  // namespace onyx::compile::ctags