#pragma once

#include "node.h"

namespace Templater::dynamic {
    
    /**
     * @brief A Node which is void by default. Provides constructors that don't accept children. 
     * All void nodes should derive from the VoidNode base instead of Node.
     * 
     */
    class VoidNode: public Node {
        public:
            /**
             * @brief Constructs a Node using the provided rvalue referenced Attributes
             * 
             */
            template <typename... Args>
            explicit VoidNode(Args&&... args) requires (isAttribute<Args>&& ...);


            /**
             * @brief Construct a new VoidNode from the attributes vector.
             * 
             * @param attributes 
             */
            explicit VoidNode(std::vector<Attribute> attributes);

            
            bool isVoid() const override;
    };

    template <typename... Args>
    VoidNode::VoidNode(Args&&... args) requires (isAttribute<Args>&& ...)
        : Node(std::move(args)...) {}
}