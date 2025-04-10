#include "text.h"

#include <unordered_map>
#include <sstream>
#include <queue>

namespace Templater::dynamic::text {
    // Escapes a given string so that it is safe for use in HTML contexts.
    // This function replaces reserved HTML characters with their corresponding entities
    // and converts non-ASCII characters to numeric HTML entities if enabled
    // Note: non-ASCII character escaping is disabled by default
    std::string escape(const std::string& str, bool escapeMultiByte) {

        // Table of characters that must be escaped in HTML to their corresponding entity strings to keep HTML integrity
        static constexpr std::array<const char*, 128> escapeTable = []() {
            std::array<const char*, 128> table{};
            table['&'] = "&amp;";
            table['<'] = "&lt;";
            table['>'] = "&gt;";
            table['\"'] = "&quot;";
            table['\''] = "&#39;";
            return table;
        }();

        // A Queue to hold numeric HTML entities for multi-byte (non-ASCII) Unicode characters.
        std::vector<uint32_t> codepointSequence;
        std::unordered_map<uint32_t, std::string> dictionary{};

        // Calculate the total size required for the escaped string.
        // This pre-calculation helps in allocating the exact amount of memory needed.
        size_t escapedSize = 0;
        bool safe = true;

        for (int i = 0; i < str.size(); i++) {
            // If the current character can cause injections
            if ((unsigned char)str[i] < 128 && escapeTable[(unsigned char)str[i]]) {
                // Add the length of its escape sequence to the total size.
                escapedSize += strlen(escapeTable[(unsigned char)str[i]]);
                safe = false;
            } else {
                // If multi-byte escaping is disabled, just increment
                if (!escapeMultiByte) {
                    escapedSize++;
                }
                // Get the full Unicode codepoint starting from the current character pointer.
                uint32_t codepoint = getUnicodeCodepoint(&str[i]);
                if (codepoint < 128) {
                    // ASCII characters require one character in the output.
                    escapedSize += 1;
                } else {
                    // For non-ASCII characters, convert the codepoint to a numeric HTML entity.
                    if (!dictionary.contains(codepoint)) {
                        // Save the computed HTML entity in a queue to use later when outputting.
                        dictionary[codepoint] = numericEntity(codepoint);
                    }
                    escapedSize += dictionary[codepoint].size();
                    codepointSequence.emplace_back(codepoint);
                    safe = false;
                }
            }
        }

        if (safe) return std::string(str);

        // Create a new string with the computed size.
        // Initialize with null characters as placeholders.
        std::string escaped(escapedSize, '\0');

        // Setup pointers for reading the input string and writing to the escaped string.
        const char* read = &str[0];
        char* write = escaped.data();
        const char* escapeSequence = 0;

        // Process the input string until we reach its null terminator.
        while (*read != '\0') {
            // If the current character can cause injections
            if ((unsigned char)*read < 128 && escapeTable[(unsigned char)*read]) {
                // Get the escape sequence for the character.
                escapeSequence = escapeTable[*read];
                // Write the escape sequence into the output.
                while (*escapeSequence != '\0') {
                    *write = *escapeSequence;
                    escapeSequence++;
                    write++;
                }
                // Move the read pointer past the escaped character.
                read++;
            } else {
                // If the character is ASCII (single-byte) or multi-byte escaping is disabled, copy it directly.
                if ((unsigned char)*read < 128 || !escapeMultiByte) {
                    *write = *read;
                    read++;
                    write++;
                } else {
                    // For multi-byte characters (non-ASCII), we need to advance the read pointer
                    // based on the size of the UTF-8 sequence.
                    if (((unsigned char)*read >> 5) == 0x6) { // 2-byte sequence
                    
                        if (*(read + 1) != '\0') {
                            read += 2;
                        }
                    } else if (((unsigned char)*read >> 4) == 0xE) { // 3-byte sequence
                    
                        if (*(read + 2) != '\0') {
                            read += 3;
                        }
                    } else if (((unsigned char)*read >> 3) == 0x1E) { // 4-byte sequence
                    
                        if (*(read + 3) != '\0') {
                            read += 4;
                        }
                    }

                    // Retrieve the pre-computed HTML entity for this Unicode character.
                    const std::string& entity = dictionary[codepointSequence.front()];
                    codepointSequence.pop_back();
                    // Write the entity into the output.
                    for (int i = 0; i < entity.size(); i++) {
                        *write = entity[i];
                        write++;
                    }
                }
            }
        }

        // Adjust the final string size based on how many characters were written.
        // This trims any extra null characters.
        escaped.resize(write - &escaped[0]);

        return escaped;
    }

    // Returns the Unicode codepoint for a UTF-8 encoded character starting at 'read'.
    // Handles 1-, 2-, 3-, and 4-byte sequences.
    uint32_t getUnicodeCodepoint(const char* read) {
        uint32_t codepoint = 0;

        // 1-byte ASCII character (0xxxxxxx)
        if ((unsigned char)*read < 128) { // 1-byte ASCII

            codepoint = *read;
        }   
        // 2-byte sequence (110xxxxx 10xxxxxx) 
        else if (((unsigned char)*read >> 5) == 0x6) { // 2-byte sequence

            if (*(read + 1) != '\0') {
                codepoint = (((unsigned char)*read & 0x1F) << 6) | ((unsigned char)*(read + 1) & 0x3F);
            }
        } 
        // 3-byte sequence (1110xxxx 10xxxxxx 10xxxxxx)
        else if (((unsigned char)*read >> 4) == 0xE) { // 3-byte sequence

            if (*(read + 2) != '\0') {
                codepoint = (((unsigned char)*read & 0x0F) << 12) |
                            (((unsigned char)*(read + 1) & 0x3F) << 6) |
                            ((unsigned char)*(read + 2) & 0x3F);
            }
        }
        // 4-byte sequence (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
        else if (((unsigned char)*read >> 3) == 0x1E) { // 4-byte sequence

            if (*(read + 3) != '\0') {
                codepoint = (((unsigned char)*read & 0x07) << 18) |
                            (((unsigned char)*(read + 1) & 0x3F) << 12) |
                            (((unsigned char)*(read + 2) & 0x3F) << 6) |
                            ((unsigned char)*(read + 3) & 0x3F);
            }
        }

        return codepoint;
    }

    // Converts a Unicode codepoint into its corresponding numeric HTML entity string.
    // For example, codepoint 160 becomes "&#xa0;".
    std::string numericEntity(uint32_t codepoint) {
        std::ostringstream oss;
        oss << "&#x" << std::hex << codepoint << ";";
        return oss.str();
    }
}