#include "nodes/foreach_node.h"

namespace onyx::dynamic::tags {
ForEach::ForEach() : Node{} {}

const std::string& ForEach::getTagName() const {
    static const std::string name = ".foreach";
    return name;
}

bool ForEach::isVoid() const { return false; }

std::unique_ptr<Node> ForEach::shallowCopy() const {
    return std::make_unique<ForEach>();
}

void ForEach::specialSerialize(std::vector<Node::SerializationNode>& stack,
                               std::ostringstream& result) const {
    stack.pop_back();
    this->iterateDirectChildrenReverse([&stack](const Node* current) {
        stack.emplace_back(SerializationNode{current, false});
    });
}

void ForEach::specialSerializePretty(
    std::vector<Node::SerializationNode>& stack, std::ostringstream& result,
    std::string& indentation, const std::string& indentationSequence,
    bool sortAttributes) const {
    specialSerialize(stack, result);
}

bool ForEach::hasSpecialSerialization() const { return true; }

Node::XPathType ForEach::getXPathType() const { return XPathType::OTHER; }
}  // namespace onyx::dynamic::tags