#pragma once

#include "index.h"
#include <unordered_map>

namespace Templater::dynamic::index {
    
    /**
     * @brief An Index which indexes all Nodes in a tree by their tag name and provides constant access to a vector of Nodes with any given tag name.
     * The underlying index data structure is an std::unordered_map.
     * 
     */
    class TagIndex: public Index {
        private:
            /**
             * @brief The index storage. 
             * The key is the tag name, while the value is the std::vector of all indexed Nodes which have that tag name.
             */
            std::unordered_map<std::string, std::vector<Node*>> index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;


            /**
             * @brief In the general case, Nodes shouldn't change their tag name after creation.
             * However, in case of peculiar Node implementations, this method is fully implemented.
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
             * @brief Get a const copy of the vector of all indexed Nodes which have the passed to the method tag name.
             * Returns an empty vector if the Index is invalidated.
             * 
             * @param tagName 
             * @return const std::vector<Node*> 
             */
            const std::vector<Node*> getByTagName(const std::string& tagName);
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };
}