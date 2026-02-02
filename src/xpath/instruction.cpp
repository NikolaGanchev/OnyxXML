#include "xpath/instruction.h"
#include <stdexcept>

namespace onyx::dynamic::xpath {
std::string_view Instruction::opcodeToString(OPCODE op) {
    switch (op) {
        case OPCODE::HALT: return "HALT";
        case OPCODE::LOAD_CONSTANT: return "LOAD_CONSTANT";
        case OPCODE::LOAD_CONTEXT_NODE: return "LOAD_CONTEXT_NODE";
        case OPCODE::LOAD_POSITION: return "LOAD_POSITION";
        case OPCODE::LOAD_LENGTH: return "LOAD_LENGTH";
        case OPCODE::LOAD_ROOT: return "LOAD_ROOT";
        case OPCODE::LOAD_VARIABLE: return "LOAD_VARIABLE";
        case OPCODE::SELECT: return "SELECT";
        case OPCODE::UNION: return "UNION";
        case OPCODE::SORT: return "SORT";
        case OPCODE::JUMP: return "JUMP";
        case OPCODE::JUMP_F: return "JUMP_F";
        case OPCODE::JUMP_T: return "JUMP_T";
        case OPCODE::LOOP_ENTER: return "LOOP_ENTER";
        case OPCODE::CONTEXT_NODE_TEST: return "CONTEXT_NODE_TEST";
        case OPCODE::LOOP_NEXT: return "LOOP_NEXT";
        case OPCODE::LOOP_UNION: return "LOOP_UNION";
        case OPCODE::CALCULATE: return "CALCULATE";
        case OPCODE::COMPARE: return "COMPARE";
        case OPCODE::CALL: return "CALL";
    }
    throw std::runtime_error("Unknown OPCODE.");
}
std::string_view Instruction::compareModeToString(COMPARE_MODE cm) {
    switch (cm) {
        case COMPARE_MODE::EQUAL: return "EQUAL";
        case COMPARE_MODE::NOT_EQUAL: return "NOT_EQUAL";
        case COMPARE_MODE::LESS_THAN: return "LESS_THAN";
        case COMPARE_MODE::LESS_THAN_OR_EQUAL: return "LESS_THAN_OR_EQUAL";
        case COMPARE_MODE::GREATER_THAN: return "GREATER_THAN";
        case COMPARE_MODE::GREATER_THAN_OR_EQUAL: return "GREATER_THAN_OR_EQUAL";
    }
    throw std::runtime_error("Unknown COMPARE_MODE.");
}
std::string_view Instruction::calcModeToString(CALCULATE_MODE cm) {
    switch (cm) {
        case CALCULATE_MODE::ADD: return "ADD";
        case CALCULATE_MODE::SUBTRACT: return "SUBTRACT";
        case CALCULATE_MODE::MULTIPLY: return "MULTIPLY";
        case CALCULATE_MODE::DIVIDE: return "DIVIDE";
        case CALCULATE_MODE::MOD: return "MOD";
    }
    throw std::runtime_error("Unknown CALCULATE_MODE.");
}
std::string_view Instruction::functionCodeToString(FUNCTION_CODE fc) {
    switch (fc) {
        case FUNCTION_CODE::LAST_0: return "last";
        case FUNCTION_CODE::POSITION_0: return "position";
        case FUNCTION_CODE::COUNT_1: return "count";
        case FUNCTION_CODE::ID_1: return "id";
        case FUNCTION_CODE::LOCAL_NAME_1: return "local-name";
        case FUNCTION_CODE::NAMESPACE_URI_1: return "namespace-uri";
        case FUNCTION_CODE::NAME_1: return "name";
        case FUNCTION_CODE::STRING_1: return "string";
        case FUNCTION_CODE::CONCAT_2: return "concat";
        case FUNCTION_CODE::STARTS_WITH_2: return "starts-with";
        case FUNCTION_CODE::CONTAINS_2: return "contains";
        case FUNCTION_CODE::SUBSTRING_BEFORE_2: return "substring-before";
        case FUNCTION_CODE::SUBSTRING_AFTER_2: return "substring-after";
        case FUNCTION_CODE::SUBSTRING_2: return "substring2";
        case FUNCTION_CODE::SUBSTRING_3: return "substring3";
        case FUNCTION_CODE::STRING_LENGTH_1: return "string-length";
        case FUNCTION_CODE::NORMALIZE_SPACE_1: return "normalize-space";
        case FUNCTION_CODE::TRANSLATE_3: return "translate";
        case FUNCTION_CODE::BOOLEAN_1: return "boolean";
        case FUNCTION_CODE::NOT_1: return "not";
        case FUNCTION_CODE::TRUE_0: return "true";
        case FUNCTION_CODE::FALSE_0: return "false";
        case FUNCTION_CODE::LANG_1: return "lang";
        case FUNCTION_CODE::NUMBER_1: return "number";
        case FUNCTION_CODE::SUM_1: return "sum";
        case FUNCTION_CODE::FLOOR_1: return "floor";
        case FUNCTION_CODE::CEILING_1: return "ceiling";
        case FUNCTION_CODE::ROUND_1: return "round";
    }
    throw std::runtime_error("Unknown FUNCTION_CODE.");
}

std::string_view Instruction::axisToString(AXIS a) {
    switch (a) {
        case AXIS::ANCESTOR: return "ancestor";
        case AXIS::ANCESTOR_OR_SELF: return "ancestor-or-self";
        case AXIS::ATTRIBUTE: return "attribute";
        case AXIS::CHILD: return "child";
        case AXIS::DESCENDANT: return "descendant";
        case AXIS::DESCENDANT_OR_SELF: return "descendant-or-self";
        case AXIS::FOLLOWING: return "following";
        case AXIS::FOLLOWING_SIBLING: return "following-sibling";
        case AXIS::NAMESPACE: return "namespace";
        case AXIS::PARENT: return "parent";
        case AXIS::PRECEDING: return "preceding";
        case AXIS::PRECEDING_SIBLING: return "preceding-sibling";
        case AXIS::SELF: return "self";
    };

    throw std::runtime_error("Unknown AXIS.");
}
}  // namespace onyx::dynamic::xpath