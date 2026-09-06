#include <optional>
#include <stack>
#include <stdexcept>
#include <string_view>

#include "attribute.h"
#include "nodes/attribute_view_node.h"
#include "nodes/comment_node.h"
#include "nodes/namespace_view_node.h"
#include "nodes/processing_instruction_node.h"
#include "nodes/root_view_node.h"
#include "nodes/util/qualified_name.h"
#include "xpath/axis.h"
#include "xpath/virtual_machine.h"

namespace onyx::dynamic::xpath {

bool VirtualMachine::nodeMatchesTest(Node* node, AXIS axis,
                                     const std::string& uri,
                                     const std::string& localName) {
    Node::XPathType type = node->getXPathType();

    if (type == Node::XPathType::OTHER) return false;

    Node::XPathType principalType = Node::XPathType::ELEMENT;
    if (axis == AXIS::ATTRIBUTE) {
        principalType = Node::XPathType::ATTRIBUTE;
    } else if (axis == AXIS::NAMESPACE) {
        principalType = Node::XPathType::NAMESPACE;
    }

    if (localName == "node()") return true;
    if (localName == "text()") return type == Node::XPathType::TEXT;
    if (localName == "comment()") return type == Node::XPathType::COMMENT;
    if (localName.starts_with("processing-instruction(")) {
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
        if (localName != "processing-instruction()") {
            // processing-instruction(" has 24 characters
            // This can throw an out of bounds
            // This is a valid error
            std::string target = localName.substr(24);

            if (localName.starts_with("processing-instruction(\"")) {
                if (target.ends_with("\")")) {
                    target.pop_back();
                    target.pop_back();

                    return pi->getTarget() == target;
                }
            } else if (localName.starts_with("processing-instruction('")) {
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

    if (uri == "" && localName == "*") {
        return type == principalType;
    }

    if (type == principalType) {
        if (type == Node::XPathType::NAMESPACE) {
            NamespaceViewNode* namespaceNode =
                static_cast<NamespaceViewNode*>(node);

            // Namespace nodes do not possess a namespace URI of their own
            // So if the query has a prefix (for example, 'namespace::foo:bar'),
            // it fails.
            if (!uri.empty()) return false;

            return localName == "*" || namespaceNode->getPrefix() == localName;
        }

        std::optional<std::string_view> expectedNamespace =
            uri.empty() ? std::optional<std::string_view>(std::nullopt)
                        : std::optional<std::string_view>(uri);

        if (type == Node::XPathType::ATTRIBUTE) {
            AttributeViewNode* attributeNode =
                static_cast<AttributeViewNode*>(node);
            const Attribute& attribute =
                attributeNode->getReferencedAttribute();

            std::optional<std::string_view> attributeNamespace =
                attributeNode->getParentNode()->resolveAttributeNamespacePrefix(
                    attribute.getNamespacePrefix());

            if (attributeNamespace != expectedNamespace) return false;

            return localName == "*" ||
                   attribute.getNCNameWithoutNamespace() == localName;
        } else {
            if (node->getNamespaceName() != expectedNamespace) return false;
            return localName == "*" || node->getTagName() == localName;
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
                                        const std::string& uri,
                                        const std::string& localName,
                                        std::vector<Node*>& result) {
    current->iterateDirectChildrenReverse(
        [&current, axis, &uri, &localName, &result, this](Node* child) {
            child->iterativeProcessor([&current, axis, &uri, &localName,
                                       &result, this](Node* child1) -> void {
                if (nodeMatchesTest(child1, axis, uri, localName)) {
                    result.push_back(child1);
                }
            });
        });
}

void VirtualMachine::collectChildren(Node* current, AXIS axis,
                                     const std::string& uri,
                                     const std::string& localName,
                                     std::vector<Node*>& result) {
    current->iterateDirectChildren(
        [&axis, &uri, &localName, &result, this](Node* child) {
            if (nodeMatchesTest(child, axis, uri, localName)) {
                result.push_back(child);
            }
        });
}

void VirtualMachine::collectParent(Node* current, AXIS axis,
                                   const std::string& uri,
                                   const std::string& localName,
                                   std::vector<Node*>& result,
                                   DocumentRoot& root) {
    Node* parent = current->getParentNode();
    if (parent && nodeMatchesTest(parent, axis, uri, localName)) {
        result.push_back(parent);
    } else if (!parent && localName == "node()") {
        root.findRoot(current);
        result.push_back(root.getXPathRoot());
    }
}

void VirtualMachine::collectAncestor(Node* current, AXIS axis,
                                     const std::string& uri,
                                     const std::string& localName,
                                     std::vector<Node*>& result,
                                     DocumentRoot& root) {
    Node* parent = current->getParentNode();
    while (parent) {
        if (nodeMatchesTest(parent, axis, uri, localName)) {
            result.push_back(parent);
        }
        parent = parent->getParentNode();
    }
    if (localName == "node()") {
        root.findRoot(current);
        result.push_back(root.getXPathRoot());
    }
}

void VirtualMachine::collectFollowingSiblings(Node* current, AXIS axis,
                                              const std::string& uri,
                                              const std::string& localName,
                                              std::vector<Node*>& result) {
    Node* sibling = current->getNextSibling();
    Node* original = sibling;
    if (!sibling) return;

    do {
        if (nodeMatchesTest(sibling, axis, uri, localName)) {
            result.push_back(sibling);
        }
        sibling = sibling->getNextSibling();
    } while (sibling != original);
}

void VirtualMachine::collectPrecedingSiblings(Node* current, AXIS axis,
                                              const std::string& uri,
                                              const std::string& localName,
                                              std::vector<Node*>& result) {
    Node* sibling = current->getPrevSibling();
    Node* original = sibling;
    do {
        if (nodeMatchesTest(sibling, axis, uri, localName)) {
            result.push_back(sibling);
        }
        sibling = sibling->getPrevSibling();
    } while (sibling != original);
}

void VirtualMachine::collectPreceding(Node* current, AXIS axis,
                                      const std::string& uri,
                                      const std::string& localName,
                                      std::vector<Node*>& result,
                                      ExecutionContext& ec) {
    ec.root.findRoot(current);
    ec.order.buildIndex(ec.root.getDocumentRoot());

    size_t index = ec.order.documentOrderMap[current];

    size_t i = index;
    while (i > 0) {
        i--;
        if (nodeMatchesTest(ec.order.documentOrderList[i], axis, uri,
                            localName) &&
            !isAncestor(ec.order.documentOrderList[i], current)) {
            result.push_back(ec.order.documentOrderList[i]);
        }
    }

    if (localName == "node()") {
        result.push_back(ec.root.getXPathRoot());
    }
}

void VirtualMachine::collectFollowing(Node* current, AXIS axis,
                                      const std::string& uri,
                                      const std::string& localName,
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
        if (nodeMatchesTest(ec.order.documentOrderList[i], axis, uri,
                            localName)) {
            result.push_back(ec.order.documentOrderList[i]);
        }
    }
}
}  // namespace onyx::dynamic::xpath
