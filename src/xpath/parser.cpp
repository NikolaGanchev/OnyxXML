#include "xpath/parser.h"
#include <stack>

namespace onyx::dynamic::xpath {
bool startsStep(const Lexer::Token& t) {
    return t.getType() == Lexer::TokenType::AXIS_NAME ||
           t.getType() == Lexer::TokenType::AT ||
           t.getType() == Lexer::TokenType::DOT || 
           t.getType() == Lexer::TokenType::TWO_DOTS ||
           t.getType() == Lexer::TokenType::NAME_TEST ||
           t.getType() == Lexer::TokenType::NODE_TYPE;
}

bool Parser::requireToken(Lexer::TokenType t, size_t relPos) {
    if (pos + relPos >= tokens.size()) {
        throw std::runtime_error("Unexpected end of token list");
    }

    return tokens[pos + relPos].getType() == t;
}

bool Parser::shouldBeUnary() {
    if (pos == 0) return true;
    Lexer::TokenType prev = tokens[pos-1].getType();
    return prev == Lexer::TokenType::OPERATOR || 
           prev == Lexer::TokenType::SLASH ||
           prev == Lexer::TokenType::DOUBLE_SLASH ||
           prev == Lexer::TokenType::PIPE || 
           prev == Lexer::TokenType::OPENING_PAREN ||
           prev == Lexer::TokenType::OPENING_BRACKET ||
           prev == Lexer::TokenType::COMMA;
}

void Parser::applyOperation() {
    if (opStack.top()->getType() == Lexer::TokenType::UNARY_MINUS) {
        if (nodeStack.empty()) {
            throw std::runtime_error("Unexpected end of node stack");
        }
        std::unique_ptr<AstNode> expr = std::move(nodeStack.top()); 
        nodeStack.pop();

        std::unique_ptr<Number> zero = std::make_unique<Number>();
        zero->num = "0";
        std::unique_ptr<BinaryOp> binary = std::make_unique<BinaryOp>();
        binary->left = std::move(zero);
        binary->op = "-";
        binary->right = std::move(expr);

        nodeStack.push(std::move(binary));
        return;
    }

    if (nodeStack.size() < 2) {
        throw std::runtime_error("Unexpected end of node stack");
    }
    std::unique_ptr<AstNode> right = std::move(nodeStack.top()); 
    nodeStack.pop();
    std::unique_ptr<AstNode> left = std::move(nodeStack.top()); 
    nodeStack.pop();

    
    if (opStack.top()->getType() == Lexer::TokenType::DOUBLE_SLASH) {
        std::unique_ptr<Step> hiddenStep = std::make_unique<Step>();
        hiddenStep->axis = "descendant-or-self"; 
        hiddenStep->test = "node()"; 

        std::unique_ptr<BinaryOp> firstSlash = std::make_unique<BinaryOp>();
        firstSlash->left = std::move(left);
        firstSlash->op = "/";
        firstSlash->right = std::move(hiddenStep);

        std::unique_ptr<BinaryOp> finalSlash = std::make_unique<BinaryOp>();
        finalSlash->left = std::move(firstSlash);
        finalSlash->op = "/";
        finalSlash->right = std::move(right);

        nodeStack.push(std::move(finalSlash));
        return;
    }

    std::unique_ptr<BinaryOp> node = std::make_unique<BinaryOp>();
    node->left = std::move(left);
    if (opStack.top()->getType() == Lexer::TokenType::SLASH) {
        node->op = "/";
    } else if (opStack.top()->getType() == Lexer::TokenType::PIPE) {
        node->op = "|";
    } else {
        node->op = opStack.top()->getValue(); 
    }
    node->right = std::move(right);
    nodeStack.push(std::move(node));
}

uint8_t Parser::getPrecedence(const Lexer::Token* t) {
    Lexer::TokenType type = t->getType();
    if (type == Lexer::TokenType::OPENING_PAREN || 
        type == Lexer::TokenType::OPENING_BRACKET) {
        return 0;
    }

    if (type == Lexer::TokenType::OPERATOR) {
        std::string_view v = t->getValue();
        if (v == "or") return 1;
        if (v == "and") return 2;
        if (v == "=" || v == "!=") return 3;
        if (v == "<" || v == "<=" || v == ">" || v == ">=") return 4;
        if (v == "+" || v == "-") return 5;
        if (v == "*" || v == "div" || v == "mod") return 6;
    }

    if (type == Lexer::TokenType::UNARY_MINUS) {
        return 7;
    }

    if (type == Lexer::TokenType::PIPE) {
        return 8;
    }

    if (type == Lexer::TokenType::SLASH || 
        type == Lexer::TokenType::DOUBLE_SLASH) {
        return 9;
    }

    throw std::runtime_error("Unknown operator encountered during parsing");

    return -1;
}

Parser::Parser(Lexer& lexer) : nodeStack{}, opStack{}, tokens{}, pos{0} {
    while (lexer.nextToken().getType() != Lexer::TokenType::END) {}

    this->tokens = lexer.getTokens();
}

std::unique_ptr<Parser::AstNode> Parser::buildAST() {
    using TokenType = Lexer::TokenType;

    bool inStep;
    while (pos < this->tokens.size()) {
        Lexer::Token& t = this->tokens[pos];

        if (t.getType() == TokenType::LITERAL) {

            std::unique_ptr<Literal> node = std::make_unique<Literal>();
            node->value = t.getValue();
            this->nodeStack.push(std::move(node));
            pos++;
        } 
        else if (t.getType() == TokenType::NUMBER) {
            std::unique_ptr<Number> node = std::make_unique<Number>();
            node->num = t.getValue();
            this->nodeStack.push(std::move(node));
            pos++;
        } 
        else if (t.getType() == Lexer::TokenType::VARIABLE_REFERENCE) {
            std::unique_ptr<VarRef> node = std::make_unique<VarRef>();
            node->name = t.getValue();
            nodeStack.push(std::move(node));
            pos++;
        } 
        else if (startsStep(t)) {
            std::unique_ptr<Step> node = std::make_unique<Step>();

            if (t.getType() == TokenType::AT) {
                node->axis = "attribute";
                if (requireToken(TokenType::NAME_TEST, 1)) {
                    pos++;
                    node->test = tokens[pos].getValue();
                } else {
                    throw std::runtime_error("Unknown operand after @");
                }
            } 
            else if (t.getType() == TokenType::DOT) {
                node->axis = "self";
                node->test = "node()";
            }
            else if (t.getType() == TokenType::TWO_DOTS) {
                node->axis = "parent";
                node->test = "node()";
            } 
            else if (t.getType() == TokenType::NAME_TEST
                        || t.getType() == TokenType::NODE_TYPE) {
                node->axis = "child";
                node->test = t.getValue();
            }
            else if (t.getType() == TokenType::AXIS_NAME) {
                node->axis = t.getValue();
                if (!requireToken(TokenType::DOUBLE_COLON, 1)) {
                    throw std::runtime_error("Invalid step expression: missing '::' after axis name");
                }
                pos++;
                if (!requireToken(TokenType::NODE_TYPE, 1)
                    && !requireToken(TokenType::NAME_TEST, 1)) {
                    throw std::runtime_error("Missing node test after axis name");
                }
                pos++;
                node->test = tokens[pos].getValue();
            }

            pos++;
            nodeStack.push(std::move(node));
        } 
        else if (t.getType() == TokenType::FUNCTION_NAME) {
            std::unique_ptr<FunctionCall> node = std::make_unique<FunctionCall>();
            node->name = t.getValue();

            if (!requireToken(TokenType::OPENING_PAREN, 1)) {
                throw std::runtime_error("Function call requires arguments");
            }
            pos++;
            nodeStack.push(std::move(node));
            nodeStack.push(std::make_unique<FunctionSentinel>());
        }
        else if (t.getType() == Lexer::TokenType::OPENING_PAREN) {
            opStack.push(&t); 
            pos++;
        }
        else if (t.getType() == Lexer::TokenType::OPENING_BRACKET) {
            opStack.push(&t);
            pos++;
        }
        else if (t.getType() == Lexer::TokenType::COMMA) {
            while (!opStack.empty() && opStack.top()->getType() != Lexer::TokenType::OPENING_PAREN) {
                applyOperation();
                opStack.pop();
            }
            if (nodeStack.size() < 3) {
                throw std::runtime_error("Invalid function call arguments");
            }
            std::unique_ptr<AstNode> arg = std::move(nodeStack.top());
            nodeStack.pop();
            if (FunctionSentinel* fs = dynamic_cast<FunctionSentinel*>(nodeStack.top().get())) {  
                nodeStack.pop();
                if (FunctionCall* fc = dynamic_cast<FunctionCall*>(nodeStack.top().get())) {  
                    fc->args.push_back(std::move(arg));
                    pos++;
                    nodeStack.push(std::make_unique<FunctionSentinel>());
                    continue;
                }
            }
            
            throw std::runtime_error("Invalid function call arguments");
        }
        else if (t.getType() == Lexer::TokenType::CLOSING_PAREN) {
            while (!opStack.empty() && opStack.top()->getType() != Lexer::TokenType::OPENING_PAREN) {
                applyOperation();
                opStack.pop();
            }
            if (opStack.empty()) {
                throw std::runtime_error("Closing parenthesis without opening parenthesis.");
            }
            opStack.pop();
            // Must at least have 1 node
            if (nodeStack.empty()) {
                throw std::runtime_error("Invalid function call arguments");
            }

            std::unique_ptr<AstNode> temp = std::move(nodeStack.top());
            nodeStack.pop();
            // Check for function
            bool function = false;

            // if the function has 0 arguments, temp is the sentinel
            // 
            if (dynamic_cast<FunctionSentinel*>(temp.get())) {
                if (nodeStack.empty()) throw std::runtime_error("Sentinel without stack");
                if (FunctionCall* fc = dynamic_cast<FunctionCall*>(nodeStack.top().get())) {  
                    // sanity check
                    if (!fc->args.empty()) {
                        throw std::runtime_error("Function should not have no arguments");
                    }
                    function = true;
                } else {
                    throw std::logic_error("Function Sentinel found without function");
                }
            }

            // otherwise, temp is the last argument and the sentinel is below that
            if (!nodeStack.empty() && !function) {
                if (dynamic_cast<FunctionSentinel*>(nodeStack.top().get())) {  
                    nodeStack.pop();
                    if (FunctionCall* fc = dynamic_cast<FunctionCall*>(nodeStack.top().get())) {  
                        fc->args.push_back(std::move(temp));
                        function = true;
                    } else {
                        throw std::logic_error("Function Sentinel found without function");
                    }
                }
            }

            // Expression of type (5)
            // Must return temp to stack
            if (!function) {
                // If temp is a step
                // It must first be closed off in a FilterExpr
                // This is because some axis have a reverse order, such as ancestor
                // but (ancestor::something) must return to sorted order
                // This information must be kept in the ast
                if (dynamic_cast<Step*>(temp.get())) {
                    std::unique_ptr<FilterExpr> wrapper = std::make_unique<FilterExpr>();
                    wrapper->subject = std::move(temp);
                    nodeStack.push(std::move(wrapper));
                } else {
                    nodeStack.push(std::move(temp));
                }
            }
            pos++;
        }
        else if (t.getType() == Lexer::TokenType::CLOSING_BRACKET) {
            while (!opStack.empty() && opStack.top()->getType() != Lexer::TokenType::OPENING_BRACKET) {
                applyOperation();
                opStack.pop();
            }
            if (opStack.empty()) {
                throw std::runtime_error("Closing bracket without opening bracket.");
            }
            opStack.pop();

            if (nodeStack.size() < 2) {
                throw std::runtime_error("Cannot apply filter without subject");
            }
            std::unique_ptr<AstNode> predExpr = std::move(nodeStack.top()); 
            nodeStack.pop();

            if (Step* step = dynamic_cast<Step*>(nodeStack.top().get())) {
                step->predicates.push_back(std::move(predExpr));
            } else if (FilterExpr* filter = dynamic_cast<FilterExpr*>(nodeStack.top().get())) {
                filter->predicates.push_back(std::move(predExpr));
            } else {
                // We are filtering something
                // VM will deal with ensuring it is a proper nodeset
                std::unique_ptr<FilterExpr> newFilter = std::make_unique<FilterExpr>();
                newFilter->subject = std::move(nodeStack.top());
                nodeStack.pop();
                newFilter->predicates.push_back(std::move(predExpr));
                nodeStack.push(std::move(newFilter));
            }
            pos++;
        }
        else if (t.getType() == TokenType::OPERATOR
                || t.getType() == TokenType::SLASH
                || t.getType() == TokenType::DOUBLE_SLASH
                || t.getType() == TokenType::PIPE) {
            bool unary = this->shouldBeUnary();

            if (unary && t.getValue() == "-") {
                t.type = TokenType::UNARY_MINUS;
            }

            if (t.getType() == TokenType::SLASH || t.getType() == TokenType::DOUBLE_SLASH) {
                // If the current token is a slash, the previous token cannot be a slash.
                // We check pos > 0 because if pos == 0, it's a valid absolute path start.
                if (pos > 0) {
                    TokenType prevType = tokens[pos - 1].getType();
                    if (prevType == TokenType::SLASH || prevType == TokenType::DOUBLE_SLASH) {
                        throw std::runtime_error("Invalid path syntax: Adjacent slashes.");
                    }
                }
            }

            if (unary && (t.getType() == Lexer::TokenType::SLASH || t.getType() == Lexer::TokenType::DOUBLE_SLASH)) {
                nodeStack.push(std::make_unique<RootNode>());
            }

            while (!opStack.empty() && 
                       getPrecedence(opStack.top()) >= getPrecedence(&t)) {
                applyOperation();
                opStack.pop();
            }
            opStack.push(&t);
            pos++;
        } else {
            throw std::runtime_error("Unexpected token");
        }
    };

    while (!opStack.empty()) {
        applyOperation();
        opStack.pop();
    }

    if (nodeStack.size() != 1) {
        if (nodeStack.empty()) {
            throw std::runtime_error("Invalid expression");
        }
        throw std::runtime_error("Invalid expression: Unclosed function call or missing operator.");
    }

    return std::move(nodeStack.top());
}
};
