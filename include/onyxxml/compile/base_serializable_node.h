#pragma once

#include <string_view>

#include "../nodes/generic_node.h"
#include "compile_string.h"
#include "document_utils.h"

namespace onyx::compile::ctags {

template <CompileString Name, bool isVoid, typename... Children>
struct BaseSerializableNode {
    /**
     * @brief The compile-time size of the Node string. Does not account
     * for '\0'
     *
     * @return std::size_t
     */
    static consteval std::size_t size() {
        size_t size = isVoid ? (std::string_view(Name.value).size() + 4)
                             : (2 * std::string_view(Name.value).size() + 5);
        ((size += Children::size()), ...);
        return size;
    }

    /**
     * @brief The count of placeholders in the subtree.
     *
     * @return std::size_t
     */
    static consteval std::size_t placeholderCount() {
        std::size_t count = 0;
        ((count += Children::placeholderCount()), ...);
        return count;
    }

    constexpr static std::size_t computedSize = size();
    constexpr static std::size_t computedPlaceholderCount = placeholderCount();

    /**
     * @brief The compile-time evaluation of this Node. Does not account for
     * '\0';
     *
     */
    static consteval EvaluatedDocument<computedSize + 1,
                                       computedPlaceholderCount>
    serialize() {
        EvaluatedDocument<computedSize + 1, computedPlaceholderCount> result =
            {};
        evaluate<computedSize + 1, computedPlaceholderCount, Children...>(
            result, Name.value);
        return result;
    }

    /**
     * @brief Evaluates this Attribute into an existing EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        if constexpr (isVoid) {
            return DocumentUtils::evaluateVoidNode<
                ContentSize, PlaceholderCount, Children...>(result, Name.value);
        } else {
            return DocumentUtils::evaluateNode<ContentSize, PlaceholderCount,
                                               Children...>(result, Name.value);
        }
    }
};
}  // namespace onyx::compile::ctags