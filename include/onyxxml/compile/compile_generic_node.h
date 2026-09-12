#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/generic_node.h"
#include "compile_base_serializable_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"
#include "document_utils.h"

namespace onyx::compile::ctags {

/**
 * @brief A compile-time GenericNode which can represent any tag
 *
 */
template <CompileString Name, bool isVoid, typename... Children>
struct GenericNode : BaseSerializableNode<Name, isVoid, Children...> {
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        std::unique_ptr<onyx::dynamic::tags::GenericNode> node =
            std::make_unique<onyx::dynamic::tags::GenericNode>(
                Name.value,
                isVoid ? onyx::dynamic::tags::GenericNode::Type::Void
                       : onyx::dynamic::tags::GenericNode::Type::NonVoid);
        (DocumentUtils::parseChildren<Children>(node.get()), ...);
        return node;
    }
};
}  // namespace onyx::compile::ctags