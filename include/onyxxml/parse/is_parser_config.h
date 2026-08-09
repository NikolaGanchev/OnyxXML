#pragma once

#include <concepts>

namespace onyx::dynamic::parser {
template <typename T>
concept isParserConfig = requires {
    { T::validate } -> std::same_as<const bool&>;
    { T::validateDuplicateAttributes } -> std::same_as<const bool&>;
    { T::maxAttributeCount } -> std::same_as<const size_t&>;
};
}  // namespace onyx::dynamic::parser