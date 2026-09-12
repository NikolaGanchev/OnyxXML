#pragma once

#include <cstddef>

namespace onyx::compile {

struct PlaceholderInfo {
    std::size_t position;
    std::size_t length;
};

}  // namespace onyx::compile