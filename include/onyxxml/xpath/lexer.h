#pragma once
#include <string>
#include <unordered_set>
#include <vector>

#include "parse/string_cursor.h"

namespace onyx::dynamic::xpath {
class Parser;
/**
 * @brief Tokenizes an XPath query into a list of Tokens. Handles basic
 * validation of the tokens.
 *
 */
class Lexer {
   public:
    class Token;

   private:
    /**
     * @brief Token list
     *
     */
    std::vector<Token> tokens;

    /**
     * @brief Cursor to read from
     *
     */
    parser::StringCursor cursor;

    /**
     * @brief Used for validating correct axis names
     *
     */
    std::unordered_set<std::string> axisNames;

    /**
     * @brief Initializes the axis names cache
     *
     */
    void initAxisNames();

    /**
     * @brief Whether the lexer is expecting an operator or not.
     * As per https://www.w3.org/TR/1999/REC-xpath-19991116/#exprlex
     * Uses 1 token lookback.
     *
     * @return true
     * @return false
     */
    bool expectingOperator();

   public:
    /**
     * @brief Contains the token types as per the grammar in
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#exprlex
     * Modifications: Slash, Double slash and pipe have been brought out of
     * the Operator type
     *
     */
    enum TokenType {
        OPENING_PAREN,    // '('
        CLOSING_PAREN,    // ')'
        OPENING_BRACKET,  // '['
        CLOSING_BRACKET,  // ']'
        DOT,              // '.'
        TWO_DOTS,         // '..'
        AT,               // '@'
        COMMA,            // ','
        DOUBLE_COLON,     // '::'
        SLASH,            // '/'
        DOUBLE_SLASH,     // '//'
        PIPE,             // '|'

        NAME_TEST,  // 	'*'	| NCName ':' '*' | QName
        NODE_TYPE,  // 'text' | 'comment' | 'processing-instruction' | 'node'
        OPERATOR,   // 'and' | 'or' | 'mod' | 'div' | '+' | '-' | '=' | '!=' |
                    // '<' | '<=' | '>' | '>='
        FUNCTION_NAME,       // QName - NodeType
        AXIS_NAME,           // 	'ancestor'
                             // | 'ancestor-or-self'
                             // | 'attribute'
                             // | 'child'
                             // | 'descendant'
                             // | 'descendant-or-self'
                             // | 'following'
                             // | 'following-sibling'
                             // | 'namespace'
                             // | 'parent'
                             // | 'preceding'
                             // | 'preceding-sibling'
                             // | 'self'
        LITERAL,             // "' [^"]* '"'	| "'" [^']* "'"
        NUMBER,              // Digits ('.' Digits?)? | '.' Digits
        VARIABLE_REFERENCE,  // 	'$' QName
        END,

        UNARY_MINUS  // Used by parser. Not emitted by Lexer
    };

    /**
     * @brief A token contain a TokenType and a string value
     *
     */
    class Token {
        friend Parser;

       private:
        /**
         * @brief The TokenType
         *
         */
        TokenType type;

        /**
         * @brief The string value
         *
         */
        std::string value;

        Token() : type{TokenType::END}, value{""} {}

        friend Lexer;

       public:
        /**
         * @brief Construct a new Token object
         *
         * @param type
         * @param value
         */
        Token(TokenType type, std::string value) : type{type}, value{value} {}

        /**
         * @brief Get the Type
         *
         * @return TokenType
         */
        TokenType getType() const { return type; }

        /**
         * @brief Get the Value
         *
         * @return const std::string_view
         */
        const std::string_view getValue() const { return value; }
    };

    /**
     * @brief Construct a new Lexer object
     *
     * @param cursor
     */
    Lexer(parser::StringCursor cursor);

    /**
     * @brief Evaluates and returns a reference to the next token.
     * Tokenization based on
     * https://www.w3.org/TR/1999/REC-xpath-19991116/#exprlex
     * When the cursor is exhausted, starts returning a Token with the TokenType
     * End and an empty string value. This token is not added to the internal
     * tokens vector.
     *
     * @return const Token&
     */
    const Token& nextToken();

    /**
     * @brief Returns the list of all tokens found. Does not include the token
     * with TokenType End.
     *
     */
    const std::vector<Token>& getTokens() const;
};
};  // namespace onyx::dynamic::xpath