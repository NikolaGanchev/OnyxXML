#pragma once

#include "namespace_node.h"
#include "node.h"

namespace onyx::dynamic {

/**
 * @brief A Namespace Node which is void by default. Provides constructors that
 * don't accept children. All Namespace void nodes should derive from the
 * NamespaceVoidNode base instead of Node. Although NamespaceVoidNode cannot
 * have children, it can still be non-owning to upkeep tree integrity.
 *
 */
class VoidNamespaceNode : public NamespaceNode {
   public:
    /**
     * @brief Constructs an owning Node using the provided rvalue referenced
     * Attributes
     *
     */
    template <typename... Args>
    explicit VoidNamespaceNode(std::string namespacePrefix, Args&&... args)
        requires(isAttribute<Args> && ...);

    /**
     * @brief Constructs a non-owning Node using the provided rvalue referenced
     * Attributes
     *
     */
    template <typename... Args>
    explicit VoidNamespaceNode(NonOwningNodeTag, std::string namespacePrefix,
                               Args&&... args)
        requires(isAttribute<Args> && ...);

    /**
     * @brief Construct a new owning NamespaceVoidNode from the attributes
     * vector.
     *
     * @param attributes
     */
    explicit VoidNamespaceNode(std::string namespacePrefix,
                               std::vector<Attribute> attributes);

    /**
     * @brief Construct a new non-owning NamespaceVoidNode from the attributes
     * vector.
     *
     * @param attributes
     */
    explicit VoidNamespaceNode(NonOwningNodeTag, std::string namespacePrefix,
                               std::vector<Attribute> attributes);

    bool isVoid() const override;
};

template <typename... Args>
VoidNamespaceNode::VoidNamespaceNode(std::string namespacePrefix,
                                     Args&&... args)
    requires(isAttribute<Args> && ...)
    : NamespaceNode(std::move(namespacePrefix), std::forward<Args>(args)...) {}

template <typename... Args>
VoidNamespaceNode::VoidNamespaceNode(NonOwningNodeTag,
                                     std::string namespacePrefix,
                                     Args&&... args)
    requires(isAttribute<Args> && ...)
    : NamespaceNode(NonOwning, std::move(namespacePrefix),
                    std::forward<Args>(args)...) {}
}  // namespace onyx::dynamic