#pragma once

#include <unordered_map>

#include "../index.h"

namespace onyx::dynamic::index {

/**
 * @brief An Index which keeps track of all nodes which have specific attribute
 * name and is queried by values of that attribute. The underlying index data
 * structure is an std::unordered_map.
 *
 */
class AttributeNameIndex : public Node::Index {
   private:
    /**
     * @brief The attribute name to index. Constant from creation.
     *
     */
    std::string attributeName;

    /**
     * @brief The index storage.
     * The key is the attribute value, while the value is the std::vector of all
     * indexed Nodes which have that attribute value.
     *
     */
    std::unordered_map<std::string, std::vector<Node*>> index;

   protected:
    bool putIfNeeded(Node* node) override;
    bool removeIfNeeded(Node* node) override;
    bool update(Node* node) override;

    /**
     * @brief Construct a new AttributeNameIndex object by a given root and
     * attribute name which is copied.
     *
     * @param root
     * @param attributeName
     */
    explicit AttributeNameIndex(Node* root, std::string& attributeName);

    /**
     * @brief Construct a new AttributeNameIndex object by a given root and
     * attribute name which is moved.
     *
     * @param root
     * @param attributeName
     */
    explicit AttributeNameIndex(Node* root, std::string&& attributeName);

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
                              &AttributeNameIndex::attributeName,
                              &AttributeNameIndex::index);

    BEFRIEND_INDEX_CREATOR_FUNCTIONS;
};
}  // namespace onyx::dynamic::index