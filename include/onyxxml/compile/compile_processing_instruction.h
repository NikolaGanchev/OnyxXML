#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/processing_instruction_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"

namespace onyx::compile::ctags {
/**
 * @brief A compile-time ProcessingInstruction struct.
 *
 * @tparam Str
 */
template <CompileString Target, CompileString Instruction>
struct ProcessingInstruction {
    /**
     * @brief The compile-time size of the ProcessingInstruction string. Does
     * not account for
     * '\0'
     *
     * @return size_t
     */
    static consteval size_t size() {
        return std::string_view(Target.value).size() + 2 +
               std::string_view(Instruction.value).size() + 2 +
               1;  // added size for <? (2), ?> (2) and space between target and
                   // instruction (1)
    }

    /**
     * @brief The ProcessingInstruction string; evaluated at compile-time. Does
     * not do any escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval std::array<char, size() + 1> serialize() {
        std::array<char, size() + 1> result = {};
        size_t index = CompileStringUtils::placeStringInArray(result, "<?", 0);
        index =
            CompileStringUtils::placeStringInArray(result, Target.value, index);
        index = CompileStringUtils::placeStringInArray(result, " ", index);
        index = CompileStringUtils::placeStringInArray(
            result, Instruction.value, index);
        index = CompileStringUtils::placeStringInArray(result, "?>", index);
        index = CompileStringUtils::placeStringInArray(result, "\0", index);
        return result;
    }

    /**
     * @brief Construct a dynamic ProcessingInstruction Node from a compile-time
     * ProcessingInstruction struct.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        return std::make_unique<onyx::dynamic::tags::ProcessingInstruction>(
            Target, Instruction);
    }
};
}  // namespace onyx::compile::ctags