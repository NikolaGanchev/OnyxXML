#pragma once
#include <cstdint>

namespace onyx::dynamic::xpath {
/**
 * @brief The operation modes for the COMPARE operation code
 *
 */
enum COMPARE_MODE : uint8_t {
    EQUAL = 0x0,
    NOT_EQUAL = 0x1,
    LESS_THAN = 0x2,
    GREATER_THAN = 0x3,
    LESS_THAN_OR_EQUAL = 0x4,
    GREATER_THAN_OR_EQUAL = 0x5
};
}  // namespace onyx::dynamic::xpath