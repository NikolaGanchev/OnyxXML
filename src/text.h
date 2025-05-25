#pragma once

#include "node.h"

namespace Templater::dynamic {
    namespace text {
        /**
         * @brief Escape a string from XML injection causing elements.
         * By default only escapes the crucial XML injection characters: <, >, ", ', &.
         * It can also escape unicode sequences if the escapeMultiByte argument is set to true.
         * Unicode sequences can be unsafe in environments that do not support UTF-8 and can be used to create injections. 
         * Unicode escaping escapes any unicode sequence to an XML/HTML entity. 
         * Due to the runtime cost and overall low risk this is false by default.
         * 
         * @param str 
         * @param escapeMultiByte Whether Unicode characters should be escaped
         * @return std::string 
         */
        std::string escape(const std::string& str, bool escapeMultiByte = false);


        /**
         * @brief Only escapes unicode sequences if escapeMultiByte is true. Otherwise returns a copy of the string.
         * 
         * @param escapeMultiByte 
         * @return std::string 
         */
        std::string escapeMultiByte(const std::string& str, bool escapeMultiByte);


        /**
         * @brief For a given unicode codepoint returns the appropriate XML/HTML entity.
         * 
         * @param codepoint 
         * @return std::string 
         */
        std::string numericEntity(uint32_t codepoint);
    

        /**
         * @brief Get the Unicode codepoint for a sequence of unicode characters. If the sequence is invalid, returns 0. Does not modify the original pointer.
         * 
         * @param read 
         * @return uint32_t The codepoint if the sequence is valid, otherwise 0.
         */
        uint32_t getUnicodeCodepoint(const char* read);

        /**
         * @brief Escape a string from XML injection causing elements.
         * This function does not provide the escape elements, they must be passed in escapeTable.
         * It can also escape unicode sequences if the escapeMultiByte argument is set to true.
         * Unicode sequences can be unsafe in environments that do not support UTF-8 and can be used to create injections. 
         * Unicode escaping escapes any unicode sequence to an XML/HTML entity. 
         * Due to the runtime cost and overall low risk this is false by default.
         * For maximum performance, the escapeTable should be constexpr.
         * 
         * 
         * @param str 
         * @param escapeTable
         * @param escapeMultiByte Whether Unicode characters should be escaped
         * @return std::string 
         */
        std::string escape(const std::string& str, const std::array<const char*, 128>& escapeTable, bool escapeMultiByte = false);


        /**
         * @brief Escapes all occurrences the given sequence with entities in the whole string.
         * 
         * @param str 
         * @param sequence 
         * @return std::string 
         */
        std::string escapeSequence(const std::string& str, const char* sequence);


        /**
         * @brief Replaces all occurrences the given sequence with the replace sequence in the whole string.
         * 
         * @param str 
         * @param sequence 
         * @param replaceSequence
         * @return std::string 
         */
        std::string replaceSequence(const std::string& str, const char* sequence, std::string_view replaceSequence);


        /**
         * @brief Replaces all occurrences the given sequences with the replace sequences in the whole string.
         * Order of replacement is based on the ordering of the map.
         * 
         * @param str 
         * @param dictionary 
         * @return std::string 
         */
        std::string replaceSequences(const std::string& str, const std::vector<std::pair<std::string_view, std::string_view>>& dictionary);

        
        /**
         * @brief Expands XML entities in a text string.
         * 
         * @param input 
         * @return std::string 
         */
        std::string expandEntities(std::string_view input);


        /**
         * @brief Converts a codepoint into a multibyte string.
         * 
         * @param codePoint 
         * @return std::string 
         */
        std::string encodeUtf8(uint32_t codePoint);


        /**
         * @brief Converts and XML entity into a multibyte string.
         * 
         * @param entity 
         * @return std::string 
         */
        std::string decodeNumericEntity(const std::string& entity);
    }
}