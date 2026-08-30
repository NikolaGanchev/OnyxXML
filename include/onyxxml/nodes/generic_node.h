#pragma once

#include "../namespace_node.h"

namespace onyx::dynamic::tags {

/**
 * @brief A Node which can is given its namespace, tag name and whether it is
 * void or not at construction. Used when concrete classes aren't
 * available/convenient. Incurs a memory size penalty compared to concrete
 * classes.
 *
 */
class GenericNode : public NamespaceNode {
   private:
    /**
     * @brief The tag name of the Node.
     *
     */
    std::string tag;

   public:
    /**
     * @brief Construct a new owning GenericNode.
     *
     * @tparam Args
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     * @param args Forwarded to the Node constructor
     */
    template <typename... Args>
    explicit GenericNode(std::string namespacePrefix, std::string tagName,
                         bool isVoid, Args&&... args);

    /**
     * @brief Construct a new owning GenericNode.
     *
     * @tparam Args
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     * @param args Forwarded to the Node constructor
     */
    template <typename... Args>
    explicit GenericNode(std::string tagName, bool isVoid, Args&&... args);

    /**
     * @brief Construct an empty owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     */
    explicit GenericNode(std::string namespacePrefix, std::string tagName,
                         bool isVoid);

    /**
     * @brief Construct an empty owning GenericNode object
     *
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     */
    explicit GenericNode(std::string tagName, bool isVoid);

    /**
     * @brief Construct a fully runtime owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(std::string namespacePrefix, std::string tagName,
                         bool isVoid, std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct a fully runtime owning GenericNode object
     *
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(std::string tagName, bool isVoid,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct an empty non-owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     */
    explicit GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, bool isVoid);

    /**
     * @brief Construct an empty non-owning GenericNode object
     *
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     */
    explicit GenericNode(NonOwningNodeTag, std::string tagName, bool isVoid);

    /**
     * @brief Construct a fully runtime non-owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, bool isVoid,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct a fully runtime non-owning GenericNode object
     *
     * @param tagName The tag name of this node
     * @param isVoid Whether this Node is void
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(NonOwningNodeTag, std::string tagName, bool isVoid,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct a new GenericNode object from a Node object being
     * moved.
     *
     * @param other
     */
    explicit GenericNode(Node&& other) noexcept;

    /**
     * @brief GenericNode move assignment from a Node
     *
     * @param other
     * @return GenericNode&
     */
    GenericNode& operator=(Node&& other) noexcept;
    const std::string& getTagName() const override;
    bool isVoid() const override;
    std::unique_ptr<Node> shallowCopy() const override;

    /**
     * @brief Contains the bit indices this class claims ownership over
     *
     */
    enum FlagBitIndices : std::size_t {
        /**
         * @brief Whether the Node is void.
         *
         */
        BIT_IS_VOID = Node::FlagBitIndices::NEXT_BIT,
        NEXT_BIT
    };
};

static_assert(GenericNode::FlagBitIndices::NEXT_BIT <= Node::maxFlagBits(),
              "GenericNode flags bit overflow");

template <typename... Args>
GenericNode::GenericNode(std::string namespacePrefix, std::string tagName,
                         bool isVoid, Args&&... args)
    : tag{std::move(tagName)},
      NamespaceNode(std::move(namespacePrefix), std::forward<Args>(args)...) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(isVoid);

    if (this->isVoid() && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void " + getTagName() +
                                 " cannot have children.");
    }
}

template <typename... Args>
GenericNode::GenericNode(std::string tagName, bool isVoid, Args&&... args)
    : GenericNode("", std::move(tagName), isVoid, std::forward<Args>(args)...) {
}
}  // namespace onyx::dynamic::tags