#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../attribute.h"
#include "compile_string.h"
#include "compile_string_utils.h"

namespace Templater::compile::ctags {
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
     * @return size_t
     */
    static consteval std::array<char, size() + 1> serialize() {
        std::array<char, size() + 1> result = {};
        size_t index = CompileStringUtils::placeStringInArray(result, " ", 0);
        index =
            CompileStringUtils::placeStringInArray(result, Name.value, index);
        index = CompileStringUtils::placeStringInArray(result, "=\"", index);
        index =
            CompileStringUtils::placeStringInArray(result, Value.value, index);
        index = CompileStringUtils::placeStringInArray(result, "\"\0", index);
        return result;
    }

    /**
     * @brief Construct a dynamic Attribute from a compile time Attribute.
     *
     * @return std::unique_ptr<Templater::dynamic::Attribute>
     */
    static std::unique_ptr<Templater::dynamic::Attribute> dynamicAttribute() {
        return std::make_unique<Templater::dynamic::Attribute>(Name, Value);
    }
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
    } -> std::same_as<std::unique_ptr<Templater::dynamic::Attribute>>;
};
}  // namespace Templater::compile::ctags