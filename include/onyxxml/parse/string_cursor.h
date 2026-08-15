#pragma once
#include <string_view>

namespace onyx::dynamic::parser {
/**
 * @brief A minimal cursor built upon a string. Supports capture operations.
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
     * @brief The end of the string
     *
     */
    const char* end;

    /**
     * @brief Construct a new StringCursor object
     *
     * @param p
     * @param encoding The encoding of the string p points towards
     */
    StringCursor(std::string_view str)
        : ptr(str.data()), captured(str.data()), end(str.data() + str.size()) {}

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
    char current() const { return *ptr; };

    /**
     * @brief Return the current capture char
     *
     * @return char
     */
    char captureCurrent() { return *captured; };

    /**
     * @brief Advances the cursor. Does not check for bounds.
     *
     * @param toAdvance
     */
    void advance(int toAdvance = 1) { ptr += toAdvance; }

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
    void captureAdvance(int toAdvance = 1) { captured += toAdvance; }

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

    /**
     * @brief Checks if the upcoming characters match the expected string.
     * If they do, move the cursor past them and return true.
     *
     * @param expected
     * @return true
     * @return false
     */
    bool consumeIfMatches(std::string_view expected) {
        if (static_cast<size_t>(end - ptr) < expected.size()) {
            return false;
        }

        for (size_t i = 0; i < expected.size(); i++) {
            if (ptr[i] != expected[i]) {
                return false;
            }
        }

        ptr += expected.size();
        return true;
    }

    bool isEOF() const { return ptr >= end; }
};
}  // namespace onyx::dynamic::parser
//  onyx::dynamic::parser