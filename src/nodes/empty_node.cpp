#include "empty_node.h"

namespace Templater::dynamic::tags {

    const std::string& EmptyNode::getTagName() const {
        static const std::string name = ".empty";
        return name;
    }

    bool EmptyNode::isVoid() const {
        return false;
    }
    
    std::unique_ptr<Node> EmptyNode::shallowCopy() const {
        return std::make_unique<EmptyNode>(std::vector<Attribute>{}, std::vector<std::unique_ptr<Node>>{});
    }

    void EmptyNode::specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const {
        stack.pop_back();
        const auto& children = this->getChildrenLive();
        for (size_t i = children.size(); i > 0; --i) {
            stack.emplace_back(SerializationNode{children[i-1], false});
        }
    }

    void EmptyNode::specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const {
        specialSerialize(stack, result);
    }
    
    bool EmptyNode::hasSpecialSerialization() const {
        return true;
    }
}