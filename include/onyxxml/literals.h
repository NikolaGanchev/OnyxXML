#pragma once

#include <string_view>

#include "./nodes/text_node.h"
#include "attribute.h"
#include "compile/compile_string.h"

namespace onyx::dynamic::tags::literals {

namespace impl {

/**
 * @brief Breaks up an attribute literal at compile time.
 *
 * @tparam Str
 */
template <compile::CompileString Str>
struct CompileAttributeStringLiteral {
    consteval static std::size_t equalsPosition() {
        constexpr std::size_t equalsPosition =
            std::string_view(Str.value).find('=');
        if (equalsPosition == std::string_view::npos) {
            throw "Attribute literal \"" + std::string(Str.value) +
                "\" is missing '='";
        }
        if (equalsPosition == 0) {
            throw "Attribute literal \"" + std::string(Str.value) +
                "\" is missing a name";
        }

        return equalsPosition;
    }

    static constexpr std::size_t equalsPos = equalsPosition();

    consteval static std::size_t nameSize() { return equalsPos; }

    consteval static std::size_t valueSize() {
        // The subtraction of 2 is due to the '=' and the null terminator
        return Str.size - equalsPos - 2;
    }

    consteval static std::array<char, nameSize() + 1> name() {
        std::array<char, nameSize() + 1> result = {};
        for (size_t i = 0; i < equalsPos; i++) {
            result[i] = Str.value[i];
        }
        result[equalsPos] = '\0';

        return result;
    }

    consteval static std::array<char, valueSize() + 1> value() {
        std::array<char, valueSize() + 1> result = {};

        for (size_t i = 0; i < valueSize(); i++) {
            result[i] = Str.value[i + equalsPos + 1];
        }

        result[valueSize()] = '\0';

        return result;
    }
};
}  // namespace impl

/**
 * @brief A user-defined literal that generates a Text node from a given const
 * char*
 *
 * @param str
 * @return Text
 */
inline Text operator""_t(const char* str, std::size_t) { return Text(str); }

/**
 * @brief A user-defined literal that generates an Attribute from a given const
 * char*. Validates that an equals character ('=') exists and is not the first
 * character.
 *
 * @param str
 * @return Text
 */
template <compile::CompileString Str>
inline Attribute operator""_attr() {
    using AttributeString = impl::CompileAttributeStringLiteral<Str>;
    constexpr auto name = AttributeString::name();
    constexpr auto value = AttributeString::value();
    return Attribute(std::string(name.data()), std::string(value.data()));
}
}  // namespace onyx::dynamic::tags::literals