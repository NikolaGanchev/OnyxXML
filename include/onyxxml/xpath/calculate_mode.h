#pragma once
#include <cstdint>

namespace onyx::dynamic::xpath {
/**
 * @brief The operation modes for the CALCULATE operation code
 *
 */
enum CALCULATE_MODE : uint8_t {
    ADD = 0x0,
    SUBTRACT = 0x1,
    MULTIPLY = 0x2,
    DIVIDE = 0x3,
    MOD = 0X4
};
}  // namespace onyx::dynamic::xpath