#pragma once

#include <string_view>

#include "../node.h"


namespace onyx::dynamic::xpath {

/**
 * @brief A non-owning view over a namespace declaration in scope for a node.
 * Used for the XPath namespace:: axis.
 */
class NamespaceViewNode : public Node {
   private:
    Node* owner;
    std::string_view prefix;
    std::string_view uri;

   public:
    /**
     * @brief Construct a new Namespace View Node object
     *
     * @param owner The element this namespace node belongs to
     * @param prefix The namespace prefix (acts as local name)
     * @param uri The namespace URI (acts as string-value)
     */
    NamespaceViewNode(Node* owner, std::string_view prefix,
                      std::string_view uri);

    /**
     * @brief NamespaceViewNode move
     *
     * @param other
     */
    NamespaceViewNode(NamespaceViewNode&& other) noexcept;

    /**
     * @brief Uses the special xml tag name ".namespace-view-node"
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
     * @brief Get the namespace prefix (the node's local name)
     *
     * @return std::string_view
     */
    std::string_view getPrefix() const;

    /**
     * @brief Get the namespace URI (the node's string value)
     *
     * @return std::string_view
     */
    std::string_view getUri() const;

    /**
     * @brief Returns true
     *
     */
    bool isVoid() const override;

    /**
     * @brief Tests equality only against namespace URIs
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