#pragma once

#include <string_view>

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
     * @brief Holds the execution result objects, as well as the and the storage
     * for the temporary nodes, such as AttributeViewName and RootViewNode.
     *
     */
    using Result = VirtualMachine::ExecutionResult;

    /**
     * @brief Construct a new XPathQuery object. Lexes, parses and compiles the
     * query to VirtualMachine bytecode.
     *
     * @param query
     * @param namespaceResolver Resolves namespaces
     */
    XPathQuery(
        std::string_view query,
        std::function<std::string(std::string_view)> namespaceResolver =
            [](std::string_view namespacePrefix) -> std::string {
            throw std::runtime_error(
                "Found namespace prefix that cannot be resolved " +
                std::string(namespacePrefix));
        });

    /**
     * @brief Executes the query on the given node.
     *
     * @param node
     * @param variableProvider Resolves variables
     * @return Result
     */
    Result execute(
        Node* node,
        std::function<XPathObject(std::string_view, std::string_view)>
            variableProvider = [](std::string_view uri,
                                  std::string_view localName) -> XPathObject {
            throw std::runtime_error("Found unresolved variable reference");
        });
};
};  // namespace onyx::dynamic::xpath