#include "nodes/attribute_view_node.h"

namespace onyx::dynamic::xpath {
AttributeViewNode::AttributeViewNode(Node* owner, size_t index)
    : Node{}, owner{owner}, index{index} {}

const std::string& AttributeViewNode::getTagName() const {
    static const std::string name = ".attribute-view-node";
    return name;
}

std::string AttributeViewNode::serialize() const { return ""; }

std::string AttributeViewNode::serializePretty(
    const std::string& indentationSequence, bool sortAttributes) const {
    return "";
}

bool AttributeViewNode::hasSpecialSerialization() const { return false; }

std::unique_ptr<Node> AttributeViewNode::shallowCopy() const {
    return std::make_unique<AttributeViewNode>(this->owner, this->index);
}

Node* AttributeViewNode::getRealNode() { return this->owner; }

const Attribute& AttributeViewNode::getReferencedAttribute() const {
    return this->owner->getAttributes()[index];
}

bool AttributeViewNode::shallowEquals(const Node& other) const {
    if (this == &other) return true;

    if (const AttributeViewNode* oth =
            dynamic_cast<const AttributeViewNode*>(&other)) {
        return this->getReferencedAttribute().getValue() ==
               oth->getReferencedAttribute().getValue();
    }

    return false;
}

bool AttributeViewNode::isVoid() const { return true; }

std::string AttributeViewNode::getStringValue() const {
    return this->getReferencedAttribute().getValue();
}

bool AttributeViewNode::hasShallowStringValue() const { return true; }
}  // namespace onyx::dynamic::xpath