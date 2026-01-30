#include "xpath/lexer.h"

#include <iostream>

#include "parse/helpers.h"

namespace onyx::dynamic::xpath {
Lexer::Lexer(parser::StringCursor cursor)
    : tokens{}, cursor{cursor}, axisNames{} {};

void Lexer::initAxisNames() {
    if (!axisNames.empty()) return;

    axisNames.emplace("ancestor");
    axisNames.emplace("ancestor-or-self");
    axisNames.emplace("attribute");
    axisNames.emplace("child");
    axisNames.emplace("descendant");
    axisNames.emplace("descendant-or-self");
    axisNames.emplace("following");
    axisNames.emplace("following-sibling");
    axisNames.emplace("namespace");
    axisNames.emplace("parent");
    axisNames.emplace("preceding");
    axisNames.emplace("preceding-sibling");
    axisNames.emplace("self");
}

bool Lexer::expectingOperator() {
    if (tokens.empty()) return false;

    Lexer::TokenType lastToken = tokens.back().getType();

    return lastToken != Lexer::TokenType::AT &&
           lastToken != Lexer::TokenType::DOUBLE_COLON &&
           lastToken != Lexer::TokenType::OPENING_PAREN &&
           lastToken != Lexer::TokenType::OPENING_BRACKET &&
           lastToken != Lexer::TokenType::COMMA &&
           lastToken != Lexer::TokenType::SLASH &&
           lastToken != Lexer::TokenType::DOUBLE_SLASH &&
           lastToken != Lexer::TokenType::PIPE &&
           lastToken != Lexer::TokenType::OPERATOR;
}

std::string_view captureLiteral(parser::StringCursor& cursor) {
    char quote = *cursor;
    cursor++;
    cursor.beginCapture();
    while (&cursor != '\0' && &cursor != quote) {
        ++cursor;
    }

    if (&cursor == '\0') {
        throw std::runtime_error("Reached end of XPath during literal parsing");
    }

    return cursor.getCaptured();
}

std::string_view captureNumber(parser::StringCursor& cursor) {
    bool foundDot = false;

    cursor.beginCapture();
    while (&cursor != '\0' && (&cursor == '.' || parser::isDigit(&cursor))) {
        if (&cursor == '.') {
            if (!foundDot) {
                foundDot = true;
            } else {
                break;
            }
        }

        ++cursor;
    }

    return cursor.getCaptured();
}

// Always skips whitespace at the end
const Lexer::Token& Lexer::nextToken() {
    // Invariant: begins at next token

    if (*cursor == '\0') {
        static Token emptyToken;
        emptyToken.type = Lexer::TokenType::END;
        return emptyToken;
    }

    Token token;

    switch (*cursor) {
        // Operators
        case '(': {
            token.type = Lexer::TokenType::OPENING_PAREN;
            cursor++;
            break;
        }
        case ')': {
            token.type = Lexer::TokenType::CLOSING_PAREN;
            cursor++;
            break;
        }
        case '[': {
            token.type = Lexer::TokenType::OPENING_BRACKET;
            cursor++;
            break;
        }
        case ']': {
            token.type = Lexer::TokenType::CLOSING_BRACKET;
            cursor++;
            break;
        }
        case '.': {
            if (cursor.peek(1) != '\0') {
                if (cursor.peek(1) == '.') {
                    token.type = Lexer::TokenType::TWO_DOTS;
                    // Cursor at character before second dot
                    cursor.advance(2);
                    break;
                }

                if (parser::isDigit(cursor.peek(1))) {
                    token.type = Lexer::TokenType::NUMBER;
                    // Since the dot has already been found, capture number
                    // cannot find it again
                    token.value = captureNumber(cursor);
                    cursor.bringToCapture();
                    // Cursor at first non digit character
                    break;
                }
            }

            token.type = Lexer::TokenType::DOT;
            cursor++;
            break;
        }
        case '@': {
            token.type = Lexer::TokenType::AT;
            cursor++;
            break;
        }
        case ',': {
            token.type = Lexer::TokenType::COMMA;
            cursor++;
            break;
        }
        case ':': {
            if (cursor.peek(1) != '\0') {
                if (cursor.peek(1) == ':') {
                    token.type = Lexer::TokenType::DOUBLE_COLON;
                    cursor.advance(2);
                    break;
                }
            }

            // ':' cannot appear in XPath outside the namespace wildcard
            // and namespaces
            throw std::runtime_error("Unexpected operator ':'");
        }
        case '/': {
            if (cursor.peek(1) != '\0') {
                if (cursor.peek(1) == '/') {
                    token.type = Lexer::TokenType::DOUBLE_SLASH;
                    cursor.advance(2);
                    break;
                }
            }

            token.type = Lexer::TokenType::SLASH;
            cursor++;
            break;
        }
        case '|': {
            token.type = Lexer::TokenType::PIPE;
            cursor++;
            break;
        }
        case '*': {
            if (this->expectingOperator()) {
                token.type = Lexer::TokenType::OPERATOR;
            } else {
                token.type = Lexer::TokenType::NAME_TEST;
            }
            token.value = "*";
            cursor++;
            break;
        }
        case '+':
        case '-':
        case '=': {
            token.type = Lexer::TokenType::OPERATOR;
            token.value = *cursor;
            cursor++;
            break;
        }
        case '!': {
            if (cursor.peek(1) != '\0') {
                if (cursor.peek(1) == '=') {
                    token.type = Lexer::TokenType::OPERATOR;
                    token.value = "!=";
                    cursor.advance(2);
                    break;
                }
            }

            throw std::runtime_error(
                "'!' cannot appear outside of strings and '!='");
        }
        case '<':
        case '>': {
            if (cursor.peek(1) != '\0') {
                if (cursor.peek(1) == '=') {
                    token.type = Lexer::TokenType::OPERATOR;
                    token.value = std::string(1, *cursor) + "=";
                    cursor.advance(2);
                    break;
                }
            }

            token.type = Lexer::TokenType::OPERATOR;
            token.value = *cursor;
            cursor++;
            break;
        }
        // Literal
        case '\'':
        case '"': {
            token.type = Lexer::TokenType::LITERAL;
            token.value = captureLiteral(cursor);
            cursor.bringToCapture();
            // Skip closing quote
            cursor++;
            break;
        }
        // Variable reference
        case '$': {
            token.type = Lexer::TokenType::VARIABLE_REFERENCE;
            cursor++;
            token.value = parser::readQName(cursor);
            if (token.value.empty()) {
                throw std::runtime_error(
                    "Cannot have empty variable reference");
            }
            cursor.advance(token.value.size());
            // Past last character of variable
            break;
        }
        // NameTest (excluding '*'), NodeType, FunctionName, AxisName, Number or
        // OperatorName ('and' | 'or' | 'mod' | 'div')
        default: {
            // Number
            if (parser::isDigit(*cursor)) {
                token.type = Lexer::TokenType::NUMBER;
                token.value = captureNumber(cursor);
                cursor.bringToCapture();
                // Past last digit
                break;
            }

            token.value = parser::readQName(cursor);

            // If this were end of document,
            // it should have been caught
            // in the end of the previous loop
            // it suggests the query actually had
            // characters that are not categorized as
            // possible name characters outside a literal.
            // But there is also the possibility of this being a valid case
            // readQName will return empty if it encounters a name of the type
            // NCName:<not valid NCName start character>
            // This is just the case of NCName:* and axis::child
            // These need to be checked via NCName
            if (token.value.empty()) {
                cursor.beginCapture();
                // NameTest and AxisName tests
                // NameTest has a special case (NCName ':' '*')
                // that needs to be handled here
                // Otherwise, there is a state where ':' exists as a character,
                // which is not a valid operator
                token.value = parser::readNCName(cursor);

                if (token.value.empty())
                    throw std::runtime_error("Unexpected characters 1.");

                cursor.advance(token.value.size());

                if (*cursor == ':') {
                    // NameTest special case
                    if (cursor.peek(1) == '*') {
                        token.type = Lexer::TokenType::NAME_TEST;
                        token.value += ":*";
                        cursor.advance(2);
                        // Cursor at character after *
                        break;
                    }

                    // AxisName
                    if (cursor.peek(1) == ':') {
                        initAxisNames();
                        if (!this->axisNames.contains(token.value)) {
                            throw std::runtime_error("Unknown axis name.");
                        }
                        token.type = Lexer::TokenType::AXIS_NAME;
                        // Cursor is already at the next token (first ':' of
                        // "::") and should not be increased
                        break;
                    }

                    // Something is definitely wrong
                    throw std::runtime_error(
                        "Unexpected operator ':' after name");
                }

                throw std::runtime_error("Unexpected characters 2.");
            }
            cursor.advance(token.value.size());

            // Cursor is at first position after reading a Name
            // By the spec, there can be whitespace here
            parser::skipWhitespace(cursor);

            // Disambiguation logic
            // OperatorName
            if (this->expectingOperator()) {
                if (token.value != "and" && token.value != "or" &&
                    token.value != "mod" && token.value != "div") {
                    throw std::runtime_error(
                        "Expected operator, but supplied '" + token.value +
                        "'");
                }

                token.type = Lexer::TokenType::OPERATOR;
                break;
            }

            // NodeType and FunctionName
            if (*cursor == '(') {
                std::cout << token.value << "\n";
                if (token.value == "comment" || token.value == "text" ||
                    token.value == "processing-instruction" ||
                    token.value == "node") {
                    bool isProcessingInstruction =
                        (token.value == "processing-instruction");

                    token.type = Lexer::TokenType::NODE_TYPE;
                    token.value += '(';
                    cursor++;
                    parser::skipWhitespace(cursor);

                    // Special case - processing-instruction can have a literal
                    // inside the parenthesis
                    if (isProcessingInstruction) {
                        if (*cursor == '\'' || *cursor == '"') {
                            token.value += "'";
                            // Even if this is empty, it is a special behaviour
                            // processing-instruction('') only catches
                            // processing instructions that have the target name
                            // '', so instructions like <? > (which actually
                            // isn't valid per the xml spec but XPath allows it)
                            // processing-instruction() catches any processing
                            // instruction
                            token.value += captureLiteral(cursor);
                            token.value += "'";
                            cursor.bringToCapture();
                            // The cursor is at the end quote and needs to
                            // advance
                            cursor++;
                            // There can be whitespace here
                            parser::skipWhitespace(cursor);
                        }
                    }

                    if (*cursor != ')') {
                        throw std::runtime_error(
                            "Missing closing bracket of node type");
                    }

                    token.value += ")";
                    cursor++;

                    break;
                } else {
                    // If not a node type, the token must be a function name
                    token.type = Lexer::TokenType::FUNCTION_NAME;
                    // We are at the parenthesis, and that isn't part of the
                    // function name, so the cursor shouldn't advance
                    break;
                }
            }

            // The only remaining possibility is that the Token is a NameTest
            token.type = Lexer::TokenType::NAME_TEST;
            // Cursor is at the character right after the name
            break;
        }
    }

    skipWhitespace(cursor);

    this->tokens.push_back(std::move(token));
    return this->tokens.back();
}

const std::vector<Lexer::Token>& Lexer::getTokens() const {
    return this->tokens;
}
};  // namespace onyx::dynamic::xpath