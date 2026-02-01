#pragma once
#include <cstdint>

namespace onyx::dynamic::xpath {

/**
 * @brief The possible axes for the SELECT instruction
 *
 */
enum AXIS : uint8_t {
    CHILD = 0x0,
    DESCENDANT = 0x1,
    PARENT = 0x2,
    ANCESTOR = 0x3,
    FOLLOWING_SIBLING = 0x4,
    PRECEDING_SIBLING = 0x5,
    FOLLOWING = 0x6,
    PRECEDING = 0x7,
    ATTRIBUTE = 0x8,
    NAMESPACE = 0x9,
    SELF = 0xA,
    DESCENDANT_OR_SELF = 0xB,
    ANCESTOR_OR_SELF = 0xC
};
}  // namespace onyx::dynamic::xpath