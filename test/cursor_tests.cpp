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
    REQUIRE_FALSE(stringCursor.isEOF());

    REQUIRE(streamCursor.current() == 't');
    REQUIRE(streamCursor.peek(1) == 'e');
    REQUIRE(streamCursor.peek(5) == 's');
    REQUIRE(streamCursor.inputEncoding == "UTF-8");
    REQUIRE_FALSE(streamCursor.isEOF());
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
    REQUIRE(stringCursor.current() == 'w');

    streamCursor.advance(6);
    streamCursor.beginCapture();
    streamCursor.captureAdvance(5);
    REQUIRE(streamCursor.getCaptured() == "world");
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