#pragma once

#include "node.h"

namespace Templater::dynamic {
    namespace text {
        std::string escape(const std::string& str, bool escapeMultiByte = false);

        std::string numericEntity(uint32_t codepoint);
    
        uint32_t getUnicodeCodepoint(const char* read);
    }
}