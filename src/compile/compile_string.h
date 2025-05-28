#pragma once

#include <string>

namespace Templater::compile {

/**
 * @brief A compile time string.
 *
 * @tparam N The size of the string
 */
template <size_t N>
struct CompileString {
    /**
     * @brief The char array that holds the string.
     *
     */
    char value[N];

    /**
     * @brief Constructs a CompileString via a given const char array.
     *
     * @param str
     *
     */
    constexpr CompileString(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    /**
     * @brief Returns the std::string constructed from this CompileString.
     *
     * @return constexpr std::string
     */
    constexpr std::string to_string() const {
        return std::string(value, N - 1);
    }

    /**
     * @brief Cast to std::string
     *
     * @return std::string
     */
    constexpr operator std::string() const { return to_string(); }
};
}  // namespace Templater::compile