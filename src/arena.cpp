#include "arena.h"

namespace onyx::dynamic {

Arena::Arena(size_t capacity) : capacity{capacity}, allocations{} {
    this->buffer = new char[capacity];
    this->position = 0;
}

Arena::Arena(Arena&& other) { move(std::move(other)); }

Arena& Arena::operator=(Arena&& other) {
    destroy();
    move(std::move(other));
    return *this;
}

Arena::~Arena() { this->destroy(); }

void Arena::destroy() {
    for (size_t i = 0; i < this->allocations.size(); i++) {
        this->allocations[i]->~Node();
    }

    delete[] buffer;
}

void Arena::move(Arena&& other) {
    this->buffer = other.buffer;
    other.buffer = nullptr;
    this->capacity = other.capacity;
    other.capacity = 0;
    this->position = other.position;
    other.position = 0;
    this->allocations = std::move(other.allocations);
}

Arena::Builder::Builder() : size(0) {};

size_t Arena::Builder::totalSize() const { return this->size; }

Arena Arena::Builder::build() const { return Arena(this->size); }
}  // namespace onyx::dynamic