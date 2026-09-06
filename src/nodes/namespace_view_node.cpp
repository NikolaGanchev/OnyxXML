#include "nodes/namespace_view_node.h"

namespace onyx::dynamic::xpath {

NamespaceViewNode::NamespaceViewNode(Node* owner, std::string_view prefix,
                                     std::string_view uri)
    : Node{}, owner{owner}, prefix{prefix}, uri{uri} {
    this->parent = owner;
}

NamespaceViewNode::NamespaceViewNode(NamespaceViewNode&& other) noexcept
    : owner{other.owner},
      prefix{other.prefix},
      uri{other.uri},
      Node{std::move(other)} {
    this->parent = owner;
    other.owner = nullptr;
}

const std::string& NamespaceViewNode::getTagName() const {
    static const std::string name = ".namespace-view-node";
    return name;
}

std::string NamespaceViewNode::serialize() const { return ""; }

std::string NamespaceViewNode::serializePretty(
    const std::string& indentationSequence, bool sortAttributes) const {
    return "";
}

bool NamespaceViewNode::hasSpecialSerialization() const { return false; }

std::unique_ptr<Node> NamespaceViewNode::shallowCopy() const {
    return std::make_unique<NamespaceViewNode>(this->owner, this->prefix,
                                               this->uri);
}

Node* NamespaceViewNode::getRealNode() const { return this->owner; }

std::string_view NamespaceViewNode::getPrefix() const { return this->prefix; }

std::string_view NamespaceViewNode::getUri() const { return this->uri; }

bool NamespaceViewNode::shallowEquals(const Node& other) const {
    if (this == &other) return true;

    if (const NamespaceViewNode* otherNamespaceView =
            dynamic_cast<const NamespaceViewNode*>(&other)) {
        return this->uri == otherNamespaceView->uri;
    }

    return false;
}

bool NamespaceViewNode::isVoid() const { return true; }

std::string NamespaceViewNode::getStringValue() const {
    return std::string(this->uri);
}

Node::XPathType NamespaceViewNode::getXPathType() const {
    return XPathType::NAMESPACE;
}

}  // namespace onyx::dynamic::xpath