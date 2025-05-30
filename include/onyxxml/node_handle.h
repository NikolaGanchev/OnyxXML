#pragma once
#include <memory>
#include <stdexcept>

namespace Templater::dynamic {
class Node;

/**
 * @brief A potentially owning Handle of a Node.
 *
 */
class NodeHandle {
   private:
    /**
     * @brief Pointer to Node.
     *
     */
    Node* pointer;

    /**
     * @brief Whether the NodeHandle owns the Node or not
     *
     */
    bool isOwning;

    /**
     * @brief If this is owning, deletes the pointer. Otherwise, the pointer is
     * set to nullptr
     *
     */
    void reset();

   public:
    /**
     * @brief Construct a new Node Handle object using a unique_ptr. The
     * constructed NodeHandle is owning.
     *
     * @param ptr The unique_ptr holding the Node
     */
    NodeHandle(std::unique_ptr<Node> ptr);

    /**
     * @brief Construct a new Node Handle object using a raw pointer. The
     * constructed NodeHandle is non-owning by default. This behaviour can be
     * changed via the owns property.
     *
     * @param raw
     * @param owns
     */
    NodeHandle(Node* raw, bool owns = false);

    /**
     * @brief Move constructor.
     *
     * @param o
     */
    NodeHandle(NodeHandle&& o) noexcept;

    /**
     * @brief Move assignment operator.
     *
     * @param o
     * @return NodeHandle&
     */
    NodeHandle& operator=(NodeHandle&& o) noexcept;

    /**
     * @brief Destroy the Node Handle object. Releases memory if owning.
     *
     */
    ~NodeHandle();

    NodeHandle(const NodeHandle&) = delete;
    NodeHandle& operator=(const NodeHandle&) = delete;

    /**
     * @brief Constructs a unique_ptr from this and sets owning to false if and
     * only if this is owning. Otherwise throws an std::logic_error.
     *
     * @return std::unique_ptr<Node>
     */
    std::unique_ptr<Node> toUnique();

    /**
     * @brief Returns the underlying pointer as non-const.
     *
     * @return Node*
     */
    Node* get();

    /**
     * @brief Returns the underlying pointer as const.
     *
     * @return const Node*
     */
    const Node* get() const;

    /**
     * @brief Dereference operator.
     *
     * @return Node&
     */
    Node& operator*();

    /**
     * @brief Pointer access operator.
     *
     * @return Node*
     */
    Node* operator->();

    /**
     * @brief Releases the underlying pointer no matter if owning or not. Sets
     * the underlying pointer to nullptr and ownership to non-owning.
     *
     * @return Node*
     */
    Node* release();

    /**
     * @brief Returns whether this NodeHandle is owning.
     *
     * @return true
     * @return false
     */
    bool owning();

    /**
     * @brief Returns whether this NodeHandle has nullptr
     *
     * @return true
     * @return false
     */
    operator bool() const;
};
}  // namespace Templater::dynamic