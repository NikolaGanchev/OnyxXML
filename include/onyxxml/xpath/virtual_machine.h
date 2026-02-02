#pragma once

#include <memory>
#include <stack>

#include "axis.h"
#include "calculate_mode.h"
#include "compare_mode.h"
#include "function_code.h"
#include "program.h"
#include "xpath_object.h"

namespace onyx::dynamic::xpath {
/**
 * @brief A virtual machine for the defined bytecode. Stores a program and can
 * run it repeatedly over different nodes.
 *
 */
class VirtualMachine {
   private:
    /**
     * @brief The internal program
     *
     */
    std::unique_ptr<Program> program;

    /**
     * @brief A struct for the data of an execution frame,
     * most commonly a loop.
     *
     */
    struct FrameContext {
        /**
         * @brief The current set of nodes
         *
         */
        std::vector<Node*> contextSet;

        /**
         * @brief The index of the context node
         *
         */
        size_t currentIndex;

        /**
         * @brief The start address of the loop for automatic returns
         *
         */
        size_t startAddress;

        /**
         * @brief Nodes that have been added by the CONTEXT_NODE_TEST
         * instruction or the LOOP_UNION instruction
         *
         */
        std::vector<Node*> result;

        /**
         * @brief If the LOOP_UNION instruction has been used.
         *
         */
        bool hasUsedLoopUnion;

        /**
         * @brief The size of the current set of nodes
         *
         * @return size_t
         */
        size_t getContextSize() const { return contextSet.size(); }
    };

    /**
     * @brief The data stack type
     *
     */
    using DataStack = std::stack<XPathObject, std::vector<XPathObject>>;

    /**
     * @brief The function type
     *
     */
    using FunctionType = XPathObject (*)(const FrameContext&, DataStack&);

    /**
     * @brief The function registry type
     *
     */
    using FunctionRegistry = std::unordered_map<FUNCTION_CODE, FunctionType>;

    /**
     * @brief Creates the function registry
     *
     * @return FunctionRegistry
     */
    static FunctionRegistry registerFunctions();

    /**
     * @brief Nodesets in XPaths by definition require being sorted in document
     * order This being the order they appear in the document Calculating this
     * dynamically is a wasteful operation This index serializes the Node tree
     * into a continuos list in document order Including a map with the mapping
     * between pointers and the indices in the list This allows fast sorting and
     * access of the document order
     *
     */
    struct DocumentOrder {
        /**
         * @brief Map from Node* to the position in the order list
         *
         */
        std::unordered_map<Node*, size_t> documentOrderMap{};
        /**
         * @brief The list of the Nodes in document order
         *
         */
        std::vector<Node*> documentOrderList{};

        /**
         * @brief Initializes the index based on the root node
         *
         * @param root
         */
        void buildIndex(Node* root);

        /**
         * @brief Compare equivalent to < by document order. With other words,
         * a < b iff a is before b in document order.
         *
         * @param a
         * @param b
         * @return true
         * @return false
         */
        bool compare(Node* a, Node* b);

        /**
         * @brief Takes a set and normalizes it.
         * Duplicates are removed and the set is sorted by document order.
         *
         * @param nodeset
         */
        void normalizeDocumentOrderSet(std::vector<Node*>& nodeset);
    };

    class ExecutionContext;

    /**
     * @brief Structure for dealing with the document root.
     * In XPath the root is not actually the first node without a parent,
     * but actually a logical element that exists above that
     * This forces the distinction between the documentRoot and the xpathRoot
     * This structure calculates them lazily to save resources when they are
     * never used.
     *
     */
    class DocumentRoot {
       private:
        /**
         * @brief The real document root
         *
         */
        Node* documentRoot = nullptr;
        /**
         * @brief The logical XPath root
         *
         */
        Node* xpathRoot = nullptr;

        /**
         * @brief The execution context to store temp nodes in
         *
         */
        ExecutionContext& ec;

       public:
        /**
         * @brief Construct a new Document Root object
         *
         * @param ec
         */
        DocumentRoot(ExecutionContext& ec);
        /**
         * @brief Initializes the roots
         *
         * @param any
         */
        void findRoot(Node* any);

        /**
         * @brief Get the document root
         *
         * @return Node*
         */
        Node* getDocumentRoot();

        /**
         * @brief Get the XPath root
         *
         * @return Node*
         */
        Node* getXPathRoot();
    };

    class ExecutionContext {
       public:
        DataStack dataStack;
        std::stack<FrameContext, std::vector<FrameContext>> contextStack;
        size_t instructionPointer = 0;
        DocumentRoot root;
        DocumentOrder order;
        std::vector<std::unique_ptr<Node>> temporaryNodes;

        ExecutionContext();
    };

    /**
     * @brief Execute the SELECT instruction
     *
     * @param instruction
     * @param ec
     */
    void executeSelect(const Instruction& instruction, ExecutionContext& ec);

    /**
     * @brief Execute the UNION instruction
     *
     * @param instruction
     * @param ec
     */
    void executeUnion(const Instruction& instruction, ExecutionContext& ec);

    /**
     * @brief Execute the SORT instruction
     *
     * @param instruction
     * @param ec
     */
    void executeSort(const Instruction& instruction, ExecutionContext& ec);

    /**
     * @brief Execute the CALCULATE instruction
     *
     * @param instruction
     * @param ec
     */
    void executeCalculate(const Instruction& instruction, ExecutionContext& ec);

    /**
     * @brief Execute the COMPARE instruction
     *
     * @param instruction
     * @param ec
     */
    void executeCompare(const Instruction& instruction, ExecutionContext& ec);

    /**
     * @brief Collect descendants of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     */
    void collectDescendants(Node* current, AXIS axis, const std::string& test,
                            std::vector<Node*>& result);

    /**
     * @brief Collect children of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     */
    void collectChildren(Node* current, AXIS axis, const std::string& test,
                         std::vector<Node*>& result);

    /**
     * @brief Collect parent of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     * @param root
     */
    void collectParent(Node* current, AXIS axis, const std::string& test,
                       std::vector<Node*>& result, DocumentRoot& root);

    /**
     * @brief Collect ancestors of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     * @param root
     */
    void collectAncestor(Node* current, AXIS axis, const std::string& test,
                         std::vector<Node*>& result, DocumentRoot& root);

    /**
     * @brief Collect following siblings of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     */
    void collectFollowingSiblings(Node* current, AXIS axis,
                                  const std::string& test,
                                  std::vector<Node*>& result);

    /**
     * @brief Collect following siblings of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     */
    void collectPrecedingSiblings(Node* current, AXIS axis,
                                  const std::string& test,
                                  std::vector<Node*>& result);

    /**
     * @brief Collect preceding of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     * @param ec
     */
    void collectPreceding(Node* current, AXIS axis, const std::string& test,
                          std::vector<Node*>& result, ExecutionContext& ec);

    /**
     * @brief Collect following of the node
     *
     * @param current
     * @param axis
     * @param test
     * @param result
     * @param ec
     */
    void collectFollowing(Node* current, AXIS axis, const std::string& test,
                          std::vector<Node*>& result, ExecutionContext& ec);

    /**
     * @brief Checks if a node passes a test along an axis
     *
     * @param node
     * @param axis
     * @param test
     * @return true
     * @return false
     */
    bool nodeMatchesTest(Node* node, AXIS axis, const std::string& test);

   public:
    /**
     * @brief Contains the result XPathObject object and the storage for the
     * temporary nodes, such as AttributeViewName and RootViewNode.
     *
     */
    class ExecutionResult {
       private:
        /**
         * @brief Manages the temporary Nodes created only for parsing, such as
         * the RootViewNode or the AttributeViewName nodes
         *
         */
        std::vector<std::unique_ptr<Node>> temporaryNodes;

       public:
        /**
         * @brief Construct a new ExecutionResult object
         *
         * @param tempStorage
         * @param obj
         */
        ExecutionResult(std::vector<std::unique_ptr<Node>>&& tempStorage,
                        XPathObject&& obj)
            : temporaryNodes{std::move(tempStorage)}, object{std::move(obj)} {}

        /**
         * @brief The result object
         *
         */
        XPathObject object;
    };

    /**
     * @brief Construct a new VirtualMachine object
     *
     * @param program
     */
    VirtualMachine(std::unique_ptr<Program> program);

    /**
     * @brief Executes the stored program on the given Node and returns the
     * execution result.
     *
     * @param current
     * @return ExecutionResult
     */
    ExecutionResult executeOn(Node* current, std::function<XPathObject(std::string_view)> variableProvider = [](std::string_view v) -> XPathObject {
                                                throw std::runtime_error("Found unresolved variable reference to " + std::string(v));
                                            });
};
}  // namespace onyx::dynamic::xpath