#pragma once

#include <array>
#include <string_view>

namespace Templater::compile {
    
    struct CompileStringUtils {
        template <size_t N>
        static consteval size_t placeStringInArray(std::array<char, N>& arr, const char* str, size_t index) {
            for (size_t i = 0; i < std::string_view(str).size(); i++) {
                arr[index + i] = str[i];
            }
        
            return index + std::string_view(str).size();
        }
    };
}