#include "nodes/empty_node.h"

namespace onyx::dynamic::tags {

const std::string& EmptyNode::getTagName() const {
    static const std::string name = ".empty";
    return name;
}

bool EmptyNode::isVoid() const { return false; }

std::unique_ptr<Node> EmptyNode::shallowCopy() const {
    return std::make_unique<EmptyNode>(std::vector<Attribute>{},
                                       std::vector<NodeHandle>{});
}

void EmptyNode::specialSerialize(std::vector<Node::SerializationNode>& stack,
                                 std::ostringstream& result) const {
    stack.pop_back();
    const Node* current = this->getLastChild();
    while (current != nullptr) {
        stack.emplace_back(SerializationNode{current, false});
        current = current->getPrevSibling();
    }
}

void EmptyNode::specialSerializePretty(
    std::vector<Node::SerializationNode>& stack, std::ostringstream& result,
    std::string& indentation, const std::string& indentationSequence,
    bool sortAttributes) const {
    specialSerialize(stack, result);
}

bool EmptyNode::hasSpecialSerialization() const { return true; }

Node::XPathType EmptyNode::getXPathType() const { return XPathType::OTHER; }
}  // namespace onyx::dynamic::tags