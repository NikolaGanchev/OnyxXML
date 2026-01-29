#pragma once

#include "../node.h"

namespace onyx::dynamic::xpath {

/**
 * @brief A non-owning view over a node. Used for XPath.
 * There are no special protections against including it in general trees or
 * adding children, but it is unintended behaviour.
 *
 */
class RootViewNode : public Node {
   public:
    /**
     * @brief Construct a new Root View Node object
     *
     * @param owner
     */
    RootViewNode(Node* child);

    /**
     * @brief Uses the special xml tag name ""
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
     * @brief Returns false
     *
     */
    bool isVoid() const override;

    XPathType getXPathType() const override;
};
}  // namespace onyx::dynamic::xpath