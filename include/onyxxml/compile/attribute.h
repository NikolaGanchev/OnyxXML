#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>

#include "../attribute.h"
#include "compile_string.h"
#include "compile_string_utils.h"
#include "evaluated_document.h"

namespace onyx::compile::ctags {
/**
 * @brief A compile time Attribute struct which has a Name and a Value.
 *
 * @tparam Name
 * @tparam Value
 */
template <CompileString Name, CompileString Value>
struct Attribute {
    /**
     * @brief The compile-time size of the attribute string. Does not account
     * for '\0'; The attribute string is formed as follows: ` name="value"`.
     *
     * @return size_t
     */
    static consteval size_t size() {
        return std::string_view(Name.value).size() +
               std::string_view(Value.value).size() +
               4;  // +4 for the space, =, and ""
    }

    /**
     * @brief The compile-time evaluation of the attribute string. Does not
     * account for '\0'; The attribute string is formed as follows: `
     * name="value"`.
     *
     */
    static consteval EvaluatedDocument<size() + 1, 0> serialize() {
        EvaluatedDocument<size() + 1, 0> result = {};
        evaluate(result);
        return result;
    }

    /**
     * @brief Evaluates the Attribute into an existing EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        CompileStringUtils::placeStringInEvaluatedDocument(result, " ");
        CompileStringUtils::placeStringInEvaluatedDocument(result, Name.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, "=\"");
        CompileStringUtils::placeStringInEvaluatedDocument(result, Value.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, "\"\0");
    }

    /**
     * @brief Construct a dynamic Attribute from a compile time Attribute.
     *
     * @return std::unique_ptr<onyx::dynamic::Attribute>
     */
    static std::unique_ptr<onyx::dynamic::Attribute> dynamicAttribute() {
        return std::make_unique<onyx::dynamic::Attribute>(Name, Value);
    }

    static consteval std::size_t placeholderCount() { return 0; }
};

/**
 * @brief Checks if the given template param is a compile time Attribute.
 * Since compile time Attributes are specialised with unpredictable inputs,
 * the check can only be done by checking for the existence of the attr() static
 * function.
 *
 * @tparam T
 */
template <typename T>
concept isAttribute = requires(T) {
    {
        T::dynamicAttribute()
    } -> std::same_as<std::unique_ptr<onyx::dynamic::Attribute>>;
};
}  // namespace onyx::compile::ctags