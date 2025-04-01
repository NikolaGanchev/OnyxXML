#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <cstring>

namespace Templater::dynamic {
    class Node;
    namespace index {
        class Index;
    }

    /**
     * @brief Holds an XML Attribute for use in a Node tree
     * 
     */
    class Attribute {
        friend Node;
        private:
            /**
             * @brief The name of the attribute.
             * 
             */
            std::string m_name;
            /**
             * @brief The value of the attribute.
             * 
             */
            std::string m_value;
            /**
             * @brief whether the value should be escaped.
             * 
             */
            bool m_shouldEscape;
            /**
             * @brief Set the Value string
             * 
             */
            void setValue(const std::string&);
            /**
             * @brief Get a mutable refernce to the inner value string
             * 
             * @return std::string& 
             */
            std::string& getValueMutable();
        public:


            /**
             * @brief Construct a new Attribute object.
             * 
             * @param name The name of the attribute
             * @param value The value of the attribute
             * @param shouldEscape Signfies if the value is xml safe; if the value is unsafe, this argument should be true, which is the default value. 
             * shouldEscape is used by consumers of the class to decide whether the value should be escaped and Has no effect on the constructed object's behaviour. 
             */
            explicit Attribute(std::string name, std::string value, bool shouldEscape = true);


            /**
             * @brief Construct a new Attribute object with an empty value. 
             * The constructed object is marked unsafe for its value, meaning shouldEscape() will return true. 
             * This is due to the possibility that the object is later modified. 
             * If an Attribute object is needed which has an empty value and which is marked as safe, 
             * @ref Attribute(std::string, std::string, bool shouldEscape) "Attribute(std::string name, \"\", bool shouldEscape = true)" should be used instead.
             * 
             * @param name  The name of the attribute
             */
            explicit Attribute(std::string name);


            /**
             * @brief Get the name string.
             * 
             * @return const std::string& 
             */
            const std::string& getName() const;


            /**
             * @brief Get the value string
             * 
             * @return const std::string& 
             */
            const std::string& getValue() const;


            /**
             * @brief Get whether the value should be escaped
             * 
             * @return true The value is unsafe and should be escaped
             * @return false The value is safe and should not be escaped
             */
            bool shouldEscape() const;
    };

    namespace dtags {
        class EmptyNode;
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
    concept isValidNodeConstructorTypePtr = std::same_as<T, std::unique_ptr<Node>> || std::same_as<T, Attribute>;


    /**
     * @brief Checks if the templete parameter is a subclass of Node.
     * 
     * @tparam T The type
     */
    template <typename T>
    concept isNode = std::derived_from<T, Node>;


    /**
     * @brief Checks if the templete parameter is an Attribute.
     * 
     * @tparam T The type
     */
    template <typename T>
    concept isAttribute = std::same_as<T, Attribute>;


    /**
     * @brief An abstract class representing an XML Node. Any Node is a tree by itself. Node objects are not aware who its parent is, only if they have one.
     * The Node owns and manages its children. It cannot be copied or copy assigned. Moves are allowed.
     * 
     */
    class Node {
        friend dtags::EmptyNode;
        friend index::Index;
        private:
            /**
             * @brief An observable constant std::string reference. 
             * Behaves as a pointer, but the assignment operator is overriden to invoke a callback on assignment.
             * The string inside cannott be otherwise modified and is always returned as a constant value.
             */
            class ObservableStringRef {
                private:
                    /**
                     * @brief The pointer to the string
                     * 
                     */
                    std::string* m_ptr;
                    /**
                     * @brief A callback which is invoked on reassignment
                     * 
                     */
                    std::function<void()> m_callback;
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
                     * @brief Custom reassingment. Swaps the internal m_ptr for the new one and invokes m_callback
                      * 
                     * @param newPtr The new std::string that m_ptr should point to
                     * @return ObservableStringRef& this
                     */
                    ObservableStringRef& operator=(std::string newPtr);
            };


            /**
             * @brief Invokes the process callback over the given Node and all its children.
             * Parses iteratively
             * 
             * @param object The root node for parsing
             * @param process The process function
             */
            void iterativeProcessor(Node& object, std::function<void(Node*)> process);


            /**
             * @brief Parses over all children of the given Node and constructs a vector including only those that satisfy the condition function.
             * 
             * @param object The root for parsing. Not included in the parse.
             * @param condition A function which returns true if the given child should be included in the return vector and false if not
             * @return std::vector<Node*> 
             */
            std::vector<Node*> iterativeChildrenParse(const Node& object, std::function<bool(Node*)> condition) const;

            
            /**
             * @brief A vector holding the Attributes of this node
             * 
             */
            std::vector<Attribute> m_attributes;

            /**
             * @brief A vector holding unique pointers to the children of this node
             * 
             */
            std::vector<std::unique_ptr<Node>> m_children;


            /**
             * @brief A vector holding raw pointers to the indices of this node
             * 
             */
            std::vector<index::Index*> m_indices;

            
            /**
             * @brief Signifies whether this node is in a tree
             * 
             */
            bool m_isInTree = false;

            
            /**
             * @brief A recursive constructor argument move processor. Takes an argument and delegates to either @ref processConstructorAttribute(Attribute&&) or processConstructorObjectMove(T&&).
             * 
             * @tparam T The type
             * @param arg Object being moved
             */
            template <typename T>
            void processConstructorArgs(T&& arg);


            /**
             * @brief Moves an Attribute to the m_attributes vector.
             * 
             * @param attribute
             */
            void processConstructorAttribute(Attribute&& attribute);

            
            /**
             * @brief Moves a Node to the m_children vector.
             * 
             * @tparam T 
             */
            template <typename T>
            void processConstructorObjectMove(T&& child) requires (isNode<T>);


            /**
             * @brief Removes a child from the given root recursively. 
             * The child is removed from m_children, m_isInTree is set to false and ownership is transferred back to the caller via the return value.
             * 
             * @param childToRemove 
             * @param currentRoot 
             * @return std::unique_ptr<Node> The ownership carrying unique pointer to the removed Node.
             */
            std::unique_ptr<Node> removeChild(Node* childToRemove, Node& currentRoot);


            /**
             * @brief Adds an index.
             * Works by adding the index to the current object's m_indices and then adding it to all children. Calls Index::putIfNeeded() for every Node in the tree.
             * 
             * @param index 
             */
            void addIndex(index::Index* index);


            /**
             * @brief Removes an index from the current object's m_indices and from all its children. Calls Index::removeIfNeeded() for every Node in the tree.
             * 
             * @param index 
             */
            void removeIndex(index::Index* index);


            /**
             * @brief Safely iterates over m_indices
             * 
             * @param callback Runs for every index of the current object
             */
            void indexParse(std::function<void(index::Index*)> callback);


            /**
             * @brief The static global indentation string to be used for indendting during serialisation. Can be overriden by arguments to Node::serialise(const std::string&, bool). 
             * The default value is "\t".
             */
            static std::string indentationSequence;


            /**
             * @brief Static global state on whether attributes should be sorted during serialisation. Can be overriden by arguments to Node::serialise(const std::string&, bool). 
             * The default value is false.
             */
            static bool sortAttributes;
        public:
            /**
             * @brief Construct a new Node object via moved arguments. Allows supplying rvalue references to other Node objects of any subclass and Attributes in any order.
             * Validated at compile time. Internally, for Nodes constructs a unique pointer from the rvalue reference.
             * @tparam Args 
             */
            template <typename... Args>
            explicit Node(Args&&... args) requires (isValidNodeConstructorType<Args>&& ...);


            /**
             * @brief Construct a new Node object at runtime. Enforced order of attributes first, then children. Children need to be given over using unique pointers.
             * 
             * @param attributes A vector of Attributes
             * @param children A vector of children
             */
            explicit Node(std::vector<Attribute> attributes, std::vector<std::unique_ptr<Node>>&& children);
            explicit Node(const Node& other) = delete;
            Node& operator=(const Node& other) = delete;


            /**
             * @brief Construct a new Node object using a Move
             * 
             * @param other 
             */
            explicit Node(Node&& other);


            /**
             * @brief Construct a new Node object with default values
             * 
             */
            explicit Node();


            /**
             * @brief Destroy the Node object. Calls Index::invalidate() on all indices whose root is this object. Sets m_isInTree to false for all children.
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
             * @brief Returns whether the XML tag is void. Void tags are self closing nodes which may have no children.
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
             * @brief Get a copy of the m_children vector for the current node
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
             * @brief Takes ownership of a Node and adds it as a child of the current node. Sets the node's m_isInTree to true. Returns a reference to the node.
             * Throws a runtime error if the current node is void.
             * 
             * @param child 
             * @return Node* 
             */
            Node* addChild(std::unique_ptr<Node> child);


            /**
             * @brief Constructs a unique pointer from the given Node and adds it to m_children.
             * Throws a runtime error if the current node is void.
             * 
             * @tparam T 
             */
            template <typename T>
            Node* addChild(T&& child) requires (isNode<T>);


            /**
             * @brief Calls addChild() using the given unique pointer.
             * 
             * @param right 
             * @return Node& this
             */
            Node& operator+=(std::unique_ptr<Node> right);


            /**
             * @brief Calls addChild() using the given rvalue reference. 
             * 
             * @tparam T 
             */
            template <typename T>
            Node& operator+=(T&& right) requires (isNode<T>);


            /**
             * @brief Currently only checks pointer equality.
             * 
             * @param right 
             * @return true 
             * @return false 
             */
            bool operator==(Node& right);

            /**
             * @brief Removes a child from the current node recursively. Any child in the tree with the current node as root is searched. 
             * The child is removed from m_children, m_isInTree is set to false and ownership is transferred back to the caller via the return value.
             * 
             * @param childToRemove 
             * @return std::unique_ptr<Node>  The ownership carrying unique pointer to the removed Node.
             */
            std::unique_ptr<Node> removeChild(Node* childToRemove);


            /**
             * @brief Returns the count of the nodes in the tree with the current node as the root, counted iteratively.
             * 
             * @return size_t 
             */
            size_t size() const;


            /**
             * @brief Serialises the tree defined by the current node to an XML string.
             * 
             * @param indentationSequence The sequence of characters which is used for indentation
             * @param sortAttributes Whether to sort attributes in tags. 
             * Attribute order doesn't matter for XML validity or rendering and sorting incurs a runtime cost. 
             * In the general case attribute insertion order will be kept by the Node.
             * Attribute sorting is mainly to be used for getting deterministic output for testing.
             * Default value: false.
             * 
             * @return std::string 
             */
            virtual std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const;


            /**
             * @brief Set the static indentation sequence
             * 
             * @param newSequence 
             */
            static void setIndentationSequence(const std::string& newSequence);


            /**
             * @brief Get the static indendtation sequence
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

    namespace dtags {

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
                const std::string m_tag;

                
                /**
                 * @brief Whether the Node is void or not.
                 * 
                 */
                const bool m_isVoid;
            public: 
                /**
                 * @brief Construct a new GenericNode object.
                 * 
                 * @tparam Args 
                 * @param tagName The tag name of this node
                 * @param isVoid Whether this Node is void
                 * @param args Forwarded to the Node constructor
                 */
                template <typename... Args>
                explicit GenericNode(std::string tagName, bool isVoid, Args&&... args);
                /**
                 * @brief Construct an empty GenericNode object
                 * 
                 * @param tagName The tag name of this node
                 * @param isVoid Whether this Node is void
                 */
                explicit GenericNode(std::string tagName, bool isVoid);


                /**
                 * @brief Construct a fully runtime GenericNode object
                 * 
                 * @param tagName The tag name of this node
                 * @param isVoid Whether this Node is void
                 * @param attributes Attributes to be forwarded to the Node constructor
                 * @param children Children to be forwared to the Node constructor
                 */
                explicit GenericNode(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::unique_ptr<Node>>&& children);


                /**
                 * @brief Construct a new GenericNode object from another GenericNode being moved.
                 * 
                 * @param other 
                 */
                explicit GenericNode(Node&& other);
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };


        /**
         * @brief A node which provides no serialisation structure. Used as a root to partial trees with adjacent children. 
         * Useful for generating partial XML fragments.
         * 
         */
        class EmptyNode: public Node {
            public: 
                using Node::Node;
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };


        /**
         * @brief A text Node
         * 
         */
        class Text: public Node {
            private:
                /**
                 * @brief The text
                 * 
                 */
                const std::string m_text;


                /**
                 * @brief Whether unicode sequences should be escaped. Set at construction. Doesn't influence the behaviour of instances of this class.
                 * 
                 */
                const bool m_escapeMultiByte;
            public:
                /**
                 * @brief Construct a new Text object by given text and whether unicode sequences should be escaped. 
                 * Unicode sequences can be unsafe on envirionments that do not support UTF-8 and can be used to create injections. 
                 * Unicode escaping escapes any unicode sequence to an XML/HTML entity. 
                 * Due to the runtime cost and overall low risk this is false by default.
                 * The escaping is not done by this class; it is a suggestion to consumers.
                 * 
                 * @param text 
                 * @param escapeMultiByte 
                 */
                explicit Text(std::string text, bool escapeMultiByte = false); 


                /**
                 * @brief Construct a new Text object by copy
                 * 
                 * @param other 
                 */
                explicit Text(const Text& other); 


                /**
                 * @brief Construct a new Text object by move
                 * 
                 * @param other 
                 */
                explicit Text(Text&& other); 
                bool isVoid() const override;


                /**
                 * @brief Get the text string. The string is returned without any transformations done to it.
                 * 
                 * @return const std::string& 
                 */
                const std::string& getText() const;

                const std::string& getTagName() const override;
                std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const override;
        };
    }

    namespace index {
        /**
         * @brief Indexes are non-owning indexings over a Node. 
         * A node with an Index also holds a non owning pointer to it. 
         * The index, with other words, is solely owned by the creator.
         * Upon destruction of a Node, it invalidates its own indices. 
         * Invalidate indices are to not return any values if queried.
         * At destruction, the index, if valid, removes itself from the root note it is applied to.
         * As a rule, nodes do not query indices. An Index should have methods to return any required info it indexes.
         * The index class is responsible for root keeping, invalidation and destruction. 
         * The index class constructor is protected. Indices are to be created using "creator" functions, which are templated functions for instantiation of indices.
         * To use them, any subclass of Index should in its definition write the macro BEFRIEND_INDEX_CREATOR_FUNCTIONS; to add creators.
         * Subclasses are responsible for providing their own implementations of storage and getters, as well as decide when the Index should be updated.
         * Any index queries are, unless specified otherwise, only valid from the viewpoint of the root Node.
         * 
         */
        class Index {
            friend Node;
            private:
                /**
                 * @brief The Node the Index was initialised with
                 * 
                 */
                Node* m_root;
                /**
                 * @brief Whether the Index is valid or not
                 * 
                 */
                bool m_valid;
            protected:
                /**
                 * @brief Indexes the given Node if it satisfies the conditions for indexing and is not already in the index.
                 * Always called when a child is being added to a Node. 
                 * 
                 * @param object 
                 * @return true The Node was indexed on this call
                 * @return false The Node doesn't need to be indexed or is already in the index.
                 */
                virtual bool putIfNeeded(Node* object) = 0;


                /**
                 * @brief Removes the given Node if it is in the index.
                 * Always called when a child is being removed from a Node, or a Node is being deconstructed. 
                 * 
                 * @param object 
                 * @return true The Node was removed
                 * @return false The Node wasn't removed
                 */
                virtual bool removeIfNeeded(Node* object) = 0;


                /**
                 * @brief Updates the given Node. The Node may be added or removed from the index, or its indexing changed.
                 * Nodes have to call this anytime the node's properties are mutated. The update() function should satisfy the following:
                 * 1. If the given Node is already in the expected position in the index, nothing is done.
                 * 2. Else, update() is equivalent to calling removeIfNeeded() and putIfNeeded(). 
                 * This may or may not change the index, but the update() function can choose to return true in both cases.
                 * 
                 * @param object 
                 * @return true The Node's indexing could have been updated but is not guaranteed to have been
                 * @return false The Node wasn't updated
                 */
                virtual bool update(Node* object) = 0;


                /**
                 * @brief Construct a new Index object. Indices are empty on construction and to index the given root, init() needs to be called.
                 * 
                 * @param root 
                 */
                explicit Index(Node* root);

                
                /**
                 * @brief Invalidates the Index. Invalidated indices do not return anything to queries. What empty value is returned is determined by subclasses.
                 * Querying an invalid index should not cause any runtime errors/exceptions.
                 * 
                 */
                void invalidate();


                /**
                 * @brief Initialises the index. Adds the index to the root Node and its children and fills the internal index with data.
                 * 
                 */
                void init();
            public:
                /**
                 * @brief Get the Root Node.
                 * 
                 * @return const Node* 
                 */
                const Node* getRoot() const;

                /**
                 * @brief Whether the Index is valid. Queries on invalid indices should return empty values, but not cause exceptions.
                 * 
                 * @return true 
                 * @return false 
                 */
                bool isValid() const;


                /**
                 * @brief Destroy the Index object. Removes the index from the root Node and all its children
                 * 
                 */
                virtual ~Index();
        };

        #define BEFRIEND_INDEX_CREATOR_FUNCTIONS template <typename T, typename... Args>\
                                            friend T Templater::dynamic::index::createIndex(Args... args) requires (isIndex<T>);\
                                            template <typename T, typename... Args>\
                                            friend T* Templater::dynamic::index::createIndexPointer(Args... args) requires (isIndex<T>);\
                                            template <typename T, typename... Args>\
                                            friend std::unique_ptr<T> Templater::dynamic::index::createIndexUniquePointer(Args... args) requires (isIndex<T>);\
                                            template <typename T, typename... Args>\
                                            friend std::shared_ptr<T> Templater::dynamic::index::createIndexSharedPointer(Args... args) requires (isIndex<T>);
    }
}

template <typename... Args>
Templater::dynamic::Node::Node(Args&&... args) requires (Templater::dynamic::isValidNodeConstructorType<Args>&& ...)
    : m_attributes{}, m_children{}, m_isInTree{false}, m_indices{} { 
    (processConstructorArgs(std::forward<Args>(args)), ...);
}

template <typename T>
void Templater::dynamic::Node::processConstructorArgs(T&& arg) {
    if constexpr (std::is_base_of_v<Node, std::decay_t<T>>) {
        processConstructorObjectMove(std::move(arg));
    } else {
        processConstructorAttribute(std::move(arg));
    }
}

template <typename T>
Templater::dynamic::Node* Templater::dynamic::Node::addChild(T&& newChild) requires (isNode<T>) {
    if (isVoid()) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
        return nullptr;
    }
    std::unique_ptr<T> obj = std::make_unique<std::decay_t<T>>(std::forward<T>(newChild));
    (dynamic_cast<Node*>(obj.get()))->m_isInTree = true;
    
    this->indexParse([&obj](index::Index* id) -> void {
        (dynamic_cast<Node*>(obj.get()))->addIndex(id);
    });

    Node* objRef = obj.get();

    m_children.push_back(std::move(obj));

    return objRef;
}

template <typename T>
void Templater::dynamic::Node::processConstructorObjectMove(T&& child) requires (isNode<T>) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Object with a child that is already a child of another Object.");
    }

    std::unique_ptr<T> obj = std::make_unique<std::decay_t<T>>(std::forward<T>(child));
    (dynamic_cast<Node*>(obj.get()))->m_isInTree = true;
    
    m_children.push_back(std::move(obj));
}

template <typename T>
Templater::dynamic::Node& Templater::dynamic::Node::operator+=(T&& right) requires (isNode<T>) {
    addChild(std::move(right));
    return (*this);
}

template <typename... Args>
Templater::dynamic::dtags::GenericNode::GenericNode(std::string tagName, bool isVoid, Args&&... args)
    : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Node(std::move(args)...) {
        
    if (this->isVoid() && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
}

template <typename... Args>
Templater::dynamic::VoidNode::VoidNode(Args&&... args) requires (isAttribute<Args>&& ...)
    : Node(std::move(args)...) {}