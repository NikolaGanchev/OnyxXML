#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/generic_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"
#include "document_utils.h"

namespace onyx::compile::ctags {

/**
 * @brief A compile-time GenericNode which can represent any tag
 *
 */
template <CompileString Name, bool isVoid, typename... Children>
struct GenericNode {
    /**
     * @brief The compile-time size of the GenericNode string. Does not accout
     * for '\0'
     *
     * @return size_t
     */
    static consteval size_t size() {
        size_t size = isVoid ? (std::string_view(Name.value).size() + 4)
                             : (2 * std::string_view(Name.value).size() + 5);
        ((size += Children::size()), ...);
        return size;
    }

    /**
     * @brief The GenericNode string; evaluated at compile-time. Does not do any
     * escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval std::array<char, size() + 1> serialize() {
        if constexpr (isVoid) {
            return DocumentUtils::serializeVoidNode<size(), Children...>(
                Name.value);
        } else {
            return DocumentUtils::serializeNode<size(), Children...>(
                Name.value);
        }
    }

    /**
     * @brief Construct a dynamic GenericNode from a compile time GenericNode
     * struct.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        std::unique_ptr<onyx::dynamic::tags::GenericNode> node =
            std::make_unique<onyx::dynamic::tags::GenericNode>(Name.value,
                                                               isVoid);
        (DocumentUtils::parseChildren<Children>(node.get()), ...);
        return node;
    }
};
}  // namespace onyx::compile::ctags