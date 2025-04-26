#include "cache_index.h"

namespace Templater::dynamic::index {
    CacheIndex::CacheIndex(Node* root)
        : Index(root), _cache{} {}; 

    bool CacheIndex::putIfNeeded(Node* node) {
        _cache.clear();
        return true;
    }

    bool CacheIndex::removeIfNeeded(Node* node) {
        _cache.clear();
        return true;
    }

    bool CacheIndex::update(Node* node) {
        _cache.clear();
        return true;
    }

    CacheIndex::CacheIndex(CacheIndex&& other)
        : Index{std::move(other)}, _cache{std::move(other._cache)} {}

    CacheIndex& CacheIndex::operator=(CacheIndex&& other) {
        this->_assign_index_base(std::move(other));
        this->_cache = std::move(other._cache);
        return *this;
    }
}