#pragma once
#include <algorithm>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

#if ICONV_AVAILABLE
#include <iconv.h>

#include <cerrno>

#endif

namespace onyx::dynamic::parser {
/**
 * @brief A cursor built upon an std::istream
 *
 */
struct StreamCursor {
    using StringType = std::string;

    /**
     * @brief The encoding of the stream
     *
     */
    std::string inputEncoding;

#if ICONV_AVAILABLE
    iconv_t cd;
#endif
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
     * @brief A buffer for holding raw bytes from the stream that haven't been
     * fully transcoded yet
     *
     */
    std::vector<char> rawBuffer;

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
     * @param inputEncoding The encoding of the stream
     * @param bufferThreshold The threshold for clearing old data
     */
    StreamCursor(std::istream& is, size_t bufferThreshold = 4096)
        : pos(0),
          captured(0),
          buf(is.rdbuf()),
          inputEncoding("UTF-8"),
          bufferThreshold(bufferThreshold) {
        buffer.reserve(bufferThreshold * 2);
#if ICONV_AVAILABLE
        this->cd = (iconv_t)-1;
#endif
    }

    ~StreamCursor() {
#if ICONV_AVAILABLE
        if (cd != (iconv_t)-1) {
            iconv_close(cd);
        }
#endif
    }

    StreamCursor(const StreamCursor&) = delete;
    StreamCursor& operator=(const StreamCursor&) = delete;

    /**
     * @brief Fills the internal buffer to the index
     *
     * @param index
     * @return true
     * @return false If the stream is exhausted
     */
    bool fillTo(size_t index) {
        while (buffer.size() <= index) {
            if (!transcoding) {
                int c = buf->sbumpc();
                if (c == std::char_traits<char>::eof()) return false;
                buffer.push_back(static_cast<char>(c));
            } else {
#if ICONV_AVAILABLE
                int c = buf->sbumpc();
                if (c == std::char_traits<char>::eof() && rawBuffer.empty()) {
                    return false;
                }

                if (c != std::char_traits<char>::eof()) {
                    rawBuffer.push_back(static_cast<char>(c));
                }

                char* inbuf = rawBuffer.data();
                size_t inbytesLeft = rawBuffer.size();

                char outChunk[32];
                char* outbuf = outChunk;
                size_t outbytesLeft = sizeof(outChunk);

                size_t res =
                    iconv(cd, &inbuf, &inbytesLeft, &outbuf, &outbytesLeft);

                if (res == (size_t)-1) {
                    if (errno == EINVAL) {
                        // Incomplete sequence. Loop again.
                        if (c == std::char_traits<char>::eof()) {
                            throw std::invalid_argument(
                                "Incomplete byte sequence at EOF");
                        }
                        continue;
                    } else {
                        throw std::runtime_error(
                            "iconv stream conversion failed");
                    }
                }

                size_t converted = sizeof(outChunk) - outbytesLeft;
                if (converted > 0) {
                    buffer.insert(buffer.end(), outChunk, outChunk + converted);
                }

                size_t consumed = rawBuffer.size() - inbytesLeft;
                if (consumed > 0) {
                    rawBuffer.erase(rawBuffer.begin(),
                                    rawBuffer.begin() + consumed);
                }

#endif
            }
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

    /**
     * @brief Sets the encoding of the input.
     *
     * The stream will transcode on the fly as it fills its internal
     * buffers.
     *
     * Upon calling this function, the internal buffer will be flushed.
     *
     * Transcoding can only happen if the capture is empty and there is no
     * recorded lookahead.
     *
     * @param newInputEncoding
     * @return true Transcoding is being done after this call
     * @return false Transcoding is not being done after this call
     */
    bool setInputEncoding(std::string newInputEncoding) {
        if (pos != captured) {
            throw std::runtime_error(
                "Cannot change encoding while capture is active");
        }

        if (buffer.size() != pos) {
            throw std::runtime_error(
                "Cannot change encoding with unconsumed decoded lookahead "
                "past pos");
        }

        if (!rawBuffer.empty()) {
            throw std::runtime_error(
                "Cannot change encoding mid multi byte sequence");
        }

#if ICONV_AVAILABLE
        iconv_t newCd = iconv_open("UTF-8", newInputEncoding.c_str());
        if (newCd == (iconv_t)-1) {
            throw std::runtime_error("Failed to initialize iconv_open for " +
                                     newInputEncoding);
        }

        if (cd != (iconv_t)-1) {
            iconv_close(cd);
        }
        cd = newCd;

        inputEncoding = std::move(newInputEncoding);
        transcoding = inputEncoding != "UTF-8";

        buffer.clear();
        pos = 0;
        captured = 0;
        return transcoding;
#else
        throw std::runtime_error(
            "Trying to transcode at runtime without iconv");
#endif
    }

   private:
    /**
     * @brief Whether the cursor is actively transcoding
     *
     */
    bool transcoding = false;
};
}  // namespace onyx::dynamic::parser