#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string_view>
#include <vector>

namespace onyx::dynamic::parser {
/**
 * @brief A cursor built upon an std::istream
 *
 */
struct StreamCursor {
    using StringType = std::string;

    /**
     * @brief The raw buffer of the stream
     *
     */
    std::streambuf* buf;

    /**
     * @brief A buffer for holding captured characters
     *
     */
    std::vector<char> buffer;

    /**
     * @brief The pointer to the position in the buffer
     *
     */
    size_t pos;

    /**
     * @brief The position of the captured index
     *
     */
    size_t captured;

    /**
     * @brief Specifies what is the maximum byte value at which
     * the internal buffer must make space by erasing old data. To facilitate
     * erases, the buffer may hold twice that number of bytes.
     */
    size_t bufferThreshold;

    /**
     * @brief Construct a new StreamCursor object
     *
     * @param is The input stream to wrap
     * @param bufferThreshold The threshold for clearing old data
     */
    StreamCursor(std::istream& is, size_t bufferThreshold = 4096)
        : pos(0),
          captured(0),
          buf(is.rdbuf()),
          bufferThreshold(bufferThreshold) {
        buffer.reserve(bufferThreshold * 2);
    }

    /**
     * @brief Fills the internal buffer to the index
     *
     * @param index
     * @return true
     * @return false If the stream is exhausted
     */
    bool fillTo(size_t index) {
        while (buffer.size() <= index) {
            int c = buf->sbumpc();
            if (c == std::char_traits<char>::eof()) return false;
            buffer.push_back(static_cast<char>(c));
        }
        return true;
    }

    /**
     * @brief Checks whether the cursor is at (or past) the end of the
     * stream.
     *
     * @return true if there is no character available at the current
     * position
     * @return false otherwise
     */
    bool isEOF() { return !fillTo(pos); }

    /**
     * @brief Get the character at the index
     *
     * @param index
     * @return char
     */
    char getAt(size_t index) {
        if (!fillTo(index)) return '\0';
        return buffer[index];
    }

    /**
     * @brief Look ahead relative to pos
     *
     * @param offset
     * @return char
     */
    char peek(int offset) { return getAt(pos + offset); }

    /**
     * @brief Return the char at pos
     *
     * @return char
     */
    char current() { return getAt(pos); };

    /**
     * @brief Return the char at captured
     *
     * @return char
     */
    char captureCurrent() { return getAt(captured); };

    /**
     * @brief Advances the pos.
     *
     * @param toAdvance
     */
    void advance(int toAdvance = 1) { pos += toAdvance; }

    /**
     * @brief Look ahead relative to captured
     *
     * @param offset
     * @return char
     */
    char capturePeek(int offset) { return getAt(captured + offset); }

    /**
     * @brief Advances the captured cursor
     *
     * @param toAdvance
     */
    void captureAdvance(int toAdvance = 1) { captured += toAdvance; }

    /**
     * @brief Brings ptr to the captured position and clears consumed buffer
     *
     */
    void bringToCapture() {
        pos = captured;
        if (pos >= bufferThreshold) {
            size_t remaining = buffer.size() - pos;

            if (remaining > 0) {
                std::memmove(buffer.data(), buffer.data() + pos, remaining);
            }
            buffer.resize(remaining);
            captured = 0;
            pos = 0;
        }
    }

    /**
     * @brief Brings captured to the ptr position.
     *
     */
    void beginCapture() { captured = pos; }

    /**
     * @brief Swaps the positions of pos and captured
     *
     */
    void swapDefault() { std::swap(pos, captured); }

    /**
     * @brief Get an std::string of characters between ptr and captured
     *
     * @return std::string
     */
    std::string getCaptured() {
        if (captured <= pos) return "";
        fillTo(captured - 1);
        return std::string(buffer.begin() + pos, buffer.begin() + captured);
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
        if (expected.empty()) return true;
        if (!fillTo(pos + expected.size() - 1)) return false;

        if (std::string_view(buffer.data() + pos, expected.size()) ==
            expected) {
            pos += expected.size();
            return true;
        }
        return false;
    }
};
}  // namespace onyx::dynamic::parser