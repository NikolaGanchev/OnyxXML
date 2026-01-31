#pragma once

#include "lexer.h"
#include <memory>
#include <stack>

namespace onyx::dynamic::xpath {
/**
 * @brief Parses a list of tokens for XPath into an Abstract syntax tree iteratively
 * using Shunting yard.
 * 
 */
class Parser {
public:
    struct AstNode;
private:
    /**
     * @brief The result stack in Shunting yard.
     * 
     */
    std::stack<std::unique_ptr<AstNode>> nodeStack;
    /**
     * @brief The operator stack in Shunting yard.
     * 
     */
    std::stack<Lexer::Token*> opStack;
    /**
     * @brief A copy of the list of tokens. The parser may modify the token list internally.
     * 
     */
    std::vector<Lexer::Token> tokens;

    /**
     * @brief The current positions in the token list.
     * 
     */
    size_t pos;

    /**
     * @brief Checks if the tokens list has the given token type at an offset relPos from pos.
     * If the token list ends, throws std::runtime_error.
     * @param t 
     * @param relPos 
     * @return true The token with the given token type was found
     * @return false The token was not found
     */
    bool requireToken(Lexer::TokenType t, size_t relPos);

    /**
     * @brief Checks if the current context suggests an operator should be interpreted as unary.
     * 
     * @return true 
     * @return false 
     */
    bool shouldBeUnary();

    /**
     * @brief Pops an operation from opStack and the needed amount of nodeStack nodes to create a new AstNode.
     * 
     */
    void applyOperation();

    /**
     * @brief Get the precedence of the given token
     * 
     * @param t 
     * @return uint8_t 
     */
    uint8_t static getPrecedence(const Lexer::Token* t);
public:
    struct BinaryOp;
    struct Step;
    /**
     * @brief Node for the Abstract syntax tree.
     * 
     */
    struct AstNode {
        enum Type {
            RootNode,
            VarRef,
            Literal,
            Number,
            FunctionCall,
            FilterExpr,
            BinaryOp,
            Step,
            FunctionSentinel
        };
        virtual Type getType() const = 0;
        virtual ~AstNode() {}
    };
    struct RootNode : public AstNode {
        Type getType() const override { return Type::RootNode; }
    };
    /**
     * @brief A variable reference
     * 
     */
    struct VarRef : public AstNode {
        std::string name;
        Type getType() const override { return Type::VarRef; }
    };
    /**
     * @brief A literal
     * 
     */
    struct Literal : public AstNode {
        std::string value;
        Type getType() const override { return Type::Literal; }
    };
    /**
     * @brief A number
     * 
     */
    struct Number : public AstNode {
        std::string num;
        Type getType() const override { return Type::Number; }
    };
    /**
     * @brief A function call
     * 
     */
    struct FunctionCall : public AstNode {
        std::string name;
        std::vector<std::unique_ptr<AstNode>> args;
        Type getType() const override { return Type::FunctionCall; }
    };
    /**
     * @brief A filter expression. That is an expression with an attached predicate.
     * 
     */
    struct FilterExpr : public AstNode {
        std::unique_ptr<AstNode> subject;
        std::vector<std::unique_ptr<AstNode>> predicates;
        Type getType() const override { return Type::FilterExpr; }
    };
    /**
     * @brief A binary operation. Includes '/', '|', comparison and arithmetic operators.
     * 
     */
    struct BinaryOp : public AstNode {
        std::unique_ptr<AstNode> left;
        std::string op;
        std::unique_ptr<AstNode> right;
        Type getType() const override { return Type::BinaryOp; }
    };
    /**
     * @brief An XPath step, which contains an axis, a node test and a list of predicates.
     * 
     */
    struct Step : public AstNode {
        std::string axis;
        std::string test;
        std::vector<std::unique_ptr<AstNode>> predicates;
        Type getType() const override { return Type::Step; }
    };
    /**
     * @brief Used to signify if a function is being parsed or not.
     * Without it, an expression such as 'count(item) + (5)'
     * may be interpreted as 'count(item, 5)'
     * 
     */
    struct FunctionSentinel : public AstNode {
        Type getType() const override { return Type::FunctionSentinel; }
    };

    /**
     * @brief Construct a new Parser object. Completes the lexing process.
     * 
     * @param lexer 
     */
    Parser(Lexer& lexer);

    /**
     * @brief Parses the token list and builds the Abstract syntax tree.
     * 
     * @return std::unique_ptr<AstNode> 
     */
    std::unique_ptr<AstNode> buildAST();
};
};