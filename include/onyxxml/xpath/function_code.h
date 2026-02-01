#pragma once
#include <cstdint>

namespace onyx::dynamic::xpath {
/**
 * @brief The function codes for XPath functions
 * Based on:
 * https://www.w3.org/TR/1999/REC-xpath-19991116/#section-Node-Set-Functions
 * The functions are defined with *no overloads or optional arguments*.
 */
enum FUNCTION_CODE : uint8_t {
    LAST_0,
    POSITION_0,
    COUNT_1,
    ID_1,
    LOCAL_NAME_1,
    NAMESPACE_URI_1,
    NAME_1,
    STRING_1,
    CONCAT_2,
    STARTS_WITH_2,
    CONTAINS_2,
    SUBSTRING_BEFORE_2,
    SUBSTRING_AFTER_2,
    SUBSTRING_2,
    SUBSTRING_3,
    STRING_LENGTH_1,
    NORMALIZE_SPACE_1,
    TRANSLATE_3,
    BOOLEAN_1,
    NOT_1,
    TRUE_0,
    FALSE_0,
    LANG_1,
    NUMBER_1,
    SUM_1,
    FLOOR_1,
    CEILING_1,
    ROUND_1
};
}  // namespace onyx::dynamic::xpath