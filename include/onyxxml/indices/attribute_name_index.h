#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../index.h"

namespace onyx::dynamic::index {

/**
 * @brief An Index which keeps track of all nodes which have a specific
 * attribute name, optionally bound to a namespace URI and is queried by values
 * of that attribute. The underlying index data structure is an
 * std::unordered_map.
 */
class AttributeNameIndex : public Node::Index {
   private:
    /**
     * @brief Target namespace URI.
     * An empty std::nullopt indicates any namespace.
     * A string with value "" indicates strictly no namespace.
     */
    std::optional<std::string> namespaceUri;

    /**
     * @brief The local attribute name to index. Constant from creation.
     *
     */
    std::string localName;

    /**
     * @brief The index storage.
     * The key is the attribute value is the attribute value, while the value is
     * the std::vector of all indexed Nodes which match.
     */
    std::unordered_map<std::string, std::vector<Node*>> index;

    /**
     * @brief Helper to find a matching attribute value on a node considering
     * namespace rules.
     */
    std::optional<std::string_view> findMatchingAttributeValue(
        Node* node) const;

   protected:
    bool putIfNeeded(Node* node) override;
    bool removeIfNeeded(Node* node) override;
    bool update(Node* node) override;

    /**
     * @brief Construct a new AttributeNameIndex object by a given root and
     * attribute name with strictly no namespace.
     * @param root
     * @param attributeName
     */
    explicit AttributeNameIndex(Node* root, std::string localName);

    /**
     * @brief Index attributes bound to a specific namespace URI and
     * local name.
     *
     * @param root
     * @param namespaceUri
     * @param localName
     */
    explicit AttributeNameIndex(Node* root, std::string namespaceUri,
                                std::string localName);

    /**
     * @brief Index attributes matching local name across any namespace.
     *
     * @param root
     * @param localName
     */
    explicit AttributeNameIndex(Node* root, AnyNamespaceTag,
                                std::string localName);

   public:
    /**
     * @brief Get a const copy of the vector of all indexed Nodes which have the
     * given attribute value. Returns an empty vector if the Index is
     * invalidated.
     *
     * @param value
     * @return const std::vector<Node*>
     */
    const std::vector<Node*> getByValue(const std::string& value);

    ADD_INDEX_MOVE_OPERATIONS(public, AttributeNameIndex,
                              &AttributeNameIndex::namespaceUri,
                              &AttributeNameIndex::localName,
                              &AttributeNameIndex::index);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;
};
}  // namespace onyx::dynamic::index