#include "empty_node.h"

namespace Templater::dynamic::dtags {

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
}