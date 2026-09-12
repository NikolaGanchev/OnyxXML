#pragma once

#include <array>
#include <string_view>

#include "evaluated_document.h"

namespace onyx::compile {

/**
 * @brief Utilities for dealing with compile strings.
 *
 */
struct CompileStringUtils {
    /**
     * @brief At compile time, takes an std::array and copies str into it
     * starting from index. Returns the next free index for writing.
     *
     * @tparam N The size of the array
     * @param arr The array
     * @param str The string
     * @param index The starting index
     * @return size_t The next free index
     */
    template <size_t N>
    static consteval size_t placeStringInArray(std::array<char, N>& arr,
                                               const char* str, size_t index) {
        for (size_t i = 0; i < std::string_view(str).size(); i++) {
            arr[index + i] = str[i];
        }

        return index + std::string_view(str).size();
    }

    /**
     * @brief At compile time, takes an EvaluatedDocument and copies str into
     * its context starting from index. Sets end to the next free index for
     * writing.
     *
     * @return size_t The next free index
     */
    template <size_t ContentSize, size_t PlaceholderCount>
    static consteval void placeStringInEvaluatedDocument(
        EvaluatedDocument<ContentSize, PlaceholderCount>& doc,
        const char* str) {
        for (size_t i = 0; i < std::string_view(str).size(); i++) {
            doc.content[doc.end] = str[i];
            doc.end++;
        }
    }
};
}  // namespace onyx::compile