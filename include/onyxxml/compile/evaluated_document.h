#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

#include "compile_string.h"
#include "placeholder_info.h"

namespace onyx::compile {

struct PlaceholderBinding {
    std::string_view key;
    std::string_view value;
};

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

template <std::size_t ContentSize, std::size_t PlaceholderCount>
struct EvaluatedDocument {
    constexpr std::array<char, ContentSize>& getContent()
        requires(PlaceholderCount == 0)
    {
        return content;
    }

    std::string instantiate(
        const PlaceholderBinding (&bindings)[PlaceholderCount]) const
        requires(PlaceholderCount != 0)
    {
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

    constexpr operator std::array<char, ContentSize>&()
        requires(PlaceholderCount == 0)
    {
        return content;
    }

    constexpr operator const std::array<char, ContentSize>&() const
        requires(PlaceholderCount == 0)
    {
        return content;
    }

    operator std::string() const
        requires(PlaceholderCount == 0)
    {
        return std::string(content.data(), ContentSize - 1);
    }

    constexpr operator std::string_view() const
        requires(PlaceholderCount == 0)
    {
        return std::string_view(content.data(), ContentSize - 1);
    }

    std::array<char, ContentSize> content = {};
    std::array<PlaceholderInfo, PlaceholderCount> placeholders = {};
    std::size_t end = 0;
    std::size_t placeholderEnd = 0;
};

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