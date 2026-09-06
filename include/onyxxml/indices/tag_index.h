#pragma once

#include <unordered_map>

#include "../index.h"

namespace onyx::dynamic::index {

/**
 * @brief An Index which indexes all Nodes in a tree by their tag name and
 * namespace URI and provides constant time access to a vector of Nodes with any
 * given tag name and URI. The underlying index data structure is an
 * std::unordered_map.
 *
 */
class TagIndex : public Node::Index {
   private:
    /**
     * @brief The index storage.
     * The key is the uri, while the inner key is the local tag name and the
     * value is the std::vector of all indexed Nodes which have that tag name.
     * Nodes without a namespace use "" as the URI key.
     */
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<Node*>>>
        index;

   protected:
    bool putIfNeeded(Node* node) override;
    bool removeIfNeeded(Node* node) override;

    /**
     * @brief In the general case, Nodes shouldn't change their tag name after
     * creation. However, in case of peculiar Node implementations, this method
     * is fully implemented.
     *
     * @param node
     * @return true
     * @return false
     */
    bool update(Node* node) override;

    /**
     * @brief Construct a new TagIndex object by a given root.
     *
     * @param root
     */
    explicit TagIndex(Node* root);

   public:
    /**
     * @brief Match elements with tagName belonging to no namespace.
     *
     * @param tagName
     */
    const std::vector<Node*> getByTagName(const std::string& tagName);

    /**
     * @brief Match elements strictly matching both namespace URI and local
     * tagName.
     *
     * @param namespaceUri
     * @param tagName
     */
    const std::vector<Node*> getByTagName(const std::string& namespaceUri,
                                          const std::string& tagName);

    /**
     * @brief Match elements with tagName across any namespace.
     *
     * @param tagName
     */
    const std::vector<Node*> getByLocalName(const std::string& tagName);

    /**
     * @brief Match all elements residing within a specific namespace URI.
     *
     * @param namespaceUri
     */
    const std::vector<Node*> getByNamespace(const std::string& namespaceUri);

    ADD_INDEX_MOVE_OPERATIONS(public, TagIndex, &TagIndex::index);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;
};
}  // namespace onyx::dynamic::index