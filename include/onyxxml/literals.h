#pragma once

#include "./nodes/text_node.h"

namespace onyx::dynamic::tags::literals {

/**
 * @brief A user-defined literal that generates a Text node from a given const
 * char*
 *
 * @param str
 * @return Text
 */
inline Text operator""_t(const char* str, std::size_t) { return Text(str); }
}  // namespace onyx::dynamic::tags::literals