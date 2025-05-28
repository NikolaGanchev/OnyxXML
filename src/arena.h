#pragma once

#include <vector>

#include "node.h"

namespace Templater::dynamic {
/**
 * @brief An Arena manages a heterogenous continuous heap-allocated array of
 * non-owning nodes. The size of the arena must be precomputed and given on
 * construction. The Arena::Builder class can simplify that task. The Arena
 * fully manages the memory of the Nodes allocated by it, including deletion
 * (which happens at destruction of the Arena). Any attempt to manage the memory
 * of the nodes provided by an Arena, such as deleting them, will most likely
 * end in an exception. Example usage:
 *
 * @code{.cpp}
 * Arena::Builder builder;
 * builder.preallocate<GenericNode>()
 *         .preallocate<Text>();
 *
 * Arena arena = builder.build();
 *
 * Node* parent = arena.allocate<GenericNode>("html", false);
 * Node* child = arena.allocate<Text>("hello world");
 *
 * parent->addChild(child);
 * // The nodes will be automatically released at the end of the scope
 * @{endcode}
 */
class Arena {
   private:
    /**
     * @brief The internal memory buffer.
     *
     */
    char* buffer;

    /**
     * @brief The capacity of the Arena.
     *
     */
    size_t capacity;

    /**
     * @brief The current offset from the beginning of the buffer.
     *
     */
    size_t position;

    /**
     * @brief Tracks allocated Nodes in the buffer for destruction.
     *
     */
    std::vector<Node*> allocations;

    /**
     * @brief Frees the allocated memory, first by manually calling the
     * destructors of all allocated nodes and then releasing the buffer,
     *
     */
    void destroy();

    /**
     * @brief Moves other into this.
     *
     * @param other
     */
    void move(Arena&& other);

   public:
    /**
     * @brief A class used to precalculate the memory needed by an Arena.
     *
     */
    class Builder {
       private:
        /**
         * @brief Current computed size.
         *
         */
        size_t size;

       public:
        /**
         * @brief Construct a new Builder object.
         *
         */
        Builder();

        /**
         * @brief Calculates the memory footprint of the parameter type,
         * including alignment. Uses alignof(T) and rounds up to the nearest
         * possible size.
         *
         * @tparam T
         */
        template <typename T>
        Builder& preallocate()
            requires(isNode<T>);

        /**
         * @brief Returns the current size of all preallocated nodes, including
         * alignment.
         *
         * @return size_t
         */
        size_t totalSize() const;

        /**
         * @brief Constructs an Arena with the current size.
         *
         * @return Arena
         */
        Arena build() const;
    };

    /**
     * @brief Construct a new Arena object via precomputed capacity.
     *
     * @param capacity
     */
    Arena(size_t capacity);

    /**
     * @brief Construct a new Arena object via move.
     *
     * @param other
     */
    Arena(Arena&& other);

    /**
     * @brief Move an Arena object into this.
     *
     * @param other
     * @return Arena&
     */
    Arena& operator=(Arena&& other);

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    /**
     * @brief Allocates an object of the given type T with the given arguments
     * Args on the internal buffer. Throws std::overflow_error if capacity is
     * overstepped.
     *
     * @tparam T
     * @tparam Args
     */
    template <typename T, typename... Args>
    T* allocate(Args... args)
        requires(isNode<T>);

    /**
     * @brief Destroy the Arena object by calling the destructors of all
     * internally allocated nodes and deleting the buffer,
     *
     */
    ~Arena();
};

template <typename T, typename... Args>
T* Arena::allocate(Args... args)
    requires(isNode<T>)
{
    size_t alignment = alignof(T);
    size_t size = sizeof(T);

    size_t aligned = (this->position + alignment - 1) & ~(alignment - 1);

    if (aligned + size > this->capacity) {
        throw std::overflow_error(
            "Trying to allocate more memory in Arena than capacity allows");
    }

    T* obj =
        new (this->buffer + aligned) T(NonOwning, std::forward<Args>(args)...);
    allocations.push_back(obj);
    this->position = aligned + size;

    return obj;
}

template <typename T>
Arena::Builder& Arena::Builder::preallocate()
    requires(isNode<T>)
{
    size_t alignment = alignof(T);
    size_t size = sizeof(T);

    this->size = (this->size + alignment - 1) & ~(alignment - 1);

    this->size += size;

    return *this;
}
}  // namespace Templater::dynamic