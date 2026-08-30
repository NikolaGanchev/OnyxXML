#include "namespace_node.h"

#include <optional>
#include <string_view>

namespace onyx::dynamic {

NamespaceNode::NamespaceNode(std::string namespacePrefix)
    : namespacePrefix{std::move(namespacePrefix)}, Node() {}

NamespaceNode::NamespaceNode(std::string namespacePrefix,
                             std::vector<Attribute> attributes,
                             std::vector<NodeHandle>&& children)
    : namespacePrefix{std::move(namespacePrefix)},
      Node{std::move(attributes), std::move(children)} {}

NamespaceNode::NamespaceNode(NonOwningNodeTag, std::string namespacePrefix)
    : namespacePrefix{std::move(namespacePrefix)}, Node(NonOwning) {}

NamespaceNode::NamespaceNode(NonOwningNodeTag, std::string namespacePrefix,
                             std::vector<Attribute> attributes,
                             std::vector<NodeHandle>&& children)
    : namespacePrefix{std::move(namespacePrefix)},
      Node{NonOwning, std::move(attributes), std::move(children)} {}

NamespaceNode::NamespaceNode(Node&& other) noexcept : Node{std::move(other)} {
    std::optional<std::string_view> prefix = other.getNamespacePrefix();
    this->namespacePrefix = prefix.has_value() ? prefix.value() : "";
};

NamespaceNode& NamespaceNode::operator=(Node&& other) noexcept {
    if (this == &other) return *this;
    std::optional<std::string_view> prefix = other.getNamespacePrefix();
    this->namespacePrefix = prefix.has_value() ? prefix.value() : "";
    Node::operator=(std::move(other));

    return *this;
}

std::optional<std::string_view> NamespaceNode::getNamespacePrefix() const {
    return namespacePrefix;
}
}  // namespace onyx::dynamic