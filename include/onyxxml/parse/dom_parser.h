#pragma once

#include <stack>

#include "../arena.h"
#include "../node.h"

namespace onyx::dynamic::parser {
class DomParser;

/**
 * @brief Holds the result of a parse - a NonOwning XML tree root and the Arena
 * it is built upon.
 *
 */
class ParseResult {
    friend DomParser;

   private:
    /**
     * @brief The allocated Arena
     *
     */
    Arena arena;

    ParseResult();

    /**
     * @brief Construct a new Parse Result object
     *
     * @param arena
     * @param root
     */
    ParseResult(Arena arena, Node* root);

   public:
    /**
     * @brief The constructed NonOwning tree root
     *
     */
    Node* root;

    ParseResult(const ParseResult& other) = delete;
    ParseResult& operator=(const ParseResult& other) = delete;

    /**
     * @brief Move constructor
     *
     * @param other
     */
    ParseResult(ParseResult&& other);

    /**
     * @brief Move assignment operator
     *
     * @param other
     * @return ParseResult&
     */
    ParseResult& operator=(ParseResult&& other);
};

/**
 * @brief An almost-fully XML standard compliant non-validating
 * syntax-preserving UTF-8 supporting DOM parser. The parser correctly parses
 * all possible XML syntax constructs except DTDs. DTDs are read and preserved
 * literally and are NOT syntax validated. Elements like the XML declaration and
 * comments are not stripped but passed back in the ParseResult. All entities
 * and character escape sequences are expanded with exception of DTD defined
 * custom entities. All other behavior is fully standard-compliant. Newlines are
 * handled per standard rules ("\r\n", "\r" get expanded to "\n"). The parser
 * throws an exception on any non-standard XML and does not try to do error
 * correction.
 *
 */
class DomParser {
   private:
    /**
     * @brief Runs the parser, validating and allocating the exact Arena memory.
     *
     * @param input
     * @return Arena
     */
    static Arena parseDryRun(std::string_view input);

   public:
    /**
     * @brief Parse an XML string
     *
     * @param input
     * @return ParseResult
     */
    static ParseResult parse(std::string_view input);
};
}  // namespace onyx::dynamic::parser