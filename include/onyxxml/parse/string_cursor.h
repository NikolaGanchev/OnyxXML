#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace onyx::dynamic::parser {
/**
 * @brief A minimal cursor built upon a string. Supports capture operations.
 * Mixing normal and capture operations is not supported.
 *
 */
struct StringCursor {
    using StringType = std::string_view;
    /**
     * @brief The current position in the string
     *
     */
    const char* ptr;
    /**
     * @brief A position ahead in the string
     *
     */
    const char* captured;

    /**
     * @brief Construct a new StringCursor object
     *
     * @param p
     */
    StringCursor(const char* p) : ptr(p), captured(p) {}

    /**
     * @brief Look ahead in the cursor. Does not check for bounds.
     *
     * @param offset
     * @return char
     */
    char peek(int offset) const { return *(ptr + offset); }

    /**
     * @brief Return the current char
     *
     * @return char
     */
    char operator*() const { return *ptr; }

    /**
     * @brief Return the current capture char
     *
     * @return char
     */
    char operator&() const { return *captured; }

    /**
     * @brief Postfix increment. Advances the capture by one
     *
     */
    void operator++(int) { ptr++; }

    /**
     * @brief Prefix increment. Advances the cursor by one
     *
     */
    void operator++() { captured++; }

    /**
     * @brief Advances the cursor. Does not check for bounds.
     *
     * @param toAdvance
     */
    void advance(int toAdvance) { ptr += toAdvance; }

    /**
     * @brief Look ahead in the capture. Does not check for bounds.
     *
     * @param offset
     * @return char
     */
    char capturePeek(int offset) const { return *(captured + offset); }

    /**
     * @brief Capture a character. This records it without advancing the cursor.
     * Moves the capture.
     *
     */
    void captureAdvance(int toAdvance) { captured += toAdvance; }

    /**
     * @brief Brings the cursor to the capture position.
     *
     */
    void bringToCapture() { ptr = captured; }

    /**
     * @brief Brings the capture to the cursor position.
     *
     */
    void beginCapture() { captured = ptr; }

    /**
     * @brief Swaps the default cursor to the capture. Useful for calling
     * functions that do not use the capture.
     *
     */
    void swapDefault() { std::swap(ptr, captured); }

    /**
     * @brief Get an std::string_view of all captured characters since the last
     * bringToCapture() call
     *
     * @return std::string_view
     */
    std::string_view getCaptured() const {
        return std::string_view(ptr, captured - ptr);
    }
};
}