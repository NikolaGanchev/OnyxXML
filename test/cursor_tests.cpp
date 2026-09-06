#include <chrono>

#include "catch2/catch_all.hpp"
#include "onyx.h"
#include "parse/stream_cursor.h"
#include "parse/string_cursor.h"

using namespace onyx::dynamic::parser;

TEST_CASE("Cursors report the initial character and can look ahead") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    REQUIRE(stringCursor.current() == 't');
    REQUIRE(stringCursor.peek(1) == 'e');
    REQUIRE(stringCursor.peek(5) == 's');

    REQUIRE(streamCursor.current() == 't');
    REQUIRE(streamCursor.peek(1) == 'e');
    REQUIRE(streamCursor.peek(5) == 's');
    REQUIRE(streamCursor.inputEncoding == "UTF-8");
}

TEST_CASE("Cursors isEOF() works with offset") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    REQUIRE_FALSE(stringCursor.isEOF());
    REQUIRE_FALSE(stringCursor.isEOF(10));
    REQUIRE(stringCursor.isEOF(11));
    REQUIRE(stringCursor.isEOF(12));

    REQUIRE_FALSE(streamCursor.isEOF(10));
    REQUIRE(streamCursor.isEOF(11));
    REQUIRE(streamCursor.isEOF(12));
}

TEST_CASE("advance moves the current position by the requested amount") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    stringCursor.advance();
    REQUIRE(stringCursor.current() == 'e');
    stringCursor.advance(3);
    REQUIRE(stringCursor.current() == ' ');

    streamCursor.advance();
    REQUIRE(streamCursor.current() == 'e');
    streamCursor.advance(3);
    REQUIRE(streamCursor.current() == ' ');
}

TEST_CASE("consumeIfMatches advances the cursor past an exact match") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    REQUIRE(stringCursor.consumeIfMatches("test"));
    REQUIRE(stringCursor.current() == ' ');

    REQUIRE(streamCursor.consumeIfMatches("test"));
    REQUIRE(streamCursor.current() == ' ');
}

TEST_CASE("consumeIfMatches leaves the cursor untouched after a mismatch") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    REQUIRE_FALSE(stringCursor.consumeIfMatches("tets"));
    REQUIRE(stringCursor.current() == 't');

    REQUIRE_FALSE(streamCursor.consumeIfMatches("tets"));
    REQUIRE(streamCursor.current() == 't');
}

TEST_CASE(
    "consumeIfMatches with an empty pattern trivially succeeds without "
    "consuming input") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    REQUIRE(stringCursor.consumeIfMatches(""));
    REQUIRE(stringCursor.current() == 't');

    REQUIRE(streamCursor.consumeIfMatches(""));
    REQUIRE(streamCursor.current() == 't');
    REQUIRE(streamCursor.buffer.size() <= 1);
}

TEST_CASE(
    "beginCapture and captureAdvance record a span without moving the "
    "primary cursor") {
    std::string text = "hello world";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    stringCursor.advance(6);
    stringCursor.beginCapture();
    stringCursor.captureAdvance(5);
    REQUIRE(stringCursor.getCaptured() == "world");
    REQUIRE(stringCursor.getCapturedSize() == 5);
    REQUIRE(stringCursor.current() == 'w');

    streamCursor.advance(6);
    streamCursor.beginCapture();
    streamCursor.captureAdvance(5);
    REQUIRE(streamCursor.getCaptured() == "world");
    REQUIRE(streamCursor.getCapturedSize() == 5);
    REQUIRE(streamCursor.current() == 'w');
}

TEST_CASE("bringToCapture moves the primary cursor to the captured position") {
    std::string text = "hello world";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    stringCursor.beginCapture();
    stringCursor.captureAdvance(5);
    stringCursor.bringToCapture();
    REQUIRE(stringCursor.current() == ' ');

    streamCursor.beginCapture();
    streamCursor.captureAdvance(5);
    streamCursor.bringToCapture();
    REQUIRE(streamCursor.current() == ' ');
}

TEST_CASE("swapDefault exchanges the primary and capture cursor positions") {
    std::string text = "hello world";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    stringCursor.advance(6);
    stringCursor.beginCapture();
    stringCursor.captureAdvance(5);
    stringCursor.swapDefault();
    REQUIRE(stringCursor.current() == '\0');
    REQUIRE(stringCursor.captureCurrent() == 'w');
    REQUIRE(stringCursor.isEOF());

    streamCursor.advance(6);
    streamCursor.beginCapture();
    streamCursor.captureAdvance(5);
    streamCursor.swapDefault();
    REQUIRE(streamCursor.current() == '\0');
    REQUIRE(streamCursor.captureCurrent() == 'w');
    REQUIRE(streamCursor.isEOF());
}

TEST_CASE(
    "isEOF reports the end of input only once every character has been "
    "consumed") {
    std::string text = "hi";
    std::stringstream inputStream(text);

    StringCursor stringCursor(text);
    StreamCursor streamCursor(inputStream);

    REQUIRE_FALSE(stringCursor.isEOF());
    stringCursor.advance(2);
    REQUIRE(stringCursor.isEOF());

    REQUIRE_FALSE(streamCursor.isEOF());
    streamCursor.advance(2);
    REQUIRE(streamCursor.isEOF());
}

TEST_CASE(
    "setInputEncoding returns false when the requested encoding matches the "
    "current one") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.current() == 't');
    streamCursor.advance();
    streamCursor.beginCapture();
    REQUIRE_FALSE(streamCursor.setInputEncoding("UTF-8"));
    REQUIRE(streamCursor.inputEncoding == "UTF-8");
    REQUIRE(streamCursor.current() == 'e');
}

TEST_CASE(
    "setInputEncoding transcodes a single byte encoding to UTF-8 on the "
    "fly") {
    std::string text = "caf\xE9";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.setInputEncoding("ISO-8859-1"));
    REQUIRE(streamCursor.inputEncoding == "ISO-8859-1");

    streamCursor.beginCapture();
    streamCursor.captureAdvance(5);
    REQUIRE(streamCursor.getCaptured() == "caf\xC3\xA9");
    REQUIRE(streamCursor.getCapturedSize() == 5);
}

TEST_CASE(
    "setInputEncoding transcodes a multi byte encoding character by "
    "character") {
    std::string text = {'\x41', '\x00', '\x42', '\x00', '\x43', '\x00'};
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.setInputEncoding("UTF-16LE"));

    REQUIRE(streamCursor.current() == 'A');
    streamCursor.advance();
    REQUIRE(streamCursor.current() == 'B');
    streamCursor.advance();
    REQUIRE(streamCursor.current() == 'C');
    streamCursor.advance();
    REQUIRE(streamCursor.isEOF());
}

TEST_CASE(
    "consumeIfMatches operates on transcoded characters rather than raw "
    "bytes") {
    std::string text = "caf\xE9 latte";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.setInputEncoding("ISO-8859-1"));
    REQUIRE(streamCursor.consumeIfMatches("caf\xC3\xA9"));
    REQUIRE(streamCursor.current() == ' ');
}

TEST_CASE(
    "isEOF accounts for the transcoded output length rather than the raw "
    "byte count") {
    std::string text = "caf\xE9";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.setInputEncoding("ISO-8859-1"));
    REQUIRE_FALSE(streamCursor.isEOF());

    streamCursor.advance(5);
    REQUIRE(streamCursor.isEOF());
}

TEST_CASE("setInputEncoding throws while a capture is active") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    streamCursor.beginCapture();
    streamCursor.captureAdvance(3);

    REQUIRE_THROWS_WITH(streamCursor.setInputEncoding("ISO-8859-1"),
                        "Cannot change encoding while capture is active");
}

TEST_CASE("setInputEncoding allows raw lookahead and transcodes it") {
    std::string text = "caf\xE9 latte";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.peek(3) == '\xE9');

    REQUIRE(streamCursor.setInputEncoding("ISO-8859-1"));

    streamCursor.beginCapture();
    streamCursor.captureAdvance(5);
    REQUIRE(streamCursor.getCaptured() == "caf\xC3\xA9");
    REQUIRE(streamCursor.getCapturedSize() == 5);
}

TEST_CASE(
    "setInputEncoding throws when there is unconsumed decoded lookahead "
    "past pos") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    streamCursor.setInputEncoding("ISO-8859-1");

    streamCursor.peek(3);

    REQUIRE_THROWS_WITH(
        streamCursor.setInputEncoding("UTF-16LE"),
        "Cannot change encoding with unconsumed decoded lookahead past pos");
}

TEST_CASE("setInputEncoding throws for an unknown or invalid encoding name") {
    std::string text = "test string";
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE_THROWS_WITH(
        streamCursor.setInputEncoding("EXAMPLE-ENCODING"),
        Catch::Matchers::ContainsSubstring("Failed to initialize iconv_open"));
}

TEST_CASE("An incomplete multi byte sequence at the end of the stream throws") {
    std::string text = {'\x42'};
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.setInputEncoding("UTF-16LE"));
    REQUIRE_THROWS_WITH(streamCursor.current(),
                        "Incomplete byte sequence at EOF");
}

TEST_CASE(
    "setInputEncoding throws when an incomplete multi byte sequence is "
    "still pending") {
    std::string text = {'\x42'};
    std::stringstream inputStream(text);

    StreamCursor streamCursor(inputStream);

    REQUIRE(streamCursor.setInputEncoding("UTF-16LE"));
    REQUIRE_THROWS_WITH(streamCursor.current(),
                        "Incomplete byte sequence at EOF");

    REQUIRE_THROWS_WITH(streamCursor.setInputEncoding("ISO-8859-1"),
                        "Cannot change encoding mid multi byte sequence");
}
