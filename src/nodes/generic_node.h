#pragma once

#include "node.h"

namespace Templater::dynamic::tags {
    
    /**
     * @brief A Node which can is given its tag name and whether it is void or not at construction. Used when concrete classes aren't available/convenient.
     * Incurs a memory size penalty compared to concrete classes.
     * 
     */
    class GenericNode: public Node {
        private:
            /**
             * @brief The tag name of the Node.
             * 
             */
            std::string tag;

            
            /**
             * @brief Whether the Node is void or not.
             * 
             */
            bool _isVoid;
        public: 
            /**
             * @brief Construct a new owning GenericNode.
             * 
             * @tparam Args 
             * @param tagName The tag name of this node
             * @param isVoid Whether this Node is void
             * @param args Forwarded to the Node constructor
             */
            template <typename... Args>
            explicit GenericNode(std::string tagName, bool isVoid, Args&&... args);


            /**
             * @brief Construct an empty owning GenericNode object
             * 
             * @param tagName The tag name of this node
             * @param isVoid Whether this Node is void
             */
            explicit GenericNode(std::string tagName, bool isVoid);


            /**
             * @brief Construct a fully runtime owning GenericNode object
             * 
             * @param tagName The tag name of this node
             * @param isVoid Whether this Node is void
             * @param attributes Attributes to be forwarded to the Node constructor
             * @param children Children to be forwarded to the Node constructor
             */
            explicit GenericNode(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<NodeHandle>&& children);


            /**
             * @brief Construct an empty non-owning GenericNode object
             * 
             * @param tagName The tag name of this node
             * @param isVoid Whether this Node is void
             */
            explicit GenericNode(NonOwningNodeTag, std::string tagName, bool isVoid);


            /**
             * @brief Construct a fully runtime non-owning GenericNode object
             * 
             * @param tagName The tag name of this node
             * @param isVoid Whether this Node is void
             * @param attributes Attributes to be forwarded to the Node constructor
             * @param children Children to be forwarded to the Node constructor
             */
            explicit GenericNode(NonOwningNodeTag, std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<NodeHandle>&& children);


            /**
             * @brief Construct a new GenericNode object from another GenericNode being moved.
             * 
             * @param other 
             */
            explicit GenericNode(Node&& other) noexcept;


            /**
             * @brief GenericNode move assignment.
             * 
             * @param other 
             * @return GenericNode& 
             */
            GenericNode& operator=(Node&& other) noexcept;
            const std::string& getTagName() const override;
            bool isVoid() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };

    
    template <typename... Args>
    GenericNode::GenericNode(std::string tagName, bool isVoid, Args&&... args)
        : tag{std::move(tagName)}, _isVoid{isVoid}, Node(std::forward<Args>(args)...) {

        if (this->isVoid() && this->getChildrenCount() > 0) {
            throw std::runtime_error("Void " + getTagName() + " cannot have children.");
        }
    }
}