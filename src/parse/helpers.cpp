#include "parse/helpers.h"

#include "text.h"

namespace onyx::dynamic::parser {
bool isWhitespace(const char pos) {
    return (pos == ' ' || pos == '\t' || pos == '\r' || pos == '\n');
}

bool isDigit(char c) { return c >= '0' && c <= '9'; }
}  // namespace onyx::dynamic::parser