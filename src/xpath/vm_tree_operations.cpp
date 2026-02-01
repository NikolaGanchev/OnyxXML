#include <stack>

#include "nodes/attribute_view_node.h"
#include "nodes/comment_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/root_view_node.h"
#include "xpath/axis.h"
#include "xpath/virtual_machine.h"

namespace onyx::dynamic::xpath {

bool VirtualMachine::nodeMatchesTest(Node* node, AXIS axis,
                                     const std::string& test) {
    Node::XPathType type = node->getXPathType();

    if (type == Node::XPathType::OTHER) return false;

    Node::XPathType principalType = (axis == AXIS::ATTRIBUTE)
                                        ? Node::XPathType::ATTRIBUTE
                                        : Node::XPathType::ELEMENT;

    if (test == "node()") return true;
    if (test == "text()") return type == Node::XPathType::TEXT;
    if (test == "comment()") return type == Node::XPathType::COMMENT;
    if (test.starts_with("processing-instruction(")) {
        if (type != Node::XPathType::PROCESSING_INSTRUCTION) return false;

        tags::ProcessingInstruction* pi =
            static_cast<tags::ProcessingInstruction*>(node);

        if (!pi) {
            throw std::runtime_error("Impossible Processing Instruction node");
        }

        // processing instructions can have an additional argument of the type
        // 'processing-instruction' '(' Literal ')'
        // Literal	   ::=   	'"' [^"]* '"'
        //	| "'" [^']* "'"
        // per https://www.w3.org/TR/1999/REC-xpath-19991116/#node-tests
        if (test != "processing-instruction()") {
            // processing-instruction(" has 24 characters
            // This can throw an out of bounds
            // This is a valid error
            std::string target = test.substr(24);

            if (test.starts_with("processing-instruction(\"")) {
                if (target.ends_with("\")")) {
                    target.pop_back();
                    target.pop_back();

                    return pi->getTarget() == target;
                }
            } else if (test.starts_with("processing-instruction('")) {
                if (target.ends_with("')")) {
                    target.pop_back();
                    target.pop_back();

                    return pi->getTarget() == target;
                }
            }

            throw std::runtime_error("Invalid processing instruction test");
        }

        return true;
    }

    if (test == "*") {
        return type == principalType;
    }

    if (type == principalType) {
        if (type == Node::XPathType::ATTRIBUTE) {
            return static_cast<AttributeViewNode*>(node)
                       ->getReferencedAttribute()
                       .getName() == test;
        } else {
            return node->getTagName() == test;
        }
    }

    return false;
}

bool isAncestor(Node* node, Node* desc) {
    while (desc->getParentNode()) {
        desc = desc->getParentNode();
        if (node == desc) return true;
    }

    return false;
}

void VirtualMachine::collectDescendants(Node* current, AXIS axis,
                                        const std::string& test,
                                        std::vector<Node*>& result) {
    Node* child1 = current->getLastChild();
    while (child1 != nullptr) {
        child1->iterativeProcessor(
            [&current, axis, &test, &result, this](Node* child) -> void {
                if (nodeMatchesTest(child, axis, test)) {
                    result.push_back(child);
                }
            });
        child1 = child1->getPrevSibling();
    }
}

void VirtualMachine::collectChildren(Node* current, AXIS axis,
                                     const std::string& test,
                                     std::vector<Node*>& result) {
    Node* child = current->getFirstChild();
    while (child) {
        if (nodeMatchesTest(child, axis, test)) {
            result.push_back(child);
        }
        child = child->getNextSibling();
    }
}

void VirtualMachine::collectParent(Node* current, AXIS axis,
                                   const std::string& test,
                                   std::vector<Node*>& result,
                                   DocumentRoot& root) {
    Node* parent = current->getParentNode();
    if (parent && nodeMatchesTest(parent, axis, test)) {
        result.push_back(parent);
    } else if (!parent && test == "node()") {
        root.findRoot(parent);
        result.push_back(root.getXPathRoot());
    }
}

void VirtualMachine::collectAncestor(Node* current, AXIS axis,
                                     const std::string& test,
                                     std::vector<Node*>& result,
                                     DocumentRoot& root) {
    Node* parent = current->getParentNode();
    while (parent) {
        if (nodeMatchesTest(parent, axis, test)) {
            result.push_back(parent);
        }
        parent = parent->getParentNode();
    }
    if (test == "node()") {
        root.findRoot(parent);
        result.push_back(root.getXPathRoot());
    }
}

void VirtualMachine::collectFollowingSiblings(Node* current, AXIS axis,
                                              const std::string& test,
                                              std::vector<Node*>& result) {
    Node* sibling = current->getNextSibling();
    while (sibling) {
        if (nodeMatchesTest(sibling, axis, test)) {
            result.push_back(sibling);
        }
        sibling = sibling->getNextSibling();
    }
}

void VirtualMachine::collectPrecedingSiblings(Node* current, AXIS axis,
                                              const std::string& test,
                                              std::vector<Node*>& result) {
    Node* sibling = current->getPrevSibling();
    while (sibling) {
        if (nodeMatchesTest(sibling, axis, test)) {
            result.push_back(sibling);
        }
        sibling = sibling->getPrevSibling();
    }
}

void VirtualMachine::collectPreceding(Node* current, AXIS axis,
                                      const std::string& test,
                                      std::vector<Node*>& result,
                                      ExecutionContext& ec) {
    ec.root.findRoot(current);
    ec.order.buildIndex(ec.root.getDocumentRoot());

    size_t index = ec.order.documentOrderMap[current];

    size_t i = index;
    while (i > 0) {
        i--;
        if (nodeMatchesTest(ec.order.documentOrderList[i], axis, test) &&
            !isAncestor(ec.order.documentOrderList[i], current)) {
            result.push_back(ec.order.documentOrderList[i]);
        }
    }

    if (test == "node()") {
        result.push_back(ec.root.getXPathRoot());
    }
}

void VirtualMachine::collectFollowing(Node* current, AXIS axis,
                                      const std::string& test,
                                      std::vector<Node*>& result,
                                      ExecutionContext& ec) {
    ec.root.findRoot(current);
    ec.order.buildIndex(ec.root.getDocumentRoot());

    size_t index = ec.order.documentOrderMap[current];

    size_t i = index + 1;
    while (i < ec.order.documentOrderList.size() &&
           isAncestor(current, ec.order.documentOrderList[i])) {
        i++;
    }

    for (; i < ec.order.documentOrderList.size(); i++) {
        if (nodeMatchesTest(ec.order.documentOrderList[i], axis, test)) {
            result.push_back(ec.order.documentOrderList[i]);
        }
    }
}
}  // namespace onyx::dynamic::xpath
