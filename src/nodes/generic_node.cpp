#include "nodes/generic_node.h"

#include "namespace_node.h"

namespace onyx::dynamic::tags {

GenericNode::GenericNode(std::string namespacePrefix, std::string tagName,
                         Type type)
    : tag{std::move(tagName)}, NamespaceNode(std::move(namespacePrefix)) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(type == Type::Void);
}

GenericNode::GenericNode(util::QualifiedNameView qualifiedNameView, Type type)
    : GenericNode(std::string(qualifiedNameView.prefix),
                  std::string(qualifiedNameView.name), type) {}

GenericNode::GenericNode(std::string namespacePrefix, std::string tagName,
                         Type type, std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : tag{std::move(tagName)},
      NamespaceNode{std::move(namespacePrefix), std::move(attributes),
                    std::move(children)} {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(type == Type::Void);

    if (type == Type::Void && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void node cannot have children.");
    }
}

GenericNode::GenericNode(util::QualifiedNameView qualifiedNameView, Type type,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : GenericNode{std::string(qualifiedNameView.prefix),
                  std::string(qualifiedNameView.name), type,
                  std::move(attributes), std::move(children)} {}

GenericNode::GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, Type type)
    : tag{std::move(tagName)},
      NamespaceNode(NonOwning, std::move(namespacePrefix)) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(type == Type::Void);
}

GenericNode::GenericNode(NonOwningNodeTag,
                         util::QualifiedNameView qualifiedNameView, Type type)
    : GenericNode(NonOwning, std::string(qualifiedNameView.prefix),
                  std::string(qualifiedNameView.name), type) {}

GenericNode::GenericNode(NonOwningNodeTag, std::string namespacePrefix,
                         std::string tagName, Type type,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : tag{std::move(tagName)},
      NamespaceNode{NonOwning, std::move(namespacePrefix),
                    std::move(attributes), std::move(children)} {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(type == Type::Void);

    if (type == Type::Void && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void node cannot have children.");
    }
}

GenericNode::GenericNode(NonOwningNodeTag,
                         util::QualifiedNameView qualifiedNameView, Type type,
                         std::vector<Attribute> attributes,
                         std::vector<NodeHandle>&& children)
    : GenericNode{NonOwning,
                  std::string(qualifiedNameView.prefix),
                  std::string(qualifiedNameView.name),
                  type,
                  std::move(attributes),
                  std::move(children)} {}

void GenericNode::setTagName(std::string newName) {
    this->tag = newName;
    this->updateAndPropagateUp(IndexPropagationMessage::UPDATE);
}

void GenericNode::setIsVoid(Type type) {
    this->setFlag<FlagBitIndices::BIT_IS_VOID>(type == Type::Void);

    this->updateAndPropagateUp(IndexPropagationMessage::UPDATE);
}

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
        this->tag, this->isVoid() ? Type::Void : Type::NonVoid,
        this->getAttributes(), std::vector<NodeHandle>{});
}
}  // namespace onyx::dynamic::tags