#pragma once

#include "../namespace_node.h"
#include "util/qualified_name.h"

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
     * @brief Represents types of GenericNode
     *
     */
    enum class Type { Void, NonVoid };
    /**
     * @brief Construct a new owning GenericNode.
     *
     * @tparam Args
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param type Void or NonVoid
     * @param args Forwarded to the Node constructor
     */
    template <typename... Args>
    explicit GenericNode(std::string namespacePrefix, std::string tagName,
                         Type type, Args&&... args);

    /**
     * @brief Construct a new owning GenericNode.
     *
     * @tparam Args
     * @param qualifiedName A qualified name that includes the name of the node
     * and may or not include a namespace prefix
     * @param type Void or NonVoid
     * @param args Forwarded to the Node constructor
     */
    template <typename... Args>
    explicit GenericNode(util::QualifiedName qualifiedName, Type type,
                         Args&&... args);

    /**
     * @brief Construct an empty owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param type Void or NonVoid
     */
    explicit GenericNode(std::string namespacePrefix, std::string tagName,
                         Type type);

    /**
     * @brief Construct an empty owning GenericNode object
     *
     * @param qualifiedName A qualified name that includes the name of the node
     * and may or not include a namespace prefix
     * @param type Void or NonVoid
     */
    explicit GenericNode(util::QualifiedName qualifiedName, Type type);

    /**
     * @brief Construct a fully runtime owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param type Void or NonVoid
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(std::string namespacePrefix, std::string tagName,
                         Type type, std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct a fully runtime owning GenericNode object
     *
     * @param qualifiedName A qualified name that includes the name of the node
     * and may or not include a namespace prefix
     * @param type Void or NonVoid
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(util::QualifiedName qualifiedName, Type type,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct an empty non-owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param type Void or NonVoid
     */
    explicit GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, Type type);

    /**
     * @brief Construct an empty non-owning GenericNode object
     *
     * @param qualifiedName A qualified name that includes the name of the node
     * and may or not include a namespace prefix
     * @param type Void or NonVoid
     */
    explicit GenericNode(NonOwningNodeTag, util::QualifiedName qualifiedName,
                         Type type);

    /**
     * @brief Construct a fully runtime non-owning GenericNode object
     *
     * @param namespacePrefix The namespace prefix of this Node
     * @param tagName The tag name of this node
     * @param type Void or NonVoid
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, Type type,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children);

    /**
     * @brief Construct a fully runtime non-owning GenericNode object
     *
     * @param qualifiedName A qualified name that includes the name of the node
     * and may or not include a namespace prefix
     * @param type Void or NonVoid
     * @param attributes Attributes to be forwarded to the Node constructor
     * @param children Children to be forwarded to the Node constructor
     */
    explicit GenericNode(NonOwningNodeTag, util::QualifiedName qualifiedName,
                         Type type, std::vector<Attribute> attributes,
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
                         Type type, Args&&... args)
    : tag{std::move(tagName)},
      NamespaceNode(std::move(namespacePrefix), std::forward<Args>(args)...) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(type == Type::Void);

    if (this->isVoid() && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void " + getTagName() +
                                 " cannot have children.");
    }
}

template <typename... Args>
GenericNode::GenericNode(util::QualifiedName qualifiedName, Type type,
                         Args&&... args)
    : GenericNode(std::string(qualifiedName.prefix),
                  std::string(qualifiedName.name), type,
                  std::forward<Args>(args)...) {}
}  // namespace onyx::dynamic::tags