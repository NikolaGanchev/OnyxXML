#include "parse/helpers.h"

#include "text.h"

namespace onyx::dynamic::parser {
bool isWhitespace(const char pos) {
    return (pos == ' ' || pos == '\t' || pos == '\r' || pos == '\n');
}
}  // namespace onyx::dynamic::parser