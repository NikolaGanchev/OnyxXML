#pragma once
#include <utility>

#include "onyx.h"

template <std::size_t N, typename F>
constexpr void forConstexpr(F&& f) {
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        (f.template operator()<Is>(), ...);
    }(std::make_index_sequence<N>{});
}

class IndexFixture : public onyx::dynamic::Node::Index {
   public:
    std::size_t putCalledCount = 0;
    std::size_t removeCalledCount = 0;
    std::size_t updateCalledCount = 0;

    ADD_INDEX_MOVE_OPERATIONS(public, IndexFixture,
                              &IndexFixture::updateCalledCount,
                              &IndexFixture::putCalledCount,
                              &IndexFixture::removeCalledCount);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;

   protected:
    bool putIfNeeded(onyx::dynamic::Node* node) override {
        putCalledCount++;
        return true;
    }
    bool removeIfNeeded(onyx::dynamic::Node* node) override {
        removeCalledCount++;
        return true;
    }
    bool update(onyx::dynamic::Node* node) override {
        updateCalledCount++;
        return true;
    }

    using onyx::dynamic::Node::Index::Index;
};