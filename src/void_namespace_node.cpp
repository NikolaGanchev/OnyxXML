#include "void_namespace_node.h"

namespace onyx::dynamic {
bool VoidNamespaceNode::isVoid() const { return true; }

VoidNamespaceNode::VoidNamespaceNode(std::string namespacePrefix,
                                     std::vector<Attribute> attributes)
    : NamespaceNode(std::move(namespacePrefix), std::move(attributes), {}) {}

VoidNamespaceNode::VoidNamespaceNode(NonOwningNodeTag,
                                     std::string namespacePrefix,
                                     std::vector<Attribute> attributes)
    : NamespaceNode(NonOwning, std::move(namespacePrefix),
                    std::move(attributes), {}) {}
}  // namespace onyx::dynamic