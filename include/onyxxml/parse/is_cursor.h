#pragma once

#include <concepts>

template <typename T>
concept isCursor = requires(T t) {
    {
        *t
    } -> std::same_as<char>;
    {
        t++
    } -> std::same_as<void>;
    {
        t.peek()
    } -> std::same_as<char>;
    {
        t.isEOF()
    } -> std::same_as<bool>;
};