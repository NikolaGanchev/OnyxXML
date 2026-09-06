#pragma once

#include <vector>

#include "../index.h"

namespace onyx::dynamic::index {
/**
 * @brief An Index which keeps track of all nodes with a specific tag name,
 * optionally bound by a namespace name. The underlying index structure is an
 * std::vector.
 *
 */
class TagNameIndex : public Node::Index {
   private:
    /**
     * @brief Target namespace URI.
     * An empty std::nullopt indicates any namespace.
     * A string with value "" indicates strictly no namespace.
     */
    std::optional<std::string> namespaceUri;

    /**
     * @brief The tag name to index. Constant from creation.
     *
     */
    std::string tagName;

    /**
     * @brief The data structure that holds all indexed Nodes with the given tag
     * name.
     *
     */
    std::vector<Node*> index;

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
     * @brief Construct a new TagNameIndex object by a given root and tag name
     * with strictly no namespace.
     *
     * @param root
     * @param tagName
     */
    explicit TagNameIndex(Node* root, std::string tagName);

    /**
     * @brief Index tags bound to a specific namespace URI and
     * tag name.
     *
     * @param root
     * @param namespaceUri
     * @param tagName
     */
    explicit TagNameIndex(Node* root, std::string namespaceUri,
                          std::string tagName);

    /**
     * @brief Index tags matching tag name across any namespace.
     *
     * @param root
     * @param tagName
     */
    explicit TagNameIndex(Node* root, AnyNamespaceTag, std::string tagName);

   public:
    /**
     * @brief Get a const copy of the vector of all indexed Nodes which have the
     * given namespace and tag name combination. Returns an empty vector if the
     * Index is invalidated.
     *
     * @return const std::vector<Node*>
     */
    const std::vector<Node*> get();

    ADD_INDEX_MOVE_OPERATIONS(public, TagNameIndex, &TagNameIndex::tagName,
                              &TagNameIndex::namespaceUri,
                              &TagNameIndex::index);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;
};
}  // namespace onyx::dynamic::index