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

    static consteval std::size_t placeholderCount() { return 0; }

    /**
     * @brief The ProcessingInstruction string; evaluated at compile-time. Does
     * not do any escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval EvaluatedDocument<size() + 1, 0> serialize() {
        EvaluatedDocument<size() + 1, 0> result = {};
        evaluate(result);
        return result;
    }

    /**
     * @brief Evaluates the ProcessingInstruction into an existing
     * EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        CompileStringUtils::placeStringInEvaluatedDocument(result, "<?");
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           Target.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, " ");
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           Instruction.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, "?>");
        CompileStringUtils::placeStringInEvaluatedDocument(result, "\0");
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