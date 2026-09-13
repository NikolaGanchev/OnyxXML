#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

#include "compile_string.h"
#include "placeholder_info.h"

namespace onyx::compile {

/**
 * @brief Contains a key and a value for a Placeholder binding.
 *
 */
struct PlaceholderBinding {
    std::string_view key;
    std::string_view value;
};

/**
 * @brief A Placeholder binding struct which guarantees the key is available at
 * compile-time.
 *
 * @tparam Key
 */
template <CompileString Key>
struct Bind {
    static consteval std::string_view getKey() {
        return std::string_view(Key.value, Key.size - 1);
    }
    std::string key;
    std::string value;
    Bind(std::string value) : value(std::move(value)), key(Key) {}
    constexpr operator PlaceholderBinding() const { return {key, value}; }
};

/**
 * @brief A struct which contains an evaluated document. An evaluated document
 * is represented as a computed, sequential std::array with a precomputed size.
 *
 * The names of placeholders are embedded into the content. As such, the content
 * should not be accessed if the Document contains placeholders.
 *
 * The getContent() function and the cast operators protect against this at
 * compile-time. However, objects of this class need to be usable as non-type
 * template parameter, so, it cannot contain private members. In short, it is
 * possible to access the content array, and even modify it, although it is not
 * advisable and not intended.
 *
 * The positions and lengths of placeholder names are recorded in the
 * placeholders array.
 *
 * If the document contains placeholders, it needs to be instantiated.
 * Instantiation is a fast and safe operation. It requires constructing a new
 * string from the internal content via copying of fragments instead of
 * substitution.
 *
 * This class provides the member function instantiate, which takes a C style
 * array of placeholder bindings of the exact placeholder count. The member
 * function strictly runs at runtime. Since it validates that there are exactly
 * as many bindings as placeholders at compile time, and validates that every
 * placeholder has a matching binding at runtime, it does establish a bijective
 * mapping between bindings and placeholders. However, it does that at runtime,
 * while the keys are guaranteed to be known at compile time already.
 *
 * Instead, the free function onyx::compile::instantiate should be
 * used, which provides compile-time validation for redundant keys, missing keys
 * and duplicate keys.
 *
 * @tparam ContentSize
 * @tparam PlaceholderCount
 */
template <std::size_t ContentSize, std::size_t PlaceholderCount>
struct EvaluatedDocument {
    /**
     * @brief Returns a reference to the internal content array. Requires that
     * there are no placeholders.
     *
     */
    constexpr std::array<char, ContentSize>& getContent()
        requires(PlaceholderCount == 0)
    {
        return content;
    }

    /**
     * @brief Takes a C style array placeholder bindings of the exact
     * placeholder count and constructs a string from the content by replacing
     * all placeholders with their matching binding.
     *
     * Since this function validates that there are exactly
     * as many bindings as placeholders at compile time, and validates that
     * every placeholder has a matching binding at runtime, it does establish a
     * bijective mapping between bindings and placeholders.
     *
     * However, it does that at runtime, while the keys are guaranteed to be
     * known at compile time already.
     *
     * Usage of the free function onyx::compile::instantiate is
     * advised instead.
     *
     * Requires at least one placeholder.
     */
    std::string instantiate(
        const PlaceholderBinding (&bindings)[PlaceholderCount]) const
        requires(PlaceholderCount != 0)
    {
        for (std::size_t i = 0; i < PlaceholderCount; i++) {
            for (std::size_t j = i + 1; j < PlaceholderCount; j++) {
                if (bindings[i].key == bindings[j].key) {
                    throw std::invalid_argument(
                        "Two bindings with the same key have been supplied to "
                        "instantiate.");
                }
            }
        }

        std::size_t size = ContentSize;
        for (const PlaceholderBinding& binding : bindings) {
            size += binding.value.size();
        }
        std::string result;
        result.reserve(size);

        std::size_t pos = 0;

        for (const PlaceholderInfo& placeholder : placeholders) {
            if (placeholder.position > pos) {
                result.append(content.data() + pos, placeholder.position - pos);
            }
            std::string_view placeholderName{
                content.data() + placeholder.position, placeholder.length};

            bool found = false;
            for (const PlaceholderBinding& binding : bindings) {
                if (binding.key == placeholderName) {
                    result.append(binding.value);
                    found = true;
                    pos = placeholder.position + placeholder.length;
                    break;
                }
            }
            if (!found) {
                throw std::invalid_argument(
                    "Couldn't instantiate because a Placeholder was not bound "
                    "to any value: " +
                    std::string(placeholderName));
            }
        }

        if (pos < end) {
            result.append(content.data() + pos, end - pos);
        }

        return result;
    }

    /**
     * @brief Convert to std::array. Requires that
     * there are no placeholders.
     *
     * @return std::array<char, ContentSize>&
     */
    constexpr operator std::array<char, ContentSize>&()
        requires(PlaceholderCount == 0)
    {
        return content;
    }

    /**
     * @brief Convert to const std::array. Requires that
     * there are no placeholders.
     *
     * @return std::array<char, ContentSize>&
     */
    constexpr operator const std::array<char, ContentSize>&() const
        requires(PlaceholderCount == 0)
    {
        return content;
    }

    /**
     * @brief Convert to std::string. Requires that
     * there are no placeholders.
     *
     * @return std::string
     */
    operator std::string() const
        requires(PlaceholderCount == 0)
    {
        return std::string(content.data(), ContentSize - 1);
    }

    /**
     * @brief Convert to std::string_view. Requires that
     * there are no placeholders.
     *
     * @return std::string_view
     */
    constexpr operator std::string_view() const
        requires(PlaceholderCount == 0)
    {
        return std::string_view(content.data(), ContentSize - 1);
    }

    /**
     * @brief The internal content. Placeholder names are embedded into the
     * document at the exact positions where their contents needs to be placed.
     *
     */
    std::array<char, ContentSize> content = {};

    /**
     * @brief The placeholders. Each placeholder stores its position within
     * the content array and its name length.
     *
     */
    std::array<PlaceholderInfo, PlaceholderCount> placeholders = {};

    /**
     * @brief The next writable position within the content array. If the
     * content is fully constructed, this must be equal to its size.
     *
     */
    std::size_t end = 0;

    /**
     * @brief The next writable position within the placeholders array. If the
     * placeholders are fully added, this must be equal to their count.
     *
     */
    std::size_t placeholderEnd = 0;
};

/**
 * @brief Takes the EvaluatedDocument as an non-type template parameter and a
 * variadic list of Bind objects, for which the placeholder name is embedded in
 * their type.
 *
 * It is verified at compile-time that there are no duplicate names, all
 * required names were passed, and no redundant names were given.
 *
 * Constructs a string from the content by replacing all placeholders with their
 * matching binding.
 *
 */
template <EvaluatedDocument doc, typename... Bindings>
std::string instantiate(Bindings... bindings) {
    static_assert(
        ([&]() consteval -> bool {
            constexpr std::string_view keys[] = {Bindings::getKey()...};
            for (std::size_t i = 0; i < sizeof...(Bindings); i++) {
                for (std::size_t j = i + 1; j < sizeof...(Bindings); j++) {
                    if (keys[i] == keys[j]) return false;
                }
            }
            return true;
        }()),
        "Two bindings with the same key have been supplied to document "
        "instantiate.");

    static_assert(
        ([&]() consteval -> bool {
            constexpr std::string_view keys[] = {Bindings::getKey()...};
            for (const PlaceholderInfo& ph : doc.placeholders) {
                std::string_view placeholderName{
                    doc.content.data() + ph.position, ph.length};
                bool found = false;
                for (std::string_view key : keys) {
                    if (key == placeholderName) {
                        found = true;
                        break;
                    }
                }

                if (!found) return false;
            }
            return true;
        }()),
        "Cannot instantiate document without bindings for all placeholders in "
        "the document.");

    static_assert(
        ([&]() consteval -> bool {
            constexpr std::string_view keys[] = {Bindings::getKey()...};
            for (std::string_view key : keys) {
                bool found = false;
                for (const PlaceholderInfo& ph : doc.placeholders) {
                    std::string_view placeholderName{
                        doc.content.data() + ph.position, ph.length};
                    if (key == placeholderName) {
                        found = true;
                        break;
                    }
                }

                if (!found) return false;
            }
            return true;
        }()),
        "A binding that does not map to any placeholder in the "
        "document has been supplied.");

    PlaceholderBinding converted[sizeof...(Bindings)] = {
        static_cast<PlaceholderBinding>(bindings)...};

    return doc.instantiate(converted);
}

}  // namespace onyx::compile