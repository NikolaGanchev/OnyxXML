#pragma once

#include <string_view>

namespace onyx::dynamic::parser {
template <typename T>
concept isCursor = requires(T t, int i, std::string_view expected) {
    /**
     * @brief The string type the Cursor returns captures in.
     *
     */
    typename T::StringType;
    /**
     * @brief Return the (current + i)th char past the current one. peek(0)
     * returns the current char.
     *
     */
    { t.peek(i) } -> std::same_as<char>;
    /**
     * @brief Returns the current char.
     *
     */
    { t.current() } -> std::same_as<char>;
    /**
     * @brief Returns the current char of the capture pointer.
     *
     */
    { t.captureCurrent() } -> std::same_as<char>;
    /**
     * @brief Return the (current + i)th char past the capture pointer.
     * capturePeek(0) returns the current capture pointer char.
     *
     */
    { t.capturePeek(i) } -> std::same_as<char>;
    /**
     * @brief Checks if the upcoming characters match the expected string.
     * If they do, move the cursor to the first position past and return true.
     * Otherwise, do not move the cursor and return false.
     *
     */
    { t.consumeIfMatches(expected) } -> std::same_as<bool>;
    /**
     * @brief Whether the (current + i)th char past the current is past the end
     * of the source.
     *
     */
    { t.isEOF(i) } -> std::same_as<bool>;
    /**
     * @brief Return a StringType of all characters in the sequences [current,
     * capture). Capture must be at or ahead of the current pointer.
     *
     */
    t.getCaptured();
    /**
     * @brief Advance the current pointer by i chars.
     *
     */
    t.advance(i);
    /**
     * @brief Advance the capture pointer by i chars.
     *
     */
    t.captureAdvance(i);
    /**
     * @brief Synchronizes the current and capture pointer by bringing the
     * current pointer to the capture pointer.
     *
     */
    t.bringToCapture();
    /**
     * @brief Swap the capture and current pointers.
     *
     */
    t.swapDefault();
    /**
     * @brief Synchronizes the current and capture pointer by bringing the
     * capture pointer to the current pointer.
     *
     */
    t.beginCapture();
};
}  // namespace onyx::dynamic::parser