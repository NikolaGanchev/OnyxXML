#pragma once

#include <cstddef>

namespace onyx::compile {

/**
 * @brief Holds the position and length of a Placeholder.
 *
 */
struct PlaceholderInfo {
    std::size_t position;
    std::size_t length;
};

}  // namespace onyx::compile