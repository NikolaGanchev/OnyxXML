#include "nodes/if_node.h"

namespace Templater::dynamic::tags {
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
    const auto& children = this->getChildrenLive();
    for (size_t i = children.size(); i > 0; --i) {
        stack.emplace_back(SerializationNode{children[i - 1], false});
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
}  // namespace Templater::dynamic::tags