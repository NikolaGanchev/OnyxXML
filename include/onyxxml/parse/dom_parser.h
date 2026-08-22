#pragma once

#include <istream>

#include "../arena.h"
#include "../node.h"
#include "../paged_arena.h"
#include "parse/encoding_string_state.h"

namespace onyx::dynamic::parser {
class DomParser;

/**
 * @brief Holds the result of a parse - a NonOwning XML tree root and the Arena
 * it is built upon.
 *
 */
template <typename ArenaType>
class ParseResult {
    friend DomParser;

   private:
    /**
     * @brief The allocated Arena
     *
     */
    ArenaType arena;

    ParseResult();

    /**
     * @brief Construct a new Parse Result object
     *
     * @param arena
     * @param root
     */
    ParseResult(ArenaType arena, Node* root);

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
     * If a non-empty encoding string that is not "autodetect" is passed, it
     * will be validated against the declared document encoding.
     *
     * If an empty encoding string is passed,
     * the parser will presume UTF-8 until an XML declaration with an encoding
     * tag is found. It will then transcode the string to the required encoding
     * and revalidate. The transcoded new string will be passed back as the
     * second element of the return pair.
     *
     * If the encoding string "autodetect" is passed, encoding will be
     * automatically detected as described in Appendix F of the XML
     * specification
     *
     * @param input
     * @param encoding
     * @return std::pair<Arena, std::optional<std::string>>
     */
    static std::pair<Arena, EncodingStringState> parseDryRun(
        std::string_view input, std::string encoding);

   public:
    /**
     * @brief Parse an XML string
     *
     * If a non-empty encoding string that is not "autodetect" is passed, it
     * will be validated against the declared document encoding.
     *
     * If an empty encoding string is passed,
     * the parser will presume UTF-8 until an XML declaration with an encoding
     * tag is found. It will automatically transcode the document to that
     * encoding and read it.
     *
     * If the encoding string "autodetect" is passed, encoding will be
     * automatically detected as described in Appendix F of the XML
     * specification
     *
     * @param input
     * @param encoding
     * @return ParseResult
     */
    static ParseResult<Arena> parse(std::string_view input,
                                    std::string encoding = "");

    /**
     * @brief Parse an XML stream
     *
     * If a non-empty encoding string that is not "autodetect" is passed, it
     * will be validated against the declared document encoding.
     *
     * If an empty encoding string is passed,
     * the parser will presume UTF-8 until an XML declaration with an encoding
     * tag is found. It will automatically transcode the document to that
     * encoding and read it.
     *
     * If the encoding string "autodetect" is passed, encoding will be
     * automatically detected as described in Appendix F of the XML
     * specification
     *
     * @param input
     * @param encoding
     * @return NodeHandle
     */
    static ParseResult<PagedArena> parse(std::istream& input,
                                         std::string encoding = "");
};

template <typename ArenaType>
ParseResult<ArenaType>::ParseResult() : arena{0}, root{nullptr} {}

template <typename ArenaType>
ParseResult<ArenaType>::ParseResult(ArenaType arena, Node* root)
    : arena{std::move(arena)}, root{root} {}

template <typename ArenaType>
ParseResult<ArenaType>::ParseResult(ParseResult&& other)
    : arena{std::move(other.arena)} {
    this->root = other.root;
    other.root = nullptr;
}

template <typename ArenaType>
ParseResult<ArenaType>& ParseResult<ArenaType>::ParseResult::operator=(
    ParseResult&& other) {
    this->arena = std::move(other.arena);
    this->root = other.root;
    other.root = nullptr;

    return *this;
}

}  // namespace onyx::dynamic::parser