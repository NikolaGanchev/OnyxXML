#include "nodes/if_node.h"

namespace onyx::dynamic::tags {
If::If() : Node{} {}

const std::string& If::getTagName() const {
    static const std::string name = ".if";
    return name;
}

bool If::isVoid() const { return false; }

std::unique_ptr<Node> If::shallowCopy() const { return std::make_unique<If>(); }

void If::specialSerialize(std::vector<Node::SerializationNode>& stack,
                          std::ostringstream& result) const {
    stack.pop_back();
    const Node* current = this->getLastChild();
    while (current != nullptr) {
        stack.emplace_back(SerializationNode{current, false});
        current = current->getPrevSibling();
    }
}

void If::specialSerializePretty(std::vector<Node::SerializationNode>& stack,
                                std::ostringstream& result,
                                std::string& indentation,
                                const std::string& indentationSequence,
                                bool sortAttributes) const {
    specialSerialize(stack, result);
}

bool If::hasSpecialSerialization() const { return true; }

Node::XPathType If::getXPathType() const { return XPathType::OTHER; }
}  // namespace onyx::dynamic::tags