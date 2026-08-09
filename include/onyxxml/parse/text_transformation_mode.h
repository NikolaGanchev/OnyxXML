#pragma once

namespace onyx::dynamic::parser {
/**
 * @brief Indicates the manner of transforming text in the parser.
 *
 */
enum class TextTransformationMode {
    /**
     * @brief Expands entities and handles EOL normalization (turns '\r\n' and
     * '\r' into '\n')
     *
     */
    TEXT,
    /**
     * @brief Expands entities and handles EOL attribute normalization (turns
     * '\r\n', '\r', '\t', '\n' into ' ' (whitespace) into '\n')
     *
     */
    ATTRIBUTE,
    /**
     * @brief Only handles EOL normalization (turns '\r\n' and '\r' into '\n')
     *
     */
    EOL_ONLY,
    /**
     * @brief No transformation required or applicable
     *
     */
    NONE
};
}  // namespace onyx::dynamic::parser