#include "xpath/compiler.h"
#include "xpath/calculate_mode.h"
#include "xpath/compare_mode.h"

namespace onyx::dynamic::xpath {
Compiler::Compiler(std::unique_ptr<Parser::AstNode> ast) : ast{std::move(ast)} {};

struct CompilerNode {
    Parser::AstNode* node;
    uint32_t resolveState = 0;
    uint32_t address1 = 0;
};

CALCULATE_MODE resolveCalculateMode(std::string_view op) {
    if (op == "+") return CALCULATE_MODE::ADD;
    else if (op == "-") return CALCULATE_MODE::SUBTRACT;
    else if (op == "div") return CALCULATE_MODE::DIVIDE;
    else if (op == "*") return CALCULATE_MODE::MULTIPLY;
    else if (op == "mov") return CALCULATE_MODE::MOD;
    
    throw std::runtime_error(std::string("Calculation operator could not be resolved: '") + std::string(op) + "'");
}

COMPARE_MODE resolveCompareMode(std::string_view op) {
    if (op == "!=") return COMPARE_MODE::NOT_EQUAL;
    else if (op == "=") return COMPARE_MODE::EQUAL;
    else if (op == "<") return COMPARE_MODE::LESS_THAN;
    else if (op == ">") return COMPARE_MODE::GREATER_THAN;
    else if (op == "<=") return COMPARE_MODE::LESS_THAN_OR_EQUAL;
    else if (op == ">=") return COMPARE_MODE::GREATER_THAN_OR_EQUAL;
    
    throw std::runtime_error(std::string("Comparison operator could not be resolved: '") + std::string(op) + "'");
}

void Compiler::initAxisCache() {
    if (!this->axisCache.empty()) return;

    this->axisCache.emplace("ancestor", AXIS::ANCESTOR);
    this->axisCache.emplace("ancestor-or-self", AXIS::ANCESTOR_OR_SELF);
    this->axisCache.emplace("attribute", AXIS::ATTRIBUTE);
    this->axisCache.emplace("child", AXIS::CHILD);
    this->axisCache.emplace("descendant", AXIS::DESCENDANT);
    this->axisCache.emplace("descendant-or-self", AXIS::DESCENDANT_OR_SELF);
    this->axisCache.emplace("following", AXIS::FOLLOWING);
    this->axisCache.emplace("following-sibling", AXIS::FOLLOWING_SIBLING);
    this->axisCache.emplace("namespace", AXIS::NAMESPACE);
    this->axisCache.emplace("parent", AXIS::PARENT);
    this->axisCache.emplace("preceding", AXIS::PRECEDING);
    this->axisCache.emplace("preceding-sibling", AXIS::PRECEDING_SIBLING);
    this->axisCache.emplace("self", AXIS::SELF);
}

AXIS Compiler::resolveAxis(std::string_view op) {
    initAxisCache();
    
    auto it = this->axisCache.find(op);
    if (it != this->axisCache.end()) {
        return it->second;
    }

    throw std::runtime_error(std::string("Axis could not be resolved: '") + std::string(op) + "'");
}

void Compiler::initFunctionCache() {
    if (!this->functionCache.empty()) return;

    this->functionCache.emplace("last", std::make_pair(FUNCTION_CODE::LAST_0, 0));
    this->functionCache.emplace("position", std::make_pair(FUNCTION_CODE::POSITION_0, 0));
    this->functionCache.emplace("count", std::make_pair(FUNCTION_CODE::COUNT_1, 1));
    this->functionCache.emplace("id", std::make_pair(FUNCTION_CODE::ID_1, 1));
    this->functionCache.emplace("local-name", std::make_pair(FUNCTION_CODE::LOCAL_NAME_1, 1));
    this->functionCache.emplace("namespace-uri", std::make_pair(FUNCTION_CODE::NAMESPACE_URI_1, 1));
    this->functionCache.emplace("name", std::make_pair(FUNCTION_CODE::NAME_1, 1));
    this->functionCache.emplace("string", std::make_pair(FUNCTION_CODE::STRING_1, 1));
    this->functionCache.emplace("concat", std::make_pair(FUNCTION_CODE::CONCAT_2, 2));
    this->functionCache.emplace("starts-with", std::make_pair(FUNCTION_CODE::STARTS_WITH_2, 2));
    this->functionCache.emplace("contains", std::make_pair(FUNCTION_CODE::CONTAINS_2, 2));
    this->functionCache.emplace("substring-before", std::make_pair(FUNCTION_CODE::SUBSTRING_BEFORE_2, 2));
    this->functionCache.emplace("substring-after", std::make_pair(FUNCTION_CODE::SUBSTRING_AFTER_2, 2));
    this->functionCache.emplace("substring", std::make_pair(FUNCTION_CODE::SUBSTRING_3, 3));
    this->functionCache.emplace("string-length", std::make_pair(FUNCTION_CODE::STRING_LENGTH_1, 1));
    this->functionCache.emplace("normalize-space", std::make_pair(FUNCTION_CODE::NORMALIZE_SPACE_1, 1));
    this->functionCache.emplace("translate", std::make_pair(FUNCTION_CODE::TRANSLATE_3, 3));
    this->functionCache.emplace("boolean", std::make_pair(FUNCTION_CODE::BOOLEAN_1, 1));
    this->functionCache.emplace("not", std::make_pair(FUNCTION_CODE::NOT_1, 1));
    this->functionCache.emplace("true", std::make_pair(FUNCTION_CODE::TRUE_0, 0));
    this->functionCache.emplace("false", std::make_pair(FUNCTION_CODE::FALSE_0, 0));
    this->functionCache.emplace("lang", std::make_pair(FUNCTION_CODE::LANG_1, 1));
    this->functionCache.emplace("number", std::make_pair(FUNCTION_CODE::NUMBER_1, 1));
    this->functionCache.emplace("sum", std::make_pair(FUNCTION_CODE::SUM_1, 1));
    this->functionCache.emplace("floor", std::make_pair(FUNCTION_CODE::FLOOR_1, 1));
    this->functionCache.emplace("ceiling", std::make_pair(FUNCTION_CODE::CEILING_1, 1));
    this->functionCache.emplace("round", std::make_pair(FUNCTION_CODE::ROUND_1, 1));
}

bool hasDefaultArgument(FUNCTION_CODE fc) {
    return fc == FUNCTION_CODE::LOCAL_NAME_1
        || fc == FUNCTION_CODE::NAMESPACE_URI_1
        || fc == FUNCTION_CODE::NAME_1
        || fc == FUNCTION_CODE::STRING_1
        || fc == FUNCTION_CODE::STRING_LENGTH_1
        || fc == FUNCTION_CODE::NORMALIZE_SPACE_1
        || fc == FUNCTION_CODE::NUMBER_1;
}

bool verifyArgumentCount(std::pair<FUNCTION_CODE, size_t> definition, size_t argCount) {
    FUNCTION_CODE fc = definition.first;
    if (hasDefaultArgument(fc) || fc == FUNCTION_CODE::SUBSTRING_3) {
        return (argCount == definition.second) || (argCount == definition.second - 1);
    }

    if (fc == FUNCTION_CODE::CONCAT_2) return argCount >= definition.second;

    return definition.second == argCount;
}

void pushInstruction(std::vector<Instruction>& instructions, Instruction&& instruction) {
    if (instructions.size() >= Instruction::maxOperand()) {
        throw std::runtime_error("Emitted more than the maximum allowed instruction count of " + Instruction::maxOperand());
    }

    instructions.emplace_back(std::move(instruction));
}

void pushData(std::vector<XPathObject>& data, std::string&& str) {
    if (data.size() >= Instruction::maxOperand()) {
        throw std::runtime_error("Emitted more than the maximum allowed data count of " + std::to_string(Instruction::maxOperand()));
    }

    data.emplace_back(std::move(str));
}

void pushData(std::vector<XPathObject>& data, double num) {
    if (data.size() >= Instruction::maxOperand()) {
        throw std::runtime_error("Emitted more than the maximum allowed data count of " + std::to_string(Instruction::maxOperand()));
    }

    data.emplace_back(num);
}

bool isReverseAxis(AXIS axis) {
    return axis == AXIS::ANCESTOR || axis == AXIS::ANCESTOR_OR_SELF || axis == AXIS::PRECEDING || axis == AXIS::PRECEDING_SIBLING;
}

std::unique_ptr<Program> Compiler::compile() {
    // this is currently very wasteful on data; it will record the same string/number twice on different indices
    // this needs to be optimized in the future - TODO
    // currently, it also doesn't protect the data and instruction boundaries
    // data can have at most 24 entries; instructions may have at most 24 entries as well
    // this is due to the restrictions on instruction size - TODO
    std::vector<XPathObject> data;
    std::vector<Instruction> instructions;

    std::stack<CompilerNode> stack;

    stack.emplace(this->ast.get());
    
    Parser::AstNode* current = nullptr;

    while (!stack.empty()) {
        CompilerNode& currentCompileNode = stack.top();
        current = currentCompileNode.node;
        uint32_t resolveState = currentCompileNode.resolveState;

        if (!current) {
            throw std::runtime_error("Found empty node");
        }
        
        switch (current->getType()) {
            case Parser::AstNode::Literal: {
                Parser::Literal* lit = static_cast<Parser::Literal*>(current);
                pushData(data, std::move(lit->value));
                pushInstruction(instructions, Instruction(OPCODE::LOAD_CONSTANT, data.size() - 1));
                stack.pop();
                break;
            };
            case Parser::AstNode::Number: {
                Parser::Number* num = static_cast<Parser::Number*>(current);
                pushData(data, XPathObject(num->num).asNumber());
                pushInstruction(instructions, Instruction(OPCODE::LOAD_CONSTANT, data.size() - 1));
                stack.pop();
                break;
            };
            case Parser::AstNode::RootNode: {
                pushInstruction(instructions, Instruction(OPCODE::LOAD_ROOT));
                stack.pop();
                break;
            };
            case Parser::AstNode::VarRef: {
                Parser::VarRef* var = static_cast<Parser::VarRef*>(current);
                pushData(data, std::move(var->name));
                pushInstruction(instructions, Instruction(OPCODE::LOAD_VARIABLE, data.size() - 1));
                stack.pop();
                break;
            };
            case Parser::AstNode::FunctionSentinel: {
                throw std::logic_error("Function Sentinel should not appear in finished AST");
            };
            case Parser::AstNode::BinaryOp: {
                Parser::BinaryOp* binaryOp = static_cast<Parser::BinaryOp*>(current);
                if (resolveState == 0 && (!binaryOp->left || !binaryOp->right)) {
                    throw std::runtime_error("Binary operation missing side.");
                }

                if (binaryOp->op == "+"
                    || binaryOp->op == "-"
                    || binaryOp->op == "div"
                    || binaryOp->op == "mod"
                    || binaryOp->op == "*") {

                    if (resolveState == 0) {
                        stack.emplace(binaryOp->right.get());
                        stack.emplace(binaryOp->left.get());
                        currentCompileNode.resolveState++;
                    } else {
                        pushInstruction(instructions, Instruction(OPCODE::CALCULATE, resolveCalculateMode(binaryOp->op)));
                        stack.pop();
                    }
                } else if (binaryOp->op == "/") {
                    // For the '/' operator, we first need to resolve the left side,
                    // apply a union loop, and then resolve the right side inside the loop
                    // Only after the whole right side is resolved, we must return to establish the
                    // instruction boundaries
                    if (resolveState == 0) {
                        stack.emplace(binaryOp->left.get());
                        currentCompileNode.resolveState++;
                    } else if (resolveState == 1) {
                        // Placeholder jump address
                        pushInstruction(instructions, Instruction(OPCODE::LOOP_ENTER, 0));
                        // We remember the instruction address to fill in the jump address later
                        currentCompileNode.address1 = instructions.size() - 1;
                        // for root nodes, this produces 
                        // LOAD_ROOT
                        // LOOP_ENTER
                        //  ...whatever is done...
                        //  LOOP_UNION
                        // LOOP_NEXT
                        // which is a loop over all in all one node
                        // this can be optimized in the future - TODO
                        // the right side must be resolved
                        stack.emplace(binaryOp->right.get());
                        currentCompileNode.resolveState++;
                    } else {
                        // We have resolved the left and right sides
                        // The loop can be finished
                        pushInstruction(instructions, Instruction(OPCODE::LOOP_UNION));
                        pushInstruction(instructions, Instruction(OPCODE::LOOP_NEXT));
                        // The address of the LOOP_ENTER jump must be filled 
                        // It is valid to point it to loop next
                        // It will jump to the next instruction
                        // which must exist because of HALT being mandatory.
                        uint32_t address = instructions.size();
                        instructions[currentCompileNode.address1] = Instruction(OPCODE::LOOP_ENTER, address);
                        stack.pop();
                    }
                } else if (binaryOp->op == "|") {

                    if (resolveState == 0) {
                        stack.emplace(binaryOp->right.get());
                        stack.emplace(binaryOp->left.get());
                        currentCompileNode.resolveState++;
                    } else {
                        pushInstruction(instructions, Instruction(OPCODE::UNION));
                        stack.pop();
                    }
                } else if (binaryOp->op == "!="
                            || binaryOp->op == "="
                            || binaryOp->op == "<"
                            || binaryOp->op == ">"
                            || binaryOp->op == "<="
                            || binaryOp->op == ">=") {

                    if (resolveState == 0) {
                        stack.emplace(binaryOp->right.get());
                        stack.emplace(binaryOp->left.get());
                        currentCompileNode.resolveState++;
                    } else {
                        pushInstruction(instructions, Instruction(OPCODE::COMPARE, resolveCompareMode(binaryOp->op)));
                        stack.pop();
                    }
                } else if (binaryOp->op == "and"
                            || binaryOp->op == "or") {
                    // 'and' and 'or' by the spec must exit early
                    // This means that the structure for this operation must have early exit jumps.
                    // Essentially, for 'and', this means a structure like this (in pseudocode):
                    // cond1:
                    // do_something()
                    // jump_if_false(on_false)
                    // cond2:
                    // do_something()
                    // jump_if_true(on_true) // we are sure cond1 was true
                    // on_false:
                    // push_false() // the jumps consume the value on the stack
                    // jump(exit)
                    // on_true()
                    // push_true()
                    // exit: // exit represents the instruction after the comparison
                    
                    // For or, it is a similar structure. but inverted:
                    // cond1:
                    // do_something()
                    // jump_if_true(on_true)
                    // cond2:
                    // do_something()
                    // jump_if_true(on_true) // we are sure cond1 was false
                    // on_false:
                    // push_false() // the jumps consume the value on the stack
                    // jump(exit)
                    // on_true()
                    // push_true()
                    // exit: // exit represents the instruction after the comparison
                    // Along the way, we need to record the addresses of the two jumps to correct them
                    bool isAnd = binaryOp->op == "and";

                    if (resolveState == 0) {
                        stack.emplace(binaryOp->left.get());
                        currentCompileNode.resolveState++;
                    } else if (resolveState == 1) {
                        // the left side has been visited
                        // The jump must be placed, then the right side must be visited
                        pushInstruction(instructions, 
                            Instruction(isAnd ? OPCODE::JUMP_F : OPCODE::JUMP_T, 0)
                        );
                        // The address must be recorded for later
                        currentCompileNode.address1 = instructions.size() - 1;
                        stack.emplace(binaryOp->right.get());
                        currentCompileNode.resolveState++;
                    } else {
                        // the right side has been visited
                        // we can calculate the jump location here, since we know the following instructions
                        uint32_t falseAddress = instructions.size() + 1; // instructions.size() is the address of the jump itself
                        // the false address is the next instruction
                        uint32_t trueAddress = falseAddress + 2; // the instruction after the false address is the jump to the exit
                        // right after it is the true address
                        pushInstruction(instructions, 
                            Instruction(OPCODE::JUMP_T, trueAddress)
                        );
                        
                        // produce the jump locations
                        // produce the false jump
                        // the instruction places a false value onto the stack to recover the one consumed in the jump
                        pushInstruction(instructions, Instruction(OPCODE::CALL, FUNCTION_CODE::FALSE_0));
                        // jump to exit
                        uint32_t exitAddress = trueAddress + 1; // Just after the trueAddress is the next instruction
                        // It must exist eventually because HALT is mandatory

                        pushInstruction(instructions, Instruction(OPCODE::JUMP, exitAddress));
                        // produce the true jump
                        // the instruction places a true value onto the stack to recover the one consumed in the jump
                        pushInstruction(instructions, Instruction(OPCODE::CALL, FUNCTION_CODE::TRUE_0));
                        // the exit location is the one after this last instruction
                        // this means, here a jump is not needed, we can just fall through
                        // Now, we go back to give the first jump the proper address
                        OPCODE opcode = instructions[currentCompileNode.address1].getOpcode();
                        instructions[currentCompileNode.address1] = Instruction(opcode, opcode == OPCODE::JUMP_F ? falseAddress : trueAddress);
                        stack.pop();
                    }
                } else {
                    throw std::runtime_error("Unexpected operator");
                }

                break;
            } 
            case Parser::AstNode::Step: {
                Parser::Step* step = static_cast<Parser::Step*>(current);

                // predicates must be handled one by one in separate loops
                // each one of them is a resolveState of the step

                // Crucially, ancestor::node()[1] returns the the first ancestor in reverse order,
                // not in document order. This means it returns the parent, not the root.
                // However, expressions force the sorting of the result into document order
                // so, (ancestor::node())[1] will actually return the root.
                // This means, after every step, a SORT instruction must be emitted
                // Since only 4 axis generate the reverse order, this can be optimized.
                // 'preceding', 'ancestor', 'ancestor-or-self' and 'preceding-sibling'.
                if (currentCompileNode.resolveState == 0) {
                    pushInstruction(instructions, Instruction(OPCODE::LOAD_CONTEXT_NODE));
                    pushData(data, std::move(step->test));   
                    pushInstruction(instructions, Instruction(OPCODE::LOAD_CONSTANT, data.size() - 1));
                    AXIS axis = resolveAxis(step->axis);
                    pushInstruction(instructions, Instruction(OPCODE::SELECT, axis));

                    if (step->predicates.empty()) {
                        if (isReverseAxis(axis)) {
                            pushInstruction(instructions, Instruction(OPCODE::SORT));  
                        }
                        stack.pop();
                        // exit early to avoid adding of first predicate
                        break;
                    }

                    pushInstruction(instructions, Instruction(OPCODE::LOOP_ENTER, 0));
                    // we remember the instruction address to fill in the jump address later
                    currentCompileNode.address1 = instructions.size() - 1;
                    stack.emplace(step->predicates[0].get());
                    currentCompileNode.resolveState++;
                } else {
                    // we must close the previous predicate loop
                    // if there is a next predicate, we must begin the next loop
                    pushInstruction(instructions, Instruction(OPCODE::CONTEXT_NODE_TEST));
                    pushInstruction(instructions, Instruction(OPCODE::LOOP_NEXT));

                    // The address of the LOOP_ENTER jump must be filled 
                    // It is valid to point it to loop next
                    // It will jump to the next instruction
                    // which must exist because of HALT being mandatory.
                    uint32_t address = instructions.size();
                    instructions[currentCompileNode.address1] = Instruction(OPCODE::LOOP_ENTER, address);

                    // we have closed the predicate
                    // if there are no more predicates, we exit early
                    if (currentCompileNode.resolveState >= step->predicates.size()) {
                        AXIS axis = resolveAxis(step->axis);
                        if (isReverseAxis(axis)) {
                            pushInstruction(instructions, Instruction(OPCODE::SORT));  
                        }
                        stack.pop();
                        break;
                    }

                    pushInstruction(instructions, Instruction(OPCODE::LOOP_ENTER, 0));
                    // we remember the instruction address to fill in the jump address later
                    currentCompileNode.address1 = instructions.size() - 1;
                    stack.emplace(step->predicates[currentCompileNode.resolveState].get());
                    currentCompileNode.resolveState++;
                }

                break;
            }
            case Parser::AstNode::FilterExpr: {
                Parser::FilterExpr* expression = static_cast<Parser::FilterExpr*>(current);

                if (!expression->subject) {
                    throw std::runtime_error("Empty expression");
                }

                // logic is very similar to Step
                // The main difference is that we must resolve the expression

                // predicates must be handled one by one in separate loops
                // each one of them is a resolveState of the expression
                if (currentCompileNode.resolveState == 0) {
                    // we must first resolve the subject
                    stack.emplace(expression->subject.get());
                    currentCompileNode.resolveState++;
                    // early exit to avoid falling into predicate logic
                    break;
                } else if (currentCompileNode.resolveState == 1) {
                    // the subject is now visited
                    // If there are no predicates, we are finished
                    if (expression->predicates.empty()) {
                        stack.pop();
                        // early exit to avoid falling into predicate logic
                        break;
                    }

                    pushInstruction(instructions, Instruction(OPCODE::LOOP_ENTER, 0));
                    // we remember the instruction address to fill in the jump address later
                    currentCompileNode.address1 = instructions.size() - 1;
                    stack.emplace(expression->predicates[currentCompileNode.resolveState - 1].get());
                    currentCompileNode.resolveState++;

                } else {
                    // we must close the previous predicate loop
                    // if there is a next predicate, we must begin the next loop
                    pushInstruction(instructions, Instruction(OPCODE::CONTEXT_NODE_TEST));
                    pushInstruction(instructions, Instruction(OPCODE::LOOP_NEXT));

                    // The address of the LOOP_ENTER jump must be filled 
                    // It is valid to point it to loop next
                    // It will jump to the next instruction
                    // which must exist because of HALT being mandatory.
                    uint32_t address = instructions.size();
                    instructions[currentCompileNode.address1] = Instruction(OPCODE::LOOP_ENTER, address);

                    // we have closed the predicate
                    // if there are no more predicates, we exit early
                    if (currentCompileNode.resolveState - 1 >= expression->predicates.size()) {
                        stack.pop();
                        break;
                    }

                    pushInstruction(instructions, Instruction(OPCODE::LOOP_ENTER, 0));
                    // we remember the instruction address to fill in the jump address later
                    currentCompileNode.address1 = instructions.size() - 1;
                    stack.emplace(expression->predicates[resolveState - 1].get());
                    currentCompileNode.resolveState++;
                }

                break;
            }
            case Parser::AstNode::FunctionCall: {
                Parser::FunctionCall* function = static_cast<Parser::FunctionCall*>(current);

                // We must first resolve the function
                // some functions have default arguments we must add instructions to create
                // these are precisely:
                // 'string local-name(node-set?)'
                // 'string namespace-uri(node-set?)'
                // 'string name(node-set?)'
                // 'string string(object?)'
                // 'number string-length(string?)'
                // 'string normalize-space(string?)'
                // 'number number(object?)'
                // However, they all use a nodeset with the context node as its only member as the default argument
                // There is also the 'string concat(string, string, string*)' function, which takes at least two arguments,
                // but is variadic. The key observation is that it is strictly equivalent to a nested 'string concat(string, string)'
                // call. For example, concat("a", "b", "c") == concat("a", concat("b", "c")).
                // This nesting can be easily achieved in the bytecode by loading all the arguments to stack
                // Then simply calling 'string concat(string, string)' the appropriate number of times
                // The number is n-1 for n arguments.
                // This is easily observed, as the first call has 2 arguments, 
                // and every call additional argument after the second results in a call to 
                // 'string concat(string, string)' that has the new argument for argument 1 and then a nested call for argument 2
                // There is the function 'string substring(string, number, number?)' which has an optional argument that changes its behaviour
                // We must handle that as well.
                // Also, invalid functions may appear, as names are not validated until compilation.

                // We begin by validating the function name and number of arguments,
                // but to call the function, all arguments must be resolved.
                // So validation is done first, but is then used only at the end
                if (currentCompileNode.resolveState == 0) {
                    this->initFunctionCache();

                    auto it = this->functionCache.find(function->name);
                    
                    if (it == this->functionCache.end()) {
                        throw std::runtime_error("Unknown function name");
                    }

                    if (!verifyArgumentCount(it->second, function->args.size())) {
                        throw std::runtime_error(std::string("Supplied incorrect number of arguments to function '") + function->name + "'");
                    }

                    // fall through
                }

                // This fails on 0 arguments
                if (currentCompileNode.resolveState < function->args.size()) {
                    stack.emplace(function->args[currentCompileNode.resolveState].get());
                    currentCompileNode.resolveState++;

                    // early exit
                    break;
                }

                // All the arguments have been processed when this is reached

                auto it = this->functionCache.find(function->name);
                    
                if (it == this->functionCache.end()) {
                    // should be impossible to reach this
                    throw std::logic_error("Unknown function name after processing arguments");
                }

                FUNCTION_CODE fc = it->second.first;

                // Handle concat
                if (fc == CONCAT_2) {
                    for (int i = 0; i < function->args.size() - 1; i++) {
                        pushInstruction(instructions, Instruction(OPCODE::CALL, CONCAT_2));
                    }
                } else if (fc == SUBSTRING_3 && function->args.size() == 2) { 
                    // Handle 'substring' with two arguments
                    pushInstruction(instructions, Instruction(OPCODE::CALL, SUBSTRING_2));
                } else if (hasDefaultArgument(fc) && function->args.size() == 0) {                    
                    // for the default argument, an additional instruction must be injected to load the context node
                    pushInstruction(instructions, Instruction(OPCODE::LOAD_CONTEXT_NODE));
                    pushInstruction(instructions, Instruction(OPCODE::CALL, fc));
                } else {
                    // Call the function
                    pushInstruction(instructions, Instruction(OPCODE::CALL, fc));
                }

                // We have finished processing the function
                stack.pop();

                break;
            }
        }
    }

    pushInstruction(instructions, Instruction(OPCODE::HALT));

    return std::make_unique<Program>(data, instructions);
}
};