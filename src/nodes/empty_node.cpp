#include "empty_node.h"

namespace Templater::dynamic::dtags {

    const std::string& EmptyNode::getTagName() const {
        static const std::string name = ".empty";
        return name;
    }

    bool EmptyNode::isVoid() const {
        return false;
    }
}