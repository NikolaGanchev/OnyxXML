#include "generic_node.h"

namespace Templater::dynamic::tags {
    
    GenericNode::GenericNode(std::string tagName, bool isVoid)
            : tag{std::move(tagName)}, _isVoid{isVoid}, Node() {}

    GenericNode::GenericNode(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::unique_ptr<Node>>&& children)
            : tag{std::move(tagName)}, _isVoid{isVoid}, Node{std::move(attributes), std::move(children)} {
                if (isVoid && this->getChildrenCount() > 0) {
                    throw std::runtime_error("Void " + getTagName() + " cannot have children.");
                }
    }

    const std::string& GenericNode::getTagName() const {
        return this->tag;
    }

    bool GenericNode::isVoid() const {
        return this->_isVoid;
    }

    GenericNode::GenericNode(Node&& other) noexcept: tag{other.getTagName()}, _isVoid{other.isVoid()}, Node{std::move(other)} {};

    GenericNode& GenericNode::operator=(Node&& other) noexcept {
        if (this == &other) return *this;
        this->_isVoid = other.isVoid();
        this->tag = other.getTagName();
        Node::operator=(std::move(other));

        return *this;
    }

    std::unique_ptr<Node> GenericNode::shallowCopy() const {
        return std::make_unique<GenericNode>(this->tag, this->_isVoid, this->getAttributes(), std::vector<std::unique_ptr<Node>>{});
    }
}