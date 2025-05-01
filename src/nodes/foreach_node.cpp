#include "foreach_node.h"

namespace Templater::dynamic::dtags {
    ForEach::ForEach(): Node{} {}

    const std::string& ForEach::getTagName() const {
        static const std::string name = ".foreach";
        return name;
    }

    bool ForEach::isVoid() const {
        return false;
    }
    
    std::unique_ptr<Node> ForEach::shallowCopy() const {
        return std::make_unique<ForEach>();
    }

    void ForEach::specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const {
        stack.pop_back();
        const auto& children = this->getChildrenLive();
        for (size_t i = children.size(); i > 0; --i) {
            stack.emplace_back(SerializationNode{children[i-1].get(), false});
        }
    }

    void ForEach::specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const {
        specialSerialize(stack, result);
    }
    
    bool ForEach::hasSpecialSerialization() const {
        return true;
    }
}