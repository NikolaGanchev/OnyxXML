#pragma once

#include <utility>
#include <string_view>

namespace onyx::dynamic::parser {
bool isWhitespace(const char pos);

const char* skipWhitespace(const char* pos);

uint32_t handleUnicodeChar(const char*& ch);

bool isNameStartChar(const char*& ch);

bool isNameChar(const char*& ch);

std::string_view readName(const char* pos);
}