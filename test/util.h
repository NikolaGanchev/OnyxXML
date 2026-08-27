#pragma once
#include <utility>

template <std::size_t N, typename F>
constexpr void forConstexpr(F&& f) {
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        (f.template operator()<Is>(), ...);
    }(std::make_index_sequence<N>{});
}