#pragma once

#include <string_view>

namespace onyx::dynamic::parser {
template <typename T>
concept isCursor = requires(T t, int i, std::string_view expected) {
    typename T::StringType;
    { t.peek(i) } -> std::same_as<char>;
    { t.current() } -> std::same_as<char>;
    { t.captureCurrent() } -> std::same_as<char>;
    { t.capturePeek(i) } -> std::same_as<char>;
    { t.consumeIfMatches(expected) } -> std::same_as<bool>;
    { t.isEOF() } -> std::same_as<bool>;
    t.getCaptured();
    t.advance(i);
    t.captureAdvance(i);
    t.bringToCapture();
    t.swapDefault();
    t.beginCapture();
};
}  // namespace onyx::dynamic::parser