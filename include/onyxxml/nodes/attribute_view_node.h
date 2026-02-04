#pragma once

#include "../node.h"

namespace onyx::dynamic::xpath {

/**
 * @brief A non-owning view over a node and an attribute index. Used for XPath.
 * There are no special protections against including it in general trees or
 * adding children, but it is unintended behaviour.
 *
 */
class AttributeViewNode : public Node {
   private:
    Node* owner;
    size_t index;

   public:
    /**
     * @brief Construct a new Attribute View Node object
     *
     * @param owner
     * @param index
     */
    AttributeViewNode(Node* owner, size_t index);

    /**
     * @brief AttributeViewNode move
     * 
     * @param other 
     */
    AttributeViewNode(AttributeViewNode&& other) noexcept;

    /**
     * @brief Uses the special xml tag name ".attribute-view-node"
     *
     * @return const std::string&
     */
    const std::string& getTagName() const override;

    /**
     * @brief Returns the empty string
     *
     * @return std::string
     */
    std::string serialize() const override;

    /**
     * @brief Returns the empty string
     *
     * @param indentationSequence
     * @param sortAttributes
     * @return std::string
     */
    std::string serializePretty(const std::string& indentationSequence,
                                bool sortAttributes) const override;

    /**
     * @brief Returns false
     *
     * @return true
     * @return false
     */
    bool hasSpecialSerialization() const override;

    /**
     * @brief Creates a copy
     *
     * @return std::unique_ptr<Node>
     */
    std::unique_ptr<Node> shallowCopy() const override;

    /**
     * @brief Get the real Node this is a view of
     *
     * @return Node*
     */
    Node* getRealNode() const;

    /**
     * @brief Get the index at which this Node is in the Attribute structure of the parent
     * 
     * @return size_t 
     */
    size_t getAttributeOffset() const;

    /**
     * @brief Get a constant reference to the referenced Attribute
     *
     * @return const Attribute&
     */
    const Attribute& getReferencedAttribute() const;

    /**
     * @brief Returns true
     *
     */
    bool isVoid() const override;

    /**
     * @brief Tests equality only against attribute values
     *
     * @param other
     * @return true
     * @return false
     */
    bool shallowEquals(const Node& other) const override;

    std::string getStringValue() const override;
    XPathType getXPathType() const override;
};
}  // namespace onyx::dynamic::xpath