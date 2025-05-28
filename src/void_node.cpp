#include "void_node.h"

namespace Templater::dynamic {
bool VoidNode::isVoid() const { return true; }

VoidNode::VoidNode(std::vector<Attribute> attributes)
    : Node(std::move(attributes), {}) {}

VoidNode::VoidNode(NonOwningNodeTag, std::vector<Attribute> attributes)
    : Node(NonOwning, std::move(attributes), {}) {}
}  // namespace Templater::dynamic