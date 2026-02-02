#include "xpath/virtual_machine.h"

#include <stack>

#include "nodes/attribute_view_node.h"
#include "nodes/comment_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/root_view_node.h"
#include "xpath/axis.h"
#include "xpath/calculate_mode.h"
#include "xpath/compare_mode.h"
#include "xpath/function_code.h"
#include "xpath/functions.h"

namespace onyx::dynamic::xpath {

VirtualMachine::VirtualMachine(std::unique_ptr<Program> program)
    : program{std::move(program)} {}

size_t jump(size_t address, const size_t instructionsSize) {
    if (address >= instructionsSize) {
        throw std::runtime_error("Instruction address out of bounds");
    }

    return address;
}

VirtualMachine::DocumentRoot::DocumentRoot(ExecutionContext& ec)
    : documentRoot{nullptr}, xpathRoot{nullptr}, ec{ec} {}

void VirtualMachine::DocumentRoot::findRoot(Node* any) {
    if (this->documentRoot) return;

    Node* current = any;

    while (current->getParentNode()) {
        current = current->getParentNode();
    }

    this->documentRoot = current;

    std::unique_ptr<RootViewNode> rootView =
        std::make_unique<RootViewNode>(this->documentRoot);

    this->xpathRoot = rootView.get();

    ec.temporaryNodes.push_back(std::move(rootView));
}

Node* VirtualMachine::DocumentRoot::getDocumentRoot() {
    return this->documentRoot;
}

Node* VirtualMachine::DocumentRoot::getXPathRoot() { return this->xpathRoot; }

void VirtualMachine::DocumentOrder::buildIndex(Node* root) {
    if (!documentOrderList.empty()) return;

    size_t idx = 0;
    root->iterativeProcessor([&idx, this](Node* obj) -> void {
        this->documentOrderList.push_back(obj);
        this->documentOrderMap[obj] = idx++;
    });
}

bool VirtualMachine::DocumentOrder::compare(Node* a, Node* b) {
    return documentOrderMap[a] < documentOrderMap[b];
}

VirtualMachine::ExecutionContext::ExecutionContext()
    : dataStack{},
      contextStack{},
      instructionPointer{0},
      root{*this},
      order{},
      temporaryNodes{} {}

void VirtualMachine::DocumentOrder::normalizeDocumentOrderSet(
    std::vector<Node*>& nodeset) {
    std::sort(nodeset.begin(), nodeset.end(),
              [this](Node* a, Node* b) { return this->compare(a, b); });

    nodeset.erase(std::unique(nodeset.begin(), nodeset.end()), nodeset.end());
}

#define EMPTY_STACK_GUARD(instruction)                            \
    if (ec.dataStack.empty()) {                                   \
        throw std::runtime_error("Cannot execute " #instruction   \
                                 " because data stack is empty"); \
    }

VirtualMachine::ExecutionResult VirtualMachine::executeOn(Node* current, std::function<XPathObject(std::string_view)> variableProvider) {
    static VirtualMachine::FunctionRegistry functionRegistry =
        registerFunctions();

    const std::vector<XPathObject>& data = this->program->getData();
    const std::vector<Instruction>& instructions =
        this->program->getInstructions();

    ExecutionContext ec{};

    ec.contextStack.emplace(FrameContext({current}, 0, 0, {}));

    while (true) {
        if (ec.instructionPointer >= instructions.size()) {
            throw std::runtime_error("Program ended without HALT");
        }

        const Instruction& instruction = instructions[ec.instructionPointer];

        if (ec.contextStack.empty()) {
            throw std::runtime_error("No available context");
        }
        FrameContext& context = ec.contextStack.top();

        OPCODE opcode = instruction.getOpcode();

        switch (opcode) {
            case OPCODE::HALT: {
                EMPTY_STACK_GUARD(HALT);

                return std::move(
                    ExecutionResult(std::move(ec.temporaryNodes),
                                    std::move(ec.dataStack.top())));
            };
            case OPCODE::LOAD_CONSTANT: {
                uint32_t address = instruction.getOperandImm();

                if (address >= data.size()) {
                    throw std::runtime_error("Data address out of bounds in LOAD_CONSTANT");
                }

                ec.dataStack.push(data[address]);

                break;
            };
            case OPCODE::LOAD_CONTEXT_NODE: {
                ec.dataStack.push(XPathObject(std::vector<Node*>{
                    context.contextSet[context.currentIndex]}));

                break;
            };
            case OPCODE::LOAD_POSITION: {
                ec.dataStack.push(
                    XPathObject(static_cast<double>(context.currentIndex + 1)));

                break;
            };
            case OPCODE::LOAD_LENGTH: {
                ec.dataStack.push(
                    XPathObject(static_cast<double>(context.getContextSize())));

                break;
            };
            case OPCODE::LOAD_ROOT: {
                ec.root.findRoot(context.contextSet[context.currentIndex]);

                ec.dataStack.push(
                    XPathObject(std::vector<Node*>{ec.root.getXPathRoot()}));

                break;
            };
            case OPCODE::LOAD_VARIABLE: {
                uint32_t address = instruction.getOperandImm();

                if (address >= data.size()) {
                    throw std::runtime_error("Data address out of bounds in LOAD_VARIABLE");
                }

                if (!data[address].isString()) {
                    throw std::runtime_error("Variable name must be string.");
                }

                ec.dataStack.push(variableProvider(data[address].asString()));

                break;
            };
            case OPCODE::SELECT: {
                executeSelect(instruction, ec);
                break;
            };
            case OPCODE::UNION: {
                executeUnion(instruction, ec);

                break;
            };
            case OPCODE::SORT: {
                executeSort(instruction, ec);

                break;
            };
            case OPCODE::JUMP: {
                ec.instructionPointer =
                    jump(instruction.getOperandImm(), instructions.size());
                continue;
            };
            case OPCODE::JUMP_F: {
                EMPTY_STACK_GUARD(JUMP_F);

                if (!ec.dataStack.top().asBool()) {
                    ec.instructionPointer =
                        jump(instruction.getOperandImm(), instructions.size());
                    ec.dataStack.pop();
                    continue;
                }

                ec.dataStack.pop();

                break;
            };
            case OPCODE::JUMP_T: {
                EMPTY_STACK_GUARD(JUMP_T);

                if (ec.dataStack.top().asBool()) {
                    ec.instructionPointer =
                        jump(instruction.getOperandImm(), instructions.size());
                    ec.dataStack.pop();
                    continue;
                }

                ec.dataStack.pop();

                break;
            };
            case OPCODE::LOOP_ENTER: {
                EMPTY_STACK_GUARD(LOOP_ENTER);

                if (!ec.dataStack.top().isNodeset()) {
                    throw std::runtime_error(
                        "Cannot enter loop because data stack top is not a "
                        "nodeset");
                }

                const std::vector<Node*>& nodeset =
                    ec.dataStack.top().asNodeset();

                if (nodeset.empty()) {
                    // The empty vector can just be left
                    ec.instructionPointer =
                        jump(instruction.getOperandImm(), instructions.size());
                    continue;
                }

                ec.contextStack.emplace(
                    FrameContext(nodeset, 0, ec.instructionPointer, {}));

                ec.dataStack.pop();

                break;
            };
            case OPCODE::CONTEXT_NODE_TEST: {
                EMPTY_STACK_GUARD(CONTEXT_NODE_TEST);

                // Encode special predicate behaviour (ancestor::div[5] means ancestor::div[position() == 5])
                // This cannot be resolved any earlier, because until evaluation is is hard to determine
                // the type of the result in the predicate
                if (ec.dataStack.top().isNumber()) {
                    if (context.currentIndex + 1 == ec.dataStack.top().asNumber()) {
                        context.result.push_back(
                            context.contextSet[context.currentIndex]);
                    }
                } else if (ec.dataStack.top().asBool()) {
                    context.result.push_back(
                        context.contextSet[context.currentIndex]);
                }

                ec.dataStack.pop();

                break;
            };
            case OPCODE::LOOP_NEXT: {
                if (context.currentIndex + 1 < context.contextSet.size()) {
                    context.currentIndex++;

                    // Due to the initial context, if the first instruction of a
                    // program is LOOP_NEXT, this can actually fail, so it needs
                    // to be validated
                    ec.instructionPointer =
                        jump(context.startAddress + 1, instructions.size());
                    continue;
                }

                if (context.hasUsedLoopUnion) {
                    ec.root.findRoot(context.contextSet[context.currentIndex]);
                    ec.order.buildIndex(ec.root.getDocumentRoot());

                    ec.order.normalizeDocumentOrderSet(context.contextSet);
                }

                ec.dataStack.push(XPathObject(context.result));

                ec.contextStack.pop();

                break;
            };
            case OPCODE::LOOP_UNION: {
                EMPTY_STACK_GUARD(LOOP_UNION);

                if (!ec.dataStack.top().isNodeset()) {
                    throw std::runtime_error(
                        "Cannot union nodeset because data stack top is not a "
                        "nodeset");
                }

                const std::vector<Node*>& nodeset =
                    ec.dataStack.top().asNodeset();

                context.result.insert(context.result.end(), nodeset.begin(),
                                      nodeset.end());

                context.hasUsedLoopUnion = true;

                ec.dataStack.pop();

                break;
            }
            case OPCODE::CALCULATE: {
                executeCalculate(instruction, ec);

                break;
            };
            case OPCODE::COMPARE: {
                executeCompare(instruction, ec);

                break;
            };
            case OPCODE::CALL: {
                FUNCTION_CODE fc =
                    static_cast<FUNCTION_CODE>(instruction.getOperandImm());
                auto it = functionRegistry.find(fc);

                if (it == functionRegistry.end()) {
                    throw std::runtime_error("Unknown function");
                }

                ec.dataStack.push(it->second(context, ec.dataStack));

                break;
            };
            default: {
                throw std::runtime_error("Invalid instruction");
            };
        };

        ec.instructionPointer++;
    }
}

void VirtualMachine::executeSelect(const Instruction& instruction,
                                   ExecutionContext& ec) {
    if (ec.dataStack.size() < 2) {
        throw std::runtime_error(
            "Cannot execute SELECT because 2 arguments are required");
    }

    if (!ec.dataStack.top().isString()) {
        throw std::runtime_error("SELECT instruction needs a Node Test string");
    }
    std::string nodeTest = ec.dataStack.top().asString();
    ec.dataStack.pop();

    if (!ec.dataStack.top().isNodeset()) {
        throw std::runtime_error("SELECT instruction needs a nodeset");
    }
    const std::vector<Node*>& nodes = ec.dataStack.top().asNodeset();

    if (nodes.size() != 1) {
        throw std::runtime_error("SELECT instruction needs a single node");
    }

    Node* contextNode = nodes[0];

    ec.dataStack.pop();

    AXIS axis = static_cast<AXIS>(instruction.getOperandImm());

    std::vector<Node*> nodeset;

    switch (axis) {
        case AXIS::CHILD: {
            collectChildren(contextNode, axis, nodeTest, nodeset);
            break;
        }
        case AXIS::DESCENDANT: {
            collectDescendants(contextNode, axis, nodeTest, nodeset);
            break;
        }
        case AXIS::PARENT: {
            collectParent(contextNode, axis, nodeTest, nodeset, ec.root);
            break;
        }
        case AXIS::ANCESTOR: {
            collectAncestor(contextNode, axis, nodeTest, nodeset, ec.root);
            break;
        }
        case AXIS::FOLLOWING_SIBLING: {
            collectFollowingSiblings(contextNode, axis, nodeTest, nodeset);
            break;
        }
        case AXIS::PRECEDING_SIBLING: {
            collectPrecedingSiblings(contextNode, axis, nodeTest, nodeset);
            break;
        }
        case AXIS::SELF: {
            if (nodeMatchesTest(contextNode, axis, nodeTest)) {
                nodeset.push_back(contextNode);
            }
            break;
        }
        case AXIS::ATTRIBUTE: {
            if (contextNode->getXPathType() == Node::XPathType::ELEMENT) {
                for (size_t i = 0; i < contextNode->getAttributes().size(); i++) {
                    AttributeViewNode tempAttr(contextNode, i);

                    if (nodeMatchesTest(&tempAttr, axis, nodeTest)) {
                        std::unique_ptr<AttributeViewNode> attrNode =
                            std::make_unique<AttributeViewNode>(contextNode, i);
                        Node* temp = attrNode.get();
                        ec.temporaryNodes.push_back(std::move(attrNode));
                        nodeset.push_back(temp);
                    }
                }
            }
            break;
        }
        case AXIS::DESCENDANT_OR_SELF: {
            if (nodeMatchesTest(contextNode, axis, nodeTest)) {
                nodeset.push_back(contextNode);
            }
            collectDescendants(contextNode, axis, nodeTest, nodeset);
            break;
        }
        case AXIS::ANCESTOR_OR_SELF: {
            if (nodeMatchesTest(contextNode, axis, nodeTest)) {
                nodeset.push_back(contextNode);
            }
            collectAncestor(contextNode, axis, nodeTest, nodeset, ec.root);
            break;
        }
        case AXIS::FOLLOWING: {
            collectFollowing(contextNode, axis, nodeTest, nodeset, ec);
            break;
        };
        case AXIS::PRECEDING: {
            collectPreceding(contextNode, axis, nodeTest, nodeset, ec);
            break;
        };
        case AXIS::NAMESPACE: {
            throw std::runtime_error("Unsupported axis used.");
            break;
        };
        default: {
            throw std::runtime_error("Unknown axis");
        }
    }

    ec.dataStack.push(XPathObject(std::move(nodeset)));
}

void VirtualMachine::executeUnion(const Instruction& instruction,
                                  ExecutionContext& ec) {
    FrameContext& context = ec.contextStack.top();

    if (ec.dataStack.size() < 2) {
        throw std::runtime_error(
            "Cannot execute UNION because stack has less than 2 elements");
    }

    if (!ec.dataStack.top().isNodeset()) {
        throw std::runtime_error(
            "Cannot execute UNION because stack has non-nodeset top");
    }

    std::vector<Node*> nodeset1 = ec.dataStack.top().asNodeset();
    ec.dataStack.pop();

    if (!ec.dataStack.top().isNodeset()) {
        throw std::runtime_error(
            "Cannot execute UNION because stack has non-nodeset top");
    }

    const std::vector<Node*>& nodeset2 = ec.dataStack.top().asNodeset();

    nodeset1.reserve(nodeset1.size() + nodeset2.size());
    nodeset1.insert(nodeset1.end(), nodeset2.begin(), nodeset2.end());

    ec.dataStack.pop();

    ec.root.findRoot(context.contextSet[context.currentIndex]);
    ec.order.buildIndex(ec.root.getDocumentRoot());

    ec.order.normalizeDocumentOrderSet(nodeset1);

    ec.dataStack.push(XPathObject(nodeset1));
}

void VirtualMachine::executeSort(const Instruction& instruction,
                                 ExecutionContext& ec) {
    EMPTY_STACK_GUARD(SORT);

    FrameContext& context = ec.contextStack.top();

    if (!ec.dataStack.top().isNodeset()) {
        return;
    }

    ec.root.findRoot(context.contextSet[context.currentIndex]);
    ec.order.buildIndex(ec.root.getDocumentRoot());

    std::vector<Node*> nodes = std::move(ec.dataStack.top().asNodeset());

    ec.dataStack.pop();

    std::sort(nodes.begin(), nodes.end(),
              [&](Node* a, Node* b) { return ec.order.compare(a, b); });

    ec.dataStack.push(XPathObject(std::move(nodes)));
}

void VirtualMachine::executeCalculate(const Instruction& instruction,
                                      ExecutionContext& ec) {
    if (ec.dataStack.size() < 2) {
        throw std::runtime_error(
            "Cannot execute math operation on less than 2 stack values");
    }

    double val2 = ec.dataStack.top().asNumber();
    ec.dataStack.pop();

    double val1 = ec.dataStack.top().asNumber();
    ec.dataStack.pop();

    double result = std::numeric_limits<double>::quiet_NaN();

    CALCULATE_MODE mode =
        static_cast<CALCULATE_MODE>(instruction.getOperandImm());

    switch (mode) {
        case CALCULATE_MODE::ADD: {
            result = val1 + val2;
            break;
        }
        case CALCULATE_MODE::SUBTRACT: {
            result = val1 - val2;
            break;
        }
        case CALCULATE_MODE::MULTIPLY: {
            result = val1 * val2;
            break;
        }
        case CALCULATE_MODE::DIVIDE: {
            result = val1 / val2;
            break;
        }
        case CALCULATE_MODE::MOD: {
            // https://www.w3.org/TR/1999/REC-xpath-19991116/#numbers
            // https://www.mindprod.com/jgloss/modulus.html#FLOATINGPOINT
            result = std::fmod(val1, val2);
            break;
        }
        default: {
            throw std::runtime_error("Unknown calculate mode");
        }
    }

    ec.dataStack.push(XPathObject(result));
}

void VirtualMachine::executeCompare(const Instruction& instruction,
                                    ExecutionContext& ec) {
    if (ec.dataStack.size() < 2) {
        throw std::runtime_error(
            "Cannot execute compare operation on less than 2 stack values");
    }

    XPathObject val2 = ec.dataStack.top();
    ec.dataStack.pop();

    XPathObject val1 = ec.dataStack.top();
    ec.dataStack.pop();

    bool result = false;

    COMPARE_MODE mode = static_cast<COMPARE_MODE>(instruction.getOperandImm());

    switch (mode) {
        case COMPARE_MODE::EQUAL: {
            result = val1 == val2;
            break;
        }
        case COMPARE_MODE::NOT_EQUAL: {
            result = val1 != val2;
            break;
        }
        case COMPARE_MODE::LESS_THAN: {
            result = val1 < val2;
            break;
        }
        case COMPARE_MODE::GREATER_THAN: {
            result = val1 > val2;
            break;
        }
        case COMPARE_MODE::LESS_THAN_OR_EQUAL: {
            result = val1 <= val2;
            break;
        }
        case COMPARE_MODE::GREATER_THAN_OR_EQUAL: {
            result = val1 >= val2;
            break;
        }
        default: {
            throw std::runtime_error("Unknown compare mode");
        }
    }

    ec.dataStack.push(XPathObject(result));
}

#undef EMPTY_STACK_GUARD
}  // namespace onyx::dynamic::xpath
