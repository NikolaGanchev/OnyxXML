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

        return escape(str, escapeTable, escapeMultiByte);
    }


    // Escapes a given string.
    // Note: non-ASCII character escaping is disabled by default
    std::string escape(const std::string& str, const std::array<const char*, 128>& escapeTable, bool escapeMultiByte) {
        // A Queue to hold numeric HTML entities for multi-byte (non-ASCII) Unicode characters.
        std::queue<uint32_t> codepointSequence;
        std::unordered_map<uint32_t, std::string> dictionary{};

        // Calculate the total size required for the escaped string.
        // This pre-calculation helps in allocating the exact amount of memory needed.
        size_t escapedSize = 0;
        bool safe = true;

        for (size_t i = 0; i < str.size(); i++) {
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
                    codepointSequence.emplace(codepoint);
                    safe = false;
                }
            }
        }

        if (safe) return std::string(str);

        // Create a new string with the computed size.
        // Initialize with null characters as placeholders.
        std::string escaped(escapedSize, '\0');

        // Setup pointers for reading the input string and writing to the escaped string.
        const char* read = str.data();
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
                    codepointSequence.pop();
                    // Write the entity into the output.
                    for (size_t i = 0; i < entity.size(); i++) {
                        *write = entity[i];
                        write++;
                    }
                }
            }
        }

        // Adjust the final string size based on how many characters were written.
        // This trims any extra null characters.
        escaped.resize(write - escaped.data());

        return escaped;
    }

    std::string expandEntities(std::string_view input) {
        std::string output;
        output.reserve(input.size() * 6 / 5);
        static constexpr std::array<std::pair<std::string_view, char32_t>, 5> namedEntities = {{
                {"lt",   '<'},
                {"gt",   '>'},
                {"amp",  '&'},
                {"apos", '\''},
                {"quot", '"'}
            }};

        const char* pos = input.data();
        const char* end = pos + input.size();
        const char* lastCopyPos = pos; // Pointer to the start of the current chunk of characters

        while (pos < end) {
            if (*pos == '&') {
                // Copy the segment of characters found before the '&'
                if (pos > lastCopyPos) {
                    output.append(lastCopyPos, pos - lastCopyPos);
                }

                const char* lookAhead = pos + 1;
                // Max entity length: &#x10FFFF; (9 chars + x), &apos; (4 chars)
                // Entities most probably shouldn't go for more than 12 characters
                const char* searchLimit = std::min(end, lookAhead + 12);
                const char* semicolonPos = std::find(lookAhead, searchLimit, ';');

                if (semicolonPos != searchLimit && *semicolonPos == ';') {
                    // An entity candidate was found: `&token;`
                    std::string_view token(lookAhead, semicolonPos - lookAhead);

                    for (auto const& [name, code]: namedEntities) {
                        if (token == name) {
                            // UTF-8 encode code point 0x00--0x7F
                            output.push_back(static_cast<char>(code));
                            pos = semicolonPos + 1;
                            lastCopyPos = pos;
                            goto cont;
                        }
                    }

                    // Numeric Entity
                    if (!token.empty() && token[0] == '#') {
                        bool isHex = token.length() > 1 && (token[1] == 'x' || token[1] == 'X');
                        uint32_t codepoint = 0;
                        const char* numStartPtr = token.data() + (isHex ? 2 : 1);
                        const char* numEndPtr = token.data() + token.length();
                        bool validNumericFormat = true;

                        if (numStartPtr >= numEndPtr) { // Empty numeric part (e.g., &#; or &#x;)
                            validNumericFormat = false;
                        } else {
                            for (const char* currentNumChar = numStartPtr; currentNumChar < numEndPtr; ++currentNumChar) {
                                char c = *currentNumChar;
                                uint32_t digit = 0;
                                if (c >= '0' && c <= '9') {
                                    digit = c - '0';
                                } else if (isHex && c >= 'a' && c <= 'f') {
                                    digit = 10 + (c - 'a');
                                } else if (isHex && c >= 'A' && c <= 'F') {
                                    digit = 10 + (c - 'A');
                                } else {
                                    validNumericFormat = false;
                                    break;
                                }

                                // Basic overflow check
                                // This check primarily prevents overflow if parsing garbage input
                                // that would result in a very large number before hitting non-digit.
                                if (isHex) {
                                    if (codepoint > (0x10FFFF / 16)) { validNumericFormat = false; break; }
                                    codepoint = (codepoint << 4) | digit;
                                } else {
                                    if (codepoint > (0x10FFFF / 10)) { validNumericFormat = false; break; }
                                    codepoint = codepoint * 10 + digit;
                                }
                            }
                        }

                        if (validNumericFormat) {
                            // Convert entity to unicode
                            char buf[4]; // Max 4 bytes for UTF-8
                            char bytesWritten = 0;
                            if (codepoint <= 0x7F) {
                                buf[0] = static_cast<char>(codepoint);
                                bytesWritten = 1;
                            } else if (codepoint <= 0x7FF) {
                                buf[0] = static_cast<char>(0xC0 | (codepoint >> 6));
                                buf[1] = static_cast<char>(0x80 | (codepoint & 0x3F));
                                bytesWritten = 2;
                            } else if (codepoint <= 0xFFFF) {
                                // Exclude surrogate pairs (0xD800-0xDFFF) as they are not valid Unicode scalar values
                                if (!(codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
                                    buf[0] = static_cast<char>(0xE0 | (codepoint >> 12));
                                    buf[1] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                                    buf[2] = static_cast<char>(0x80 | (codepoint & 0x3F));
                                    bytesWritten = 3;
                                }
                            } else if (codepoint <= 0x10FFFF) {
                                buf[0] = static_cast<char>(0xF0 | (codepoint >> 18));
                                buf[1] = static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
                                buf[2] = static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                                buf[3] = static_cast<char>(0x80 | (codepoint & 0x3F));
                                bytesWritten = 4;
                            }

                            if (bytesWritten > 0) {
                                output.append(buf, bytesWritten);
                                pos = semicolonPos + 1; // Advance past the entity
                                lastCopyPos = pos; // Reset last_copy_pos
                                goto cont;
                            }
                        } else {
                            // There was &, but no ; was found. This is a critical mistake requiring an exception.
                            throw std::invalid_argument("& outside of entities not allowed.");
                        }
                    } else {
                        // There was &, but no ; was found. This is a critical mistake requiring an exception.
                        throw std::invalid_argument("& outside of entities not allowed.");
                    }

                    // If no named or valid numeric entity was found, copy the original text including '&'
                    output.append(pos, semicolonPos + 1 - pos);

                    // Advance past the entity
                    pos = semicolonPos + 1; 
                    lastCopyPos = pos;
                    goto cont;
                } else {
                    // There was &, but no ; was found. This is a critical mistake requiring an exception.
                    throw std::invalid_argument("& outside of entities not allowed.");
                }
            } else if (*pos == '\r') {
                // Copy the segment of normal characters found before the '\r'
                if (pos > lastCopyPos) {
                    output.append(lastCopyPos, pos - lastCopyPos);
                }
                output.push_back('\n');
                pos++;
                if (pos < end && *pos == '\n') { // Check boundary for p before dereferencing
                    pos++;
                }
                lastCopyPos = pos;
                goto cont;
            }
            // If it's a normal character, just advance the pointer for later bulk copy
            pos++;

            cont:; // Label for goto
        }

        // After the loop, copy any remaining characters
        if (pos > lastCopyPos) {
            output.append(lastCopyPos, pos - lastCopyPos);
        }

        return output;
    }

    // Returns the Unicode codepoint for a UTF-8 encoded character starting at 'read'.
    // Handles 1-, 2-, 3-, and 4-byte sequences.
    uint32_t getUnicodeCodepoint(const char* read) {
        uint32_t codepoint = 0;

        // 1-byte ASCII character (0xxxxxxx)
        if ((unsigned char)*read < 128) { // 1-byte ASCII

            codepoint = (unsigned char) *read;
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

    std::string escapeMultiByte(const std::string& str, bool escapeMultiByte) {
        if (!escapeMultiByte) return std::string(str);

        static constexpr std::array<const char*, 128> emptyEscapeTable = []() {
            std::array<const char*, 128> table{};
            return table;
        }();

        return escape(str, emptyEscapeTable, escapeMultiByte);
    }
    
    std::string escapeSequence(const std::string& str, const char* sequence) {
        std::string escapeSeq = "";

        const char* seqPos = sequence;
        while (*seqPos != '\0') {
            escapeSeq += numericEntity(getUnicodeCodepoint(seqPos));
            seqPos++;
        }

        return replaceSequence(str, sequence, escapeSeq);
    }

    std::string replaceSequence(const std::string& str, const char* sequence, std::string_view replaceSequence) {
        // Calculate the total size required for the escaped string.
        // This pre-calculation helps in allocating the exact amount of memory needed.
        size_t escapedSize = 0;
        bool safe = true;

        for (size_t i = 0; i < str.size(); i++) {
            if ((unsigned char)str[i] == *sequence) {
                const char* seqPos = sequence;
                int j = i;
                while (j < str.size() && *seqPos != '\0' && str[j] == *seqPos) {
                    j++;
                    seqPos++;
                }
                if (*seqPos == '\0') {
                    i = j - 1;
                    safe = false;
                    escapedSize += replaceSequence.size();
                    continue;
                }
            }
            escapedSize++;
        }

        if (safe) return std::string(str);

        // Create a new string with the computed size.
        // Initialize with null characters as placeholders.
        std::string escaped(escapedSize, '\0');

        // Setup pointers for reading the input string and writing to the escaped string.
        const char* read = str.data();
        char* write = escaped.data();

        // Process the input string until we reach its null terminator.
        while (*read != '\0') {
            if ((unsigned char)*read == *sequence) {
                const char* seqPos = sequence;
                const char* readSequenceCandidate = read;
                while (*readSequenceCandidate != '\0' && *seqPos != '\0' && *readSequenceCandidate == *seqPos) {
                    readSequenceCandidate++;
                    seqPos++;
                }
                if (*seqPos == '\0') {
                    read = readSequenceCandidate;
                    safe = false;
                    for (size_t i = 0; i < replaceSequence.size(); i++) {
                        *write = replaceSequence[i];
                        write++;
                    }
                    continue;
                }
            }
            *write = *read;
            write++;
            read++;
        }

        // Adjust the final string size based on how many characters were written.
        // This trims any extra null characters.
        escaped.resize(write - escaped.data());

        return escaped;
    }

    std::string replaceSequences(const std::string& str, const std::vector<std::pair<std::string_view, std::string_view>>& dictionary) {
        // Calculate the total size required for the escaped string.
        // This pre-calculation helps in allocating the exact amount of memory needed.
        size_t escapedSize = 0;
        bool safe = true;

        bool foundMatch = true;
        for (size_t i = 0; i < str.size(); i++) {
            for (const auto& [key, replacement] : dictionary) {
                foundMatch = true;
                if (key == "") continue;
                size_t k = i;
                for (size_t j = 0; j < key.size(); j++, k++) {
                    if (k >= str.size() || (unsigned char)str[k] != key[j]) {   
                        foundMatch = false;
                        break;
                    }
                }   
                if (foundMatch) {
                    i = k - 1;
                    safe = false;
                    escapedSize += replacement.size();
                    break;
                }
            }
            if (!foundMatch) {
                escapedSize++;
            }
        }

        if (safe) return std::string(str);

        // Create a new string with the computed size.
        // Initialize with null characters as placeholders.
        std::string escaped(escapedSize, '\0');

        // Setup pointers for reading the input string and writing to the escaped string.
        const char* read = str.data();
        char* write = escaped.data();

        // Process the input string until we reach its null terminator.
        while (*read != '\0') {
            for (const auto& [key, replacement] : dictionary) {
                foundMatch = true;
                if (key == "") continue;
                const char* readSequenceCandidate = read;
                for (size_t j = 0; j < key.size(); j++, readSequenceCandidate++) {
                    if (*readSequenceCandidate == '\0' || (unsigned char) *readSequenceCandidate != key[j]) {   
                        foundMatch = false;
                        break;
                    }
                }   
                if (foundMatch) {
                    read = readSequenceCandidate;
                    for (size_t i = 0; i < replacement.size(); i++) {
                        *write = replacement[i];
                        write++;
                    }
                    break;
                }
            }
            if (!foundMatch) {
                *write = *read;
                write++;
                read++;
            }
        }

        // Adjust the final string size based on how many characters were written.
        // This trims any extra null characters.
        escaped.resize(write - escaped.data());

        return escaped;
    }
}