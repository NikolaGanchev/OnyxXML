#pragma once

#include <concepts>

template <typename T>
concept isParserConfig = requires {
    { T::validate } -> std::same_as<const bool&>;
};