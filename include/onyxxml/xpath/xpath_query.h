#pragma once

#include "virtual_machine.h"

namespace onyx::dynamic::xpath {
/**
 * @brief A facade for the XPath compilation and execution pipeline.
 * Compiles an XPath query and allows its repeated execution.
 * 
 */
class XPathQuery {
private:
    /**
     * @brief The internal virtual machine.
     * 
     */
    VirtualMachine vm;
public:
    /**
     * @brief Holds the execution result objects, as well as the and the storage for the
     * temporary nodes, such as AttributeViewName and RootViewNode.
     * 
     */
    using Result = VirtualMachine::ExecutionResult;

    /**
     * @brief Construct a new XPathQuery object. Lexes, parses and compiles the query to VirtualMachine bytecode.
     * 
     * @param query 
     */
    XPathQuery(std::string_view query);

    /**
     * @brief Executes the query on the given node.
     * 
     * @param node 
     * @return ExecutionResult 
     */
    Result execute(Node* node);
};
};