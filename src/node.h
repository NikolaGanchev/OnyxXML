#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <cstring>
#include <stdexcept>
#include "attribute.h"
#include <sstream>
#include "node_handle.h"

namespace Templater::dynamic {
    class Node;
    class NodeHandle;
    namespace index {
        class Index;
    }

    /**
     * @brief Checks if the template parameter is s subclass of Node or an Attribute.
     * 
     * @tparam T The type
     */
    template <typename T>
    concept isValidNodeConstructorType = std::derived_from<T, Node> || std::same_as<T, Attribute>;
    

    /**
     * @brief Checks if the template parameter is a unique pointer of Node or an Attribute.
     * 
     * @tparam T The type
     */
    template <typename T>
    concept isValidNodeConstructorTypePtr = std::same_as<T, std::unique_ptr<Node>> || std::same_as<T, Node*> || std::same_as<T, Attribute>;


    /**
     * @brief Checks if the template parameter is a subclass of Node.
     * 
     * @tparam T The type
     */
    template <typename T>
    concept isNode = std::derived_from<T, Node>;


    /**
     * @brief Checks if the template parameter is an Attribute.
     * 
     * @tparam T The type
     */
    template <typename T>
    concept isAttribute = std::same_as<T, Attribute>;

    /**
     * @brief  Checks if the type T can be used to call Node::addChild(), i.e., if T is a subclass of Node or std::unique_ptr<Node>.
     * 
     * @tparam T 
     */
    template<typename T>
    concept isValidNodeChild = std::same_as<T, std::unique_ptr<Node>> || std::same_as<T, Node*> || std::derived_from<std::decay_t<T>, Node>;
    
    struct NonOwningNodeTag {};
    inline constexpr NonOwningNodeTag NonOwning{};

    /**
     * @brief An abstract class representing an XML Node. Any Node is a tree by itself. Node objects are not aware who its parent is, only if they have one.
     * The Node owns and manages its children by default. By passing the NonOwning tag to constructors, a non-owning Node can be created. 
     * Non-owning and owning nodes cannot be mixed.
     * Nodes cannot be copied or copy assigned. Moves are allowed.
     * 
     */
    class Node {
        public: 
            class Index;
            using Handle = NodeHandle;
        private:
            /**
             * @brief An observable constant std::string reference. 
             * Behaves as a pointer, but the assignment operator is overriden to invoke a callback on assignment.
             * The string inside cannot be otherwise modified and is always returned as a constant value.
             */
            class ObservableStringRef {
                private:
                    /**
                     * @brief The pointer to the string
                     * 
                     */
                    std::string* ptr;
                    /**
                     * @brief A callback which is invoked on reassignment
                     * 
                     */
                    std::function<void()> callback;
                public:
                    /**
                     * @brief Construct a new Observable String Ref object
                     * 
                     * @param ref The std::string pointer
                     * @param callback The callback function for reassignment
                     */
                    ObservableStringRef(std::string* ref, std::function<void()> callback);

                
                    /**
                     * @brief Implicit cast to const std::string*
                     * 
                     * @return const std::string* 
                     */
                    operator const std::string*() const;

                
                    /**
                     * @brief Achieves pointer behaviour by overriding -> to return the underlying std::string
                     * 
                     * @return const std::string* 
                     */


                    const std::string* operator->() const;


                    /**
                     * @brief Achieves pointer behaviour by overriding * to return the underlying std::string
                     * 
                     * @return const std::string* 
                     */
                    const std::string& operator*() const;


                    /**
                     * @brief Compares for equality against the underlying std::string to the argument
                     * 
                     * @param str The std::string to compare for equality against
                     * @return true The strings are equal
                     * @return false The strings are not equal
                     */
                    bool operator==(const std::string& str) const;


                    /**
                     * @brief Compares for non-equality against the underlying std::string to the argument
                     * 
                     * @param str The std::string to compare for non-equality against
                     * @return true The strings are not equal
                     * @return false The strings are equal
                     */
                    bool operator!=(const std::string& str) const;


                    /**
                     * @brief Custom reassignment. Swaps the internal ptr for the new one and invokes callback
                      * 
                     * @param newPtr The new std::string that ptr should point to
                     * @return ObservableStringRef& this
                     */
                    ObservableStringRef& operator=(std::string newPtr);
            };


            /**
             * @brief Invokes the process callback over this and all its children.
             * Parses iteratively
             * 
             * @param process The process function
             */
            void iterativeProcessor(const std::function<void(Node*)>& process);


            /**
             * @brief Parses over all children of this and constructs a vector including only those that satisfy the condition function.
             * 
             * @param condition A function which returns true if the given child should be included in the return vector and false if not
             * @return std::vector<Node*> 
             */
            std::vector<Node*> iterativeChildrenParse(const std::function<bool(Node*)>& condition) const;

            
            /**
             * @brief A vector holding the Attributes of this node
             * 
             */
            std::vector<Attribute> attributes;

            /**
             * @brief A vector holding unique pointers to the children of this node
             * 
             */
            std::vector<Node*> children;


            /**
             * @brief A vector holding raw pointers to the indices of this node
             * 
             */
            std::vector<Node::Index*> indices;


            /**
             * @brief The parent of this Node. nullptr if no parent exists.
             * 
             */
            Node* parent;


            /**
             * @brief Whether this Node is owning. An owning Node must release the memory of its children upon destruction.
             * Owning Nodes can only contain owning children. Non-owning Nodes can only contain non-owning children.
             * 
             */
            bool _isOwning;
            

            /**
             * @brief A recursive constructor argument move processor. Takes an argument and delegates to either @ref processConstructorAttribute(Attribute&&) or processConstructorObjectMove(T&&).
             * 
             * @tparam T The type
             * @param arg Object being moved
             */
            template <typename T>
            void processConstructorArgs(T&& arg);


            /**
             * @brief Moves an Attribute to the attributes vector.
             * 
             * @param attribute
             */
            void processConstructorAttribute(Attribute&& attribute);

            
            /**
             * @brief Moves a Node to the children vector.
             * 
             * @tparam T 
             */
            template <typename T>
            void processConstructorObjectMove(T&& child) requires (isNode<T>);


            /**
             * @brief Adds an index.
             * Works by adding the index to the current object's indices and then adding it to all children. Calls Index::putIfNeeded() for every Node in the tree.
             * 
             * @param index 
             */
            void addIndex(Node::Index* index);

            /**
             * @brief Removes an index from the current object's indices and from all its children. Calls Index::removeIfNeeded() for every Node in the tree.
             * 
             * @param index 
             */
            void removeIndex(Node::Index* index);

            /**
             * @brief Replaces the address of of one index with another without changing any of the index contents. Internal use only.
             * 
             * @param oldIndex
             * @param newIndex
             */
            void replaceIndex(Node::Index* oldIndex, Node::Index* newIndex);

            /**
             * @brief Safely iterates over indices
             * 
             * @param callback Runs for every index of the current object
             */
            void indexParse(const std::function<void(Node::Index*)>& callback);


            /**
             * @brief The static global indentation string to be used for indenting during serialization. 
             * The default value is "\t".
             */
            static std::string indentationSequence;


            /**
             * @brief Static global state on whether attributes should be sorted during serialization. 
             * The default value is false.
             */
            static bool sortAttributes;

            
            /**
             * @brief Destroys this Node.
             * 
             */
            void destroy();


            /**
             * @brief Takes over the indices of the Node other by changing their roots. Expects that the indices are already moved to this.
             * 
             */
            void takeOverIndices(Node& other);

        protected:
            /**
             * @brief Get a reference to the live children std::vector
             * 
             * @return const std::vector<Node*>& 
             */
            const std::vector<Node*>& getChildrenLive() const;
        public:
            /**
             * @brief Used during serialization for keeping track of nodes.
             * 
             */
            struct SerializationNode {
                /**
                 * @brief The pointer to the node.
                 * 
                 */
                const Node* obj;


                /**
                 * @brief Whether the node has already been passed once by serialization.
                 * 
                 */
                bool visited;
            };

            /**
             * @brief Methods for special serialization on a Node. Any node subclass returning true on hasSpecialSerialization() must also derive this class.
             * 
             */
            class SpecialSerializable {
                friend Node;
                protected:
                    /**
                     * @brief Called during serialization of a tree for nodes which return true to hasSpecialSerialization(). 
                     * Temporarily takes over control of the serialization. Must do all serialization actions. Has no obligation towards indentation or formatting, 
                     * as long as only valid XML is sent to the result stream. Called after SerializationNode::visited is set to true and and if not 
                     * popped from the stack, when it is encountered again.
                     * 
                     * @param stack The current serialization stack. The top node is the node upon which the method is called. It may choose when and if to pop itself from the stack.
                     * @param result The current result stream. 
                     */
                    virtual void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const = 0;


                    /**
                     * @brief Called during serialization of a tree for nodes which return true to hasSpecialSerialization(). 
                     * Temporarily takes over control of the serialization. Must do all serialization actions. Must pretty format any XML sent
                     * to the output stream. Called after SerializationNode::visited is set to true and and if not 
                     * popped from the stack, when it is encountered again.
                     * 
                     * @param stack The current serialization stack. The top node is the node upon which the method is called. It may choose when and if to pop itself from the stack.
                     * @param result The current result stream. It is guaranteed the result stream will be on the beginning of a newline when received and must be at the beginning of a newline when execution ends. 
                     * @param indentation The current indentation string. The initial state of this variable will be the correct depth-based indentation string of the current node.
                     * @param indentationSequence The indentation string.
                     * @param sortAttributes Whether attributes should be sorted.
                     */
                    virtual void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const = 0;


                    /**
                     * @brief Destroy the Special Serializable object
                     * 
                     */
                    virtual ~SpecialSerializable() {};
            };

            /**
             * @brief Construct a new owning Node object via moved arguments. Allows supplying rvalue references to other Node objects of any subclass and Attributes in any order.
             * Validated at compile time. Internally, for Nodes constructs a pointer from the rvalue reference.
             * @tparam Args 
             */
            template <typename... Args>
            explicit Node(Args&&... args) requires (isValidNodeConstructorType<Args>&& ...);


            /**
             * @brief Construct a new owning Node object at runtime. Enforced order of attributes first, then children. Children need to be given over using unique pointers.
             * 
             * @param attributes A vector of Attributes
             * @param children A vector of children
             */
            explicit Node(std::vector<Attribute> attributes, std::vector<NodeHandle>&& children);


            /**
             * @brief Construct a new non-owning Node object at runtime. Enforced order of attributes first, then children. Children need to be given over using unique pointers.
             * 
             * @param attributes A vector of Attributes
             * @param children A vector of children
             */
            explicit Node(NonOwningNodeTag, std::vector<Attribute> attributes, std::vector<NodeHandle>&& children);


            explicit Node(const Node& other) = delete;
            Node& operator=(const Node& other) = delete;


            /**
             * @brief Construct a new Node object using a move.
             * 
             * @param other 
             */
            explicit Node(Node&& other) noexcept;


            /**
             * @brief Node move assignment.
             * 
             * @param other 
             * @return Node& 
             */
            Node& operator=(Node&& other) noexcept;


            /**
             * @brief Construct a new owning Node object with default values
             * 
             */
            explicit Node();

            /**
             * @brief Construct a new non-owning Node object with default values
             * 
             */
            explicit Node(NonOwningNodeTag);


            /**
             * @brief Destroy the Node object. Calls Index::invalidate() on all indices whose root is this object. Sets isInTree to false for all children.
             * 
             */
            virtual ~Node();


            /**
             * @brief Get the tag name string
             * 
             * @return const std::string& 
             */
            virtual const std::string& getTagName() const = 0;


            /**
             * @brief Returns whether the XML tag is void. Void tags are self closing nodes which may not have children.
             * 
             * @return true The tag is void
             * @return false The tag is not void
             */
            virtual bool isVoid() const = 0;


            /**
             * @brief Returns whether the current Node is in a tree
             * 
             * @return true
             * @return false 
             */
            bool isInTree() const;


            /**
             * @brief Whether the Node is owning.
             * 
             * @return true 
             * @return false 
             */
            bool isOwning() const;


            /**
             * @brief Get the Parent Node. Will be nullptr if the Node has no parent.
             * 
             * @return Node* 
             */
            Node* getParentNode() const;
            

            /**
             * @brief Get a copy of the children vector for the current node
             * 
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildren() const;


            /**
             * @brief Get the children count of the current node. Only counts immediate children.
             * 
             * @return size_t The count of the children.
             */
            size_t getChildrenCount() const;


            /**
             * @brief Get all children that have the given "class" attribute value. Searches all children in the tree.
             * 
             * @param className
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildrenByClassName(const std::string& className) const;


            /**
             * @brief Get all children that have the given tag name. Searches all children in the tree.
             * 
             * @param tagName
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildrenByTagName(const std::string& tagName) const;


            /**
             * @brief Get all children that have the given "name" attribute value. Searches all children in the tree.
             * 
             * @param name
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildrenByName(const std::string& name) const;


            /**
             * @brief Get all children that have the given "id" attribute value. Searches all children in the tree.
             * 
             * @param id
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildrenById(const std::string& id) const;


            /**
             * @brief Get all children that have the given attribute name-value pair. Searches all children in the tree.
             * 
             * @param attribute The attribute name
             * @param value The attribute value
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;


            /**
             * @brief Get all children that have the given attribute name. Searches all children in the tree.
             * 
             * @param attribute The attribute name
             * @return std::vector<Node*> 
             */
            std::vector<Node*> getChildrenByAttributeName(const std::string& attribute) const;
            

            /**
             * @brief Get if the current instance has the given attribute.
             * 
             * @param name 
             * @return true
             * @return false 
             */
            bool hasAttribute(const std::string& name) const;


            /**
             * @brief Get the Attributes vector. Returns a const live reference to the inner Attribute vector of the current Node.
             * 
             * @return const std::vector<Attribute>& 
             */
            const std::vector<Attribute>& getAttributes() const;


            /**
             * @brief Get the attribute value by the given name. Throws a runtime error if the attribute name doesn't exist on this node.
             * 
             * @param name 
             * @return const std::string& 
             */
            const std::string& getAttributeValue(const std::string& name) const;


            /**
             * @brief Set the attribute value by name. If the attribute doesn't exist on the current node, it will be added.
             * 
             * @param name 
             * @param newValue 
             */
            void setAttributeValue(const std::string& name, const std::string& newValue);


            /**
             * @brief Removes the attribute from the current node.
             * 
             * @param name 
             */
            void removeAttribute(const std::string& name);


            /**
             * @brief Returns a reassignable constant string reference to the value of the Attribute with the given name. 
             * If no such Attribute exists, it is initialised with the empty string "" and a reassignable constant string reference to it is returned.
             * 
             * @param name 
             * @return ObservableStringRef Behaves as an std::string pointer that can be reassigned, but cannot be modified by std::string method such std::string::append().=
             */
            ObservableStringRef operator[](const std::string& name);


            /**
             * @brief Adds a Node as a child of the current node. Sets the node's isInTree to true. Returns a reference to the node.
             * Throws a runtime error if the current node is void.
             * Throws a runtime error if the owning mode of this isn't the same as that of NodeHandle.
             * 
             * @param child 
             * @return Node* 
             */
            Node* addChild(NodeHandle child);

            /**
             * @brief Takes ownership of a Node and adds it as a child of the current node. Sets the node's isInTree to true. Returns a reference to the node.
             * Throws a runtime error if the current node is void.
             * Throws a runtime error if this is in non-owning mode.
             * 
             * @param child 
             * @return Node* 
             */
            Node* addChild(std::unique_ptr<Node> child);

            
            /**
             * @brief Adds a Node as a child of the current node. Sets the node's isInTree to true. Returns a reference to the node.
             * Throws a runtime error if the current node is void.
             * Throws a runtime error if this is in owning mode.
             * 
             * @param child 
             * @return Node* 
             */
            Node* addChild(Node* child);
        

            /**
             * @brief Constructs a unique pointer from the given Node and adds it to children.
             * Throws a runtime error if the current node is void.
             * 
             * @tparam T 
             * @param child
             */
            template <typename T>
            Node* addChild(T&& child) requires (isNode<T>);


            /**
             * @brief Replace the given child with a different child. Returns the removed child.
             * 
             * @tparam T 
             * @param childToReplace
             * @param newChild
             */
            template <typename T>
            NodeHandle replaceChild(Node* childToReplace, T&& newChild) requires (isValidNodeChild<T>);


            /**
             * @brief Calls addChild() using the given pointer.
             * 
             * @param right 
             * @return Node& this
             */
            Node& operator+=(NodeHandle right);


            /**
             * @brief Calls addChild() using the given rvalue reference. 
             * 
             * @tparam T 
             */
            template <typename T>
            Node& operator+=(T&& right) requires (isNode<T>);

            bool operator==(Node& right) = delete;

            /**
             * @brief Removes a child from the current node iteratively. Any child in the tree with the current node as root is searched. 
             * If the child is found, is is removed from its parent, isInTree is set to false and ownership is transferred back to the caller via the return value.
             * Returns nullptr if the child is not found.
             * 
             * @param childToRemove 
             * @return NodeHandle The ownership carrying handle to the removed Node. May be nullptr if the child isn't found.
             */
            NodeHandle removeChild(Node* childToRemove);


            /**
             * @brief Returns the count of the nodes in the tree with the current node as the root, counted iteratively.
             * 
             * @return size_t 
             */
            size_t size() const;


            /**
             * @brief Returns the max depth of the tree, calculated iteratively.
             * 
             * @return size_t 
             */
            size_t depth() const;


            /**
             * @brief Returns the leaf count of the tree. Leaves are nodes which have no children.
             * 
             * @return size_t 
             */
            size_t leafCount() const;
            

            /**
             * @brief Serializes the tree defined by the current node to an XML string
             * 
             * @return std::string 
             */
            virtual std::string serialize() const;


            /**
             * @brief Serializes the tree defined by the current node to an XML string with pretty printing.
             * Caution: is slower than serialize(). Performance loss for deep trees and trees with heavily varying depth has been observed to be about 30%.
             * 
             * @param indentationSequence The sequence of characters which is used for indentation
             * @param sortAttributes Whether to sort attributes in tags. 
             * Attribute order doesn't matter for XML validity or rendering and sorting incurs a runtime cost. 
             * In the general case attribute insertion order will be kept by the Node.
             * Attribute sorting is mainly to be used for getting deterministic output for testing.
             * 
             * @return std::string 
             */
            virtual std::string serializePretty(const std::string& indentationSequence, bool sortAttributes) const;


            /**
             * @brief Denotes whether a node has special serialization or not. If a node has special serialization, it must also derive from SpecialSerializable.
             * The corresponding SpecialSerializable methods will be called during serialization.
             * 
             * @return true The node has special serialization 
             * @return false The node does not have special serialization 
             */
            virtual bool hasSpecialSerialization() const;


            /**
             * @brief Creates a shallow copy. A shallow copy copies this node's attributes and other data members. However, tree status, children and indices aren't copied.
             * 
             * @return std::unique_ptr<Node> 
             */
            virtual std::unique_ptr<Node> shallowCopy() const = 0;

            /**
             * @brief Creates a deep copy. A deep copy copies this node's attributes, children and other data members, but not indices. 
             * Caution: expensive operation.
             * 
             * @return std::unique_ptr<Node> 
             */
            virtual std::unique_ptr<Node> deepCopy() const;


            /**
             * @brief Checks if the attributes and other data members of the two nodes are equal. Does not compare children (except for count), nor indices.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            virtual bool shallowEquals(const Node& other) const;


            /**
             * @brief Checks if the attributes, children and data members of the two nodes are equal. Does not compare indices, nor parents.
             * 
             * @param other 
             * @return true 
             * @return false 
             */
            bool deepEquals(const Node& other) const;


            /**
             * @brief Set the static indentation sequence
             * 
             * @param newSequence 
             */
            static void setIndentationSequence(const std::string& newSequence);


            /**
             * @brief Get the static indentation sequence
             * 
             * @return const std::string& 
             */
            static const std::string& getIndentationSequence();


            /**
             * @brief Set the static sortAttributes boolean
             * 
             * @param shouldSort 
             */
            static void setSortAttributes(bool shouldSort);


            /**
             * @brief Get the static sortAttributes boolean
             * 
             * @return true 
             * @return false 
             */
            static bool getSortAttributes();
    };
}


template <typename... Args>
Templater::dynamic::Node::Node(Args&&... args) requires (Templater::dynamic::isValidNodeConstructorType<Args>&& ...)
    : attributes{}, children{}, parent{nullptr}, indices{}, _isOwning(true) { 
    (processConstructorArgs(std::forward<Args>(args)), ...);
}

template <typename T>
void Templater::dynamic::Node::processConstructorArgs(T&& arg) {
    if constexpr (std::is_base_of_v<Node, std::decay_t<T>>) {
        processConstructorObjectMove(std::forward<T>(arg));
    } else {
        processConstructorAttribute(std::forward<T>(arg));
    }
}

template <typename T>
Templater::dynamic::Node* Templater::dynamic::Node::addChild(T&& newChild) requires (isNode<T>) {
    if (isVoid()) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
    }
    if (newChild._isOwning != this->_isOwning) {
        throw std::runtime_error("Attempted to add child to " + getTagName() + " with different owning mode.");
    }

    T* obj = new std::decay_t<T>(std::forward<T>(newChild));
    (dynamic_cast<Node*>(obj))->parent = this;
    
    this->indexParse([&obj](Node::Index* id) -> void {
        (dynamic_cast<Node*>(obj))->addIndex(id);
    });

    children.push_back(obj);

    return obj;
}

template <typename T>
void Templater::dynamic::Node::processConstructorObjectMove(T&& child) requires (isNode<T>) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Node with a child that is already a child of another Node.");
    }
    if (child._isOwning != this->_isOwning) {
        throw std::runtime_error("Attempted to add child to Node with different owning mode.");
    }

    T* obj = new std::decay_t<T>(std::forward<T>(child));
    (dynamic_cast<Node*>(obj))->parent = this;
    
    children.push_back(obj);
}

template <typename T>
Templater::dynamic::Node& Templater::dynamic::Node::operator+=(T&& right) requires (isNode<T>) {
    addChild(std::forward<T>(right));
    return (*this);
}

template <typename T>
Templater::dynamic::NodeHandle Templater::dynamic::Node::replaceChild(Node* childToReplace, T&& newChild) requires (isValidNodeChild<T>) {
    // TODO: Check if childToReplace is in the tree
    if (!childToReplace->isInTree()) {
        throw std::invalid_argument("Child to replace has no parent");
    }

    childToReplace->parent->addChild(std::move(newChild));
    return childToReplace->parent->removeChild(childToReplace);
}