#pragma once

#include <stdexcept>
#include <string>

#include "compile_string.h"
#include "placeholder_info.h"

namespace onyx::compile {

struct CompileStringUtils;
namespace ctags {
template <CompileString Name>
struct Placeholder;
};
struct PlaceholderBinding {
    std::string_view key;
    std::string_view value;
};

template <std::size_t ContentSize, std::size_t PlaceholderCount>
struct EvaluatedDocument {
    friend CompileStringUtils;
    template <CompileString Name>
    friend struct ::onyx::compile::ctags::Placeholder;

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

   private:
    std::array<char, ContentSize> content = {};
    std::array<PlaceholderInfo, PlaceholderCount> placeholders = {};
    std::size_t end = 0;
    std::size_t placeholderEnd = 0;
};

}  // namespace onyx::compile