#pragma once

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "node.h"

namespace onyx::dynamic {

/**
 * @brief A dynamic PagedArena that manages a heterogenous heap-allocated list
 * of non-owning nodes. Grows dynamically.
 *
 * Example usage:
 * @code{.cpp}
 * PagedArena arena(4096); // Optional page size
 *
 * Node* parent = arena.allocate<GenericNode>("html", false);
 * Node* child = arena.allocate<Text>("hello world");
 *
 * parent->addChild(child);
 * @{endcode}
 */
class PagedArena {
   private:
    /**
     * @brief Represents a single block of memory.
     */
    struct Page {
        std::unique_ptr<char[]> buffer;
        size_t capacity;
        size_t position;

        /**
         * @brief Construct a new Page object
         *
         * @param size
         */
        Page(size_t size);

        /**
         * @brief Checks if the Page has enough capacity to allocate with the
         * given size and alignment
         *
         * @param size
         * @param alignment
         * @return true
         * @return false
         */
        bool canAllocate(size_t size, size_t alignment) const;

        /**
         * @brief Allocates a portion of the buffer.
         *
         * @param size
         * @param alignment
         * @return char*
         */
        char* allocateRaw(size_t size, size_t alignment);
    };

    /**
     * @brief Collection of memory pages.
     */
    std::vector<std::unique_ptr<Page>> pages;

    /**
     * @brief Tracks allocated Nodes for destruction.
     */
    std::vector<Node*> allocations;

    /**
     * @brief The default size for new pages.
     */
    size_t pageSize;

    /**
     * @brief Pointer to the active page.
     */
    Page* currentPage;

    /**
     * @brief Destroys all nodes and pages.
     */
    void destroy();

    /**
     * @brief Moves other into this.
     */
    void move(PagedArena&& other);

    /**
     * @brief allocaties a new page of at least neededSize.
     */
    void expand(size_t neededSize);

   public:
    /**
     * @brief Construct a new PagedArena object.
     * * @param pageSize The size of memory blocks to allocate. Defaults to
     * 16KB.
     */
    PagedArena(size_t pageSize = 16384);

    /**
     * @brief Move constructor
     *
     * @param other
     */
    PagedArena(PagedArena&& other);

    /**
     * @brief Move assignment
     *
     * @param other
     * @return PagedArena&
     */
    PagedArena& operator=(PagedArena&& other);

    PagedArena(const PagedArena&) = delete;
    PagedArena& operator=(const PagedArena&) = delete;

    /**
     * @brief Destroy the PagedArena object
     *
     */
    ~PagedArena();

    /**
     * @brief Allocates an object of the given type T with the given arguments.
     * Automatically expands memory if the current page is full.
     *
     * @tparam T The Node type
     * @tparam Args Constructor arguments
     */
    template <typename T, typename... Args>
    T* allocate(Args&&... args)
        requires(isNode<T>);
};

template <typename T, typename... Args>
T* PagedArena::allocate(Args&&... args)
    requires(isNode<T>)
{
    size_t alignment = alignof(T);
    size_t size = sizeof(T);

    if (!currentPage || !currentPage->canAllocate(size, alignment)) {
        size_t required = size + alignment;
        expand(std::max(this->pageSize, required));
    }

    char* memory = currentPage->allocateRaw(size, alignment);

    T* obj = new (memory) T(NonOwning, std::forward<Args>(args)...);

    allocations.push_back(obj);

    return obj;
}
}  // namespace onyx::dynamic