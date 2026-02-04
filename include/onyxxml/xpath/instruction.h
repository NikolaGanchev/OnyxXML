#pragma once
#include <string_view>

#include "axis.h"
#include "calculate_mode.h"
#include "compare_mode.h"
#include "function_code.h"
#include "opcode.h"

namespace onyx::dynamic::xpath {

/**
 * @brief Class that encodes an instruction. There are two types of
 * instructions:
 * Stack-type instructions. They have no arguments and have 24 bit
 * of padding after the opcode.
 * Immediate-type instructions. They have a single
 * 24 bit data section index argument after the opcode.
 *
 */
class Instruction {
   private:
    /**
     * @brief The instruction data
     *
     */
    uint32_t data;

   public:
    /**
     * @brief Construct a new Instruction object based on the raw instruction
     *
     * @param data
     */
    Instruction(uint32_t data) : data(data) {}

    /**
     * @brief Construct a new Instruction object of stack type
     *
     * @param op
     */
    explicit Instruction(OPCODE op) { data = static_cast<uint32_t>(op) << 24; }

    /**
     * @brief Construct a new Instruction object of immediate type
     *
     * @param op
     * @param operand
     */
    Instruction(OPCODE op, uint32_t operand) {
        data = (static_cast<uint32_t>(op) << 24) | (operand & 0x00FFFFFF);
    }

    /**
     * @brief Get the operation code
     *
     * @return OPCODE
     */
    OPCODE getOpcode() const { return static_cast<OPCODE>(data >> 24); }

    /**
     * @brief Get the operand in an immediate type instruction
     *
     * @return uint32_t
     */
    uint32_t getOperandImm() const { return data & 0x00FFFFFF; }

    /**
     * @brief Get the raw instruction data
     *
     * @return uint32_t
     */
    uint32_t getRaw() const { return data; }

    /**
     * @brief Converts an OPCODE to string
     *
     * @param op
     * @return std::string_view
     */
    static std::string_view opcodeToString(OPCODE op);

    /**
     * @brief Converts a COMPARE_MODE to string
     *
     * @param cm
     * @return std::string_view
     */
    static std::string_view compareModeToString(COMPARE_MODE cm);

    /**
     * @brief Converts a CALCULATE_MODE to string
     *
     * @param cm
     * @return std::string_view
     */
    static std::string_view calcModeToString(CALCULATE_MODE cm);

    /**
     * @brief Converts a FUNCTION_CODE to string
     *
     * @param fc
     * @return std::string_view
     */
    static std::string_view functionCodeToString(FUNCTION_CODE fc);

    /**
     * @brief Converts an AXIS to string
     *
     * @param a
     * @return std::string_view
     */
    static std::string_view axisToString(AXIS a);

    /**
     * @brief Returns the maximum value of an operand
     *
     * @return uint32_t
     */
    static uint32_t maxOperand() { return 0x00FFFFFF; }
};
}  // namespace onyx::dynamic::xpath