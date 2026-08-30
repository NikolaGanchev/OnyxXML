#pragma once

#include <string_view>

#include "node.h"

namespace onyx::dynamic {

/**
 * @brief A Node which can be given its namespace prefix at construction.
 *
 */
class NamespaceNode : public Node {
   private:
    /**
     * @brief The namespace prefix of the Node.
     *
     */
    std::string namespacePrefix;

   public:
    /**
     * @brief Construct a new owning NamespaceNode.
     *
     * @tparam Args
     * @param namespacePrefix The namespace prefix of this Node
     * @param args Forwarded to the Node constructor
     */
    template <typename... Args>
    explicit NamespaceNode(std::string namespacePrefix, Args&&... args);

    /**
     * @brief Construct an empty owning NamespaceNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     */
    explicit NamespaceNode(std::string namespacePrefix);

    /**
     * @brief Construct a fully runtime owning NamespaceNode object
     *
     * @param namespacePrefix The namespace prefix of this Node]
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit NamespaceNode(std::string namespacePrefix,
                           std::vector<Attribute> attributes,
                           std::vector<NodeHandle>&& children);

    /**
     * @brief Construct an empty non-owning NamespaceNode object
     *
     * @param namespacePrefix The namespace prefix of this Node]
     */
    explicit NamespaceNode(NonOwningNodeTag, std::string namespacePrefix);

    /**
     * @brief Construct a fully runtime non-owning NamespaceNode object
     *
     * @param namespacePrefix The namespace prefix of this Node]
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit NamespaceNode(NonOwningNodeTag, std::string namespacePrefix,
                           std::vector<Attribute> attributes,
                           std::vector<NodeHandle>&& children);

    /**
     * @brief Construct a new NamespaceNode object from a Node object being
     * moved.
     *
     * @param other
     */
    explicit NamespaceNode(Node&& other) noexcept;

    /**
     * @brief NamespaceNode move assignment from a Node
     *
     * @param other
     * @return NamespaceNode&
     */
    NamespaceNode& operator=(Node&& other) noexcept;

    virtual std::optional<std::string_view> getNamespacePrefix() const override;
};

template <typename... Args>
NamespaceNode::NamespaceNode(std::string namespacePrefix, Args&&... args)
    : namespacePrefix{std::move(namespacePrefix)},
      Node(std::forward<Args>(args)...) {}
}  // namespace onyx::dynamic