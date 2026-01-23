#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string_view>
#include <vector>

namespace onyx::dynamic::parser {
/**
 * @brief A cursor built upon an std::istream
 */
struct StreamCursor {
    using StringType = std::string;

    /**
     * @brief The stream
     *
     */
    std::istream* stream;

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
     * @brief Construct a new StreamCursor object
     *
     * @param is The input stream to wrap
     */
    StreamCursor(std::istream& is)
        : stream(&is), pos(0), captured(0), buf(is.rdbuf()) {}

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
    char operator*() { return getAt(pos); }

    /**
     * @brief Return the char at captured
     *
     * @return char
     */
    char operator&() { return getAt(captured); }

    /**
     * @brief Postfix increment. Advances the pos.
     *
     */
    void operator++(int) { pos++; }

    /**
     * @brief Prefix increment. Advances the captured.
     *
     */
    void operator++() { captured++; }

    /**
     * @brief Advances the pos.
     *
     * @param toAdvance
     */
    void advance(int toAdvance) { pos += toAdvance; }

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
    void captureAdvance(int toAdvance) { captured += toAdvance; }

    /**
     * @brief Brings ptr to the captured position and clears consumed buffer
     *
     */
    void bringToCapture() {
        pos = captured;
        if (pos > 0) {
            buffer.erase(buffer.begin(), buffer.begin() + pos);
            captured -= pos;
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
};
}  // namespace onyx::dynamic::parser