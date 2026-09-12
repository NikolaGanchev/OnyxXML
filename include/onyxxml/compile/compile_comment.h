#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/comment_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"
#include "evaluated_document.h"

namespace onyx::compile::ctags {
/**
 * @brief A compile time Comment struct.
 *
 * @tparam Str
 */
template <CompileString Str>
struct Comment {
    /**
     * @brief The compile-time size of the Comment string. Does not account for
     * '\0'
     *
     * @return size_t
     */
    static consteval size_t size() {
        return std::string_view(Str.value).size() + 4 +
               3;  // added size for <!-- (4) and --> (3)
    }

    /**
     * @brief The Comment string; evaluated at compile-time. Does not do any
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
     * @brief Evaluates the Comment into an existing EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        CompileStringUtils::placeStringInEvaluatedDocument(result, "<!--");
        CompileStringUtils::placeStringInEvaluatedDocument(result, Str.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, "-->");
        CompileStringUtils::placeStringInEvaluatedDocument(result, "\0");
    }

    /**
     * @brief Construct a dynamic Comment Node from a compile-time Comment
     * struct.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        return std::make_unique<onyx::dynamic::tags::Comment>(Str);
    }

    static consteval std::size_t placeholderCount() { return 0; }
};
}  // namespace onyx::compile::ctags