#include "paged_arena.h"

namespace onyx::dynamic {

PagedArena::Page::Page(size_t size) : capacity(size), position(0) {
    buffer = std::make_unique<char[]>(size);
}

bool PagedArena::Page::canAllocate(size_t size, size_t alignment) const {
    size_t aligned = (position + alignment - 1) & ~(alignment - 1);
    return (aligned + size <= capacity);
}

char* PagedArena::Page::allocateRaw(size_t size, size_t alignment) {
    size_t aligned = (position + alignment - 1) & ~(alignment - 1);
    char* ptr = buffer.get() + aligned;
    position = aligned + size;
    return ptr;
}

PagedArena::PagedArena(size_t pageSize)
    : pageSize(pageSize), currentPage(nullptr) {}

PagedArena::PagedArena(PagedArena&& other)
    : pageSize(4096), currentPage(nullptr) {
    move(std::move(other));
}

PagedArena& PagedArena::operator=(PagedArena&& other) {
    if (this != &other) {
        destroy();
        move(std::move(other));
    }
    return *this;
}

PagedArena::~PagedArena() { this->destroy(); }

void PagedArena::destroy() {
    for (size_t i = 0; i < this->allocations.size(); i++) {
        this->allocations[i]->~Node();
    }

    allocations.clear();
    pages.clear();
    currentPage = nullptr;
}

void PagedArena::move(PagedArena&& other) {
    this->pages = std::move(other.pages);
    this->allocations = std::move(other.allocations);
    this->pageSize = other.pageSize;
    this->currentPage = other.currentPage;

    other.currentPage = nullptr;
}

void PagedArena::expand(size_t neededSize) {
    pages.push_back(std::make_unique<Page>(neededSize));
    currentPage = pages.back().get();
}
}  // namespace onyx::dynamic