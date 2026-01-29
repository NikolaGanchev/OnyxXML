#include "nodes/root_view_node.h"

namespace onyx::dynamic::xpath {
RootViewNode::RootViewNode(Node* child) : Node{NonOwning} {
    this->firstChild = child;
    this->lastChild = child;
}

const std::string& RootViewNode::getTagName() const {
    static const std::string name = "";
    return name;
}

std::string RootViewNode::serialize() const { return ""; }

std::string RootViewNode::serializePretty(
    const std::string& indentationSequence, bool sortAttributes) const {
    return "";
}

bool RootViewNode::hasSpecialSerialization() const { return false; }

std::unique_ptr<Node> RootViewNode::shallowCopy() const {
    return std::make_unique<RootViewNode>(this->firstChild);
}

bool RootViewNode::isVoid() const { return true; }

Node::XPathType RootViewNode::getXPathType() const { return XPathType::ROOT; }
}  // namespace onyx::dynamic::xpath