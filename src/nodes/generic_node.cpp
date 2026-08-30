#include "nodes/generic_node.h"

#include "namespace_node.h"

namespace onyx::dynamic::tags {

GenericNode::GenericNode(std::string namespacePrefix, std::string tagName,
                         bool isVoid)
    : tag{std::move(tagName)}, NamespaceNode(std::move(namespacePrefix)) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(isVoid);
}

GenericNode::GenericNode(std::string tagName, bool isVoid)
    : GenericNode("", std::move(tagName), isVoid) {}

GenericNode::GenericNode(std::string namespacePrefix, std::string tagName,
                         bool isVoid, std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : tag{std::move(tagName)},
      NamespaceNode{std::move(namespacePrefix), std::move(attributes),
                    std::move(children)} {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(isVoid);

    if (isVoid && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void " + getTagName() +
                                 " cannot have children.");
    }
}

GenericNode::GenericNode(std::string tagName, bool isVoid,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : GenericNode{"", std::move(tagName), isVoid, std::move(attributes),
                  std::move(children)} {}

GenericNode::GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, bool isVoid)
    : tag{std::move(tagName)},
      NamespaceNode(NonOwning, std::move(namespacePrefix)) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(isVoid);
}

GenericNode::GenericNode(NonOwningNodeTag, std::string tagName, bool isVoid)
    : GenericNode(NonOwning, "", std::move(tagName), isVoid) {}

GenericNode::GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, bool isVoid,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : tag{std::move(tagName)},
      NamespaceNode{NonOwning, std::move(namespacePrefix),
                    std::move(attributes), std::move(children)} {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(isVoid);

    if (isVoid && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void " + getTagName() +
                                 " cannot have children.");
    }
}

GenericNode::GenericNode(NonOwningNodeTag, std::string tagName, bool isVoid,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : GenericNode{NonOwning,
                  "",
                  std::move(tagName),
                  isVoid,
                  std::move(attributes),
                  std::move(children)} {}

const std::string& GenericNode::getTagName() const { return this->tag; }

bool GenericNode::isVoid() const {
    return this->getFlag<FlagBitIndices::BIT_IS_VOID>();
}

GenericNode::GenericNode(Node&& other) noexcept
    : tag{other.getTagName()}, NamespaceNode{std::move(other)} {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(other.isVoid());
};

GenericNode& GenericNode::operator=(Node&& other) noexcept {
    if (this == &other) return *this;
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(other.isVoid());
    this->tag = other.getTagName();
    Node::operator=(std::move(other));

    return *this;
}

std::unique_ptr<Node> GenericNode::shallowCopy() const {
    return std::make_unique<GenericNode>(
        std::string(this->getNamespacePrefix().has_value()
                        ? this->getNamespacePrefix().value()
                        : ""),
        this->tag, this->isVoid(), this->getAttributes(),
        std::vector<NodeHandle>{});
}
}  // namespace onyx::dynamic::tags