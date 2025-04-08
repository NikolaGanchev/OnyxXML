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

}