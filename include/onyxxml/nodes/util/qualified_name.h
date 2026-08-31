#pragma once
#include <string_view>

namespace onyx::dynamic::tags::util {
/**
 * @brief A class used as a parameter for classes consuming qualified XML
 * namespace, i.e., which may have a namespace prefix.
 * Will fully evaluate at compile time if possible.
 * Does not throw exceptions. If the separator (':') is in the first position,
 * this class will report empty prefix. Conversely, if it is in the last, it
 * will report empty name.
 *
 * This class implements a move constructor with non-standard behaviour. It does
 * not take ownership of or copy the temporary string, but merely extracts
 * slices of it. Any consumer of this class who wishes to use it as an argument
 * should ne aware that its contents are not guaranteed to exist beyond the call
 * to the function using it as argument.
 */
struct QualifiedName {
    /**
     * @brief The namespace prefix
     *
     */
    std::string_view prefix;
    /**
     * @brief The tag name
     *
     */
    std::string_view name;

    /**
     * @brief Construct a QualifiedName from std::string_view, possibly at
     * compile time
     *
     */
    constexpr QualifiedName(std::string_view name) {
        std::size_t separatorPos = name.find(":");
        if (separatorPos == std::string_view::npos) {
            prefix = "";
            this->name = std::string_view(name);
            return;
        }

        this->prefix = name.substr(0, separatorPos);
        this->name = name.substr(separatorPos + 1);
    }

    /**
     * @brief Construct a QualifiedName from const char*, possibly at
     * compile time
     *
     */
    constexpr QualifiedName(const char* name)
        : QualifiedName(std::string_view(name)) {}

    /**
     * @brief Construct a QualifiedName from std::string&&, possibly at
     * compile time. Does not actually take ownership of the string or copy it.
     *
     */
    QualifiedName(std::string&& name) : QualifiedName(std::string_view(name)) {}
};
}  // namespace onyx::dynamic::tags::util