#include "void_node.h"

namespace Templater::dynamic {
    bool VoidNode::isVoid() const {
        return true;
    }

    VoidNode::VoidNode(std::vector<Attribute> attributes): Node(std::move(attributes), {}) {}
}