#pragma once

#include "node.h"

namespace Templater::dynamic::dtags {
    
    /**
     * @brief A node which provides no serialization structure. Used as a root to partial trees with adjacent children. 
     * Useful for generating partial XML fragments.
     * 
     */
    class EmptyNode: public Node {
        public: 
            using Node::Node;


            /**
             * @brief The tag name of an empty node is the invalid xml tag name ".empty". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            bool isVoid() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
}