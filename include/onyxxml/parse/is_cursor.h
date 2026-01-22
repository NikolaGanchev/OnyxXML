#pragma once

#include <concepts>

template <typename T>
concept isCursor = requires(T t, int i) {
    typename T::StringType;
    { 
        t.peek(i) 
    } -> std::same_as<char>;
    { 
        *t
    } -> std::same_as<char>;
    { 
        &t 
    } -> std::same_as<char>;
    { 
        t.capturePeek(i)
    } -> std::same_as<char>;
    t.getCaptured();
    t++;
    ++t;
    t.advance(i);
    t.captureAdvance(i);
    t.bringToCapture();
    t.swapDefault();
    t.beginCapture();
};