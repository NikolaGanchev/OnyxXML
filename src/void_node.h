#pragma once

#include "node.h"

namespace Templater::dynamic {
    
    /**
     * @brief A Node which is void by default. Provides constructors that don't accept children. 
     * All void nodes should derive from the VoidNode base instead of Node.
     * Although VoidNode cannot have children, it can still be non-owning to upkeep tree integrity.
     * 
     */
    class VoidNode: public Node {
        public:
            /**
             * @brief Constructs an owning Node using the provided rvalue referenced Attributes
             * 
             */
            template <typename... Args>
            explicit VoidNode(Args&&... args) requires (isAttribute<Args>&& ...);

            
            /**
             * @brief Constructs a non-owning Node using the provided rvalue referenced Attributes
             * 
             */
            template <typename... Args>
            explicit VoidNode(NonOwningNodeTag, Args&&... args) requires (isAttribute<Args>&& ...);


            /**
             * @brief Construct a new owning VoidNode from the attributes vector.
             * 
             * @param attributes 
             */
            explicit VoidNode(std::vector<Attribute> attributes);


            /**
             * @brief Construct a new non-owning VoidNode from the attributes vector.
             * 
             * @param attributes 
             */
            explicit VoidNode(NonOwningNodeTag, std::vector<Attribute> attributes);

            
            bool isVoid() const override;
    };

    template <typename... Args>
    VoidNode::VoidNode(Args&&... args) requires (isAttribute<Args>&& ...)
        : Node(std::forward<Args>(args)...) {}

    template <typename... Args>
    VoidNode::VoidNode(NonOwningNodeTag, Args&&... args) requires (isAttribute<Args>&& ...)
        : Node(NonOwning, std::forward<Args>(args)...) {}
}