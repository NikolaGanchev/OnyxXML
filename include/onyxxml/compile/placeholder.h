#pragma once

#include <array>
#include <stdexcept>
#include <string_view>

#include "onyxxml/compile/compile_string.h"
#include "onyxxml/compile/compile_string_utils.h"
#include "onyxxml/compile/placeholder_info.h"


namespace onyx::compile::ctags {

/**
 * @brief A compile time Placeholder struct.
 * Used to mark named places in a compile-time document where later dynamic
 * content should be inserted. Placeholder strings should be unique.
 *
 * @tparam Str id
 */
template <CompileString Name>
struct Placeholder {
   public:
    static consteval std::size_t placeholderCount() { return 1; }

    static consteval std::string_view getPlaceholderName() {
        return std::string_view(Name.value, Name.size);
    }

    /**
     * @brief The compile-time size of the Placeholder; always 0.
     *
     * @return std::size_t
     */
    static consteval std::size_t size() { return Name.size - 1; }

    /**
     * @brief Throws as Placeholders cannot be individually serialized.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval std::array<char, size() + 1> serialize() {
        throw "Cannot serialize placeholder";
    }

    /**
     * @brief Evaluates the Placeholder into an existing EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        PlaceholderInfo info{};
        info.position = result.end;
        info.length = Name.size - 1;

        CompileStringUtils::placeStringInEvaluatedDocument(result, Name.value);

        result.placeholders[result.placeholderEnd] = info;
        result.placeholderEnd++;
    }

    /**
     * @brief Dynamic trees cannot be initialized with a placeholder. Throws an
     * std::logic_error exception.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        throw std::logic_error(
            "Cannot turn compile-time Placeholder into dynamic tree");
    }
};
}  // namespace onyx::compile::ctags