#pragma once
#include <cstdint>

namespace onyx::dynamic::xpath {
/**
 * @brief The operation codes of the virtual machine.
 * The virtual machine instructions are 32-bit fixed-width. The first 8 bits are
 * always an operation code. This enum provides the operation codes.
 *
 */
enum class OPCODE : uint8_t {
    /**
     * @brief Stops the virtual machine.
     * Stack type instruction.
     */
    HALT = 0x0,

    /**
     * @brief Pushes a constant to the top of the stack. That may be a number or
     * a string.
     * Immediate type instruction.
     */
    LOAD_CONSTANT = 0x1,

    /**
     * @brief Pushes the node the current context is acting on on top of the
     * stack. Stack type instruction.
     */
    LOAD_CONTEXT_NODE = 0x2,

    /**
     * @brief Pushes the context position on top of the stack.
     * Stack type instruction.
     */
    LOAD_POSITION = 0x3,

    /**
     * @brief Pushes the context set length on top of the stack.
     * Stack type instruction.
     */
    LOAD_LENGTH = 0x4,

    /**
     * @brief Pushes the root node of the document on top of the stack.
     * Stack type instruction.
     */
    LOAD_ROOT = 0x5,

    /**
     * @brief Selects nodes on a given axis with a given name (from the stack)
     * into a nodeset and pushes it to the top of the stack based on the node on
     * top of the stack. Pop the stack. Immediate instruction.
     */
    SELECT = 0x10,

    /**
     * @brief Merges the two nodesets on top of the stack into a sorted nodeset
     * based on document order. Pops the stack twice.
     * Stack type instruction.
     */
    UNION = 0x11,

    /**
     * @brief Takes the nodeset on top of the stack and sorts it into document
     * order. Used after a SELECT call to preceding, ancestor, ancestor-or-self
     * and preceding-sibling.
     *
     */
    SORT = 0x12,

    /**
     * @brief Unconditional jump to a given position.
     * Immediate type instruction.
     */
    JUMP = 0x20,

    /**
     * @brief Jump to the given position if the object on top of the stack is
     * falsy. Pops the stack.
     * Immediate type instruction.
     */
    JUMP_F = 0x21,

    /**
     * @brief Jump to the given position if the object on top of the stack is
     * truthy. Pops the stack.
     * Immediate type instruction.
     */
    JUMP_T = 0x22,

    /**
     * @brief Begin a loop over the nodeset on top of the stack. Pops the stack.
     * Begins a new context with the context node being the first of the
     * nodeset. If the nodeset is empty, jumps to the provided address instead.
     * Immediate type instruction.
     */
    LOOP_ENTER = 0x23,

    /**
     * @brief Pops a value from the stack. If it is truthy, adds the context
     * node to the context result stack. Otherwise, does not add it.
     * Stack type instruction.
     */
    CONTEXT_NODE_TEST = 0x24,

    /**
     * @brief If the loop has exhausted all nodes, pushes the result set of the
     * context on the stack. If the LOOP_UNION instruction has been used,
     * performs a normalization. Otherwise, sets the context node to be the next
     * node on the nodeset and performs a jump.
     * Stack type instruction.
     */
    LOOP_NEXT = 0x25,

    /**
     * @brief Pops the stack and adds all the elements to the context result
     * list, performing a union. Critically, for performance, this does not
     * perform a proper union until the end of the loop since the result list
     * cannot be accessed. Stack type instruction
     *
     */
    LOOP_UNION = 0x26,

    /**
     * @brief Performs the given math operation on the necessary number of stack
     * elements, which it pops. Pushes the result to the top of the stack.
     * Immediate type instruction.
     */
    CALCULATE = 0x30,

    /**
     * @brief Does the supplied comparison on the top two stack elements. Pushes
     * the result onto the stack.
     * Immediate type instruction.
     */
    COMPARE = 0x31,

    /**
     * @brief Calls an external function by its code and pops from the top of
     * the stack the necessary elements to supply it with.
     * Immediate instruction.
     */
    CALL = 0x41
};
}  // namespace onyx::dynamic::xpath