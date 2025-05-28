#pragma once

#include <vector>

#include "index.h"

namespace Templater::dynamic::index {
/**
 * @brief An Index which keeps track of all nodes with a specific tag name. The
 * underlying index structure is an std::vector.
 *
 */
class TagNameIndex : public Node::Index {
   private:
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
     * which is copied.
     *
     * @param root
     * @param tagName
     */
    explicit TagNameIndex(Node* root, std::string& tagName);

    /**
     * @brief Construct a new TagNameIndex object by a given root and tag name
     * which is moved.
     *
     * @param root
     * @param tagName
     */
    explicit TagNameIndex(Node* root, std::string&& tagName);

   public:
    /**
     * @brief Get a const copy of the vector of all indexed Nodes which have the
     * given tag name. Returns an empty vector if the Index is invalidated.
     *
     * @return const std::vector<Node*>
     */
    const std::vector<Node*> get();

    ADD_INDEX_MOVE_OPERATIONS(public, TagNameIndex, &TagNameIndex::tagName,
                              &TagNameIndex::index);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;
};
}  // namespace Templater::dynamic::index