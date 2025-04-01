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

    class Attribute {
        friend Node;
        private:
            std::string m_name;
            std::string m_value;
            bool m_shouldEscape;
            void setValue(const std::string&);
            std::string& getValueMutable();
        public:
            explicit Attribute(std::string name, std::string value, bool shouldEscape = true);
            explicit Attribute(std::string name);
            const std::string& getName() const;
            const std::string& getValue() const;
            bool shouldEscape() const;
    };

    namespace dtags {
        class EmptyNode;
    }

    template <typename T>
    concept isValidNodeConstructorType = std::derived_from<T, Node> || std::same_as<T, Attribute>;
    
    template <typename T>
    concept isValidNodeConstructorTypePtr = std::same_as<T, std::unique_ptr<Node>> || std::same_as<T, Attribute>;

    template <typename T>
    concept isNode = std::derived_from<T, Node>;
    
    template <typename T>
    concept isAttribute = std::same_as<T, Attribute>;

    class Node {
        friend dtags::EmptyNode;
        friend index::Index;
        private:
            class ObservableStringRef {
                private:
                    std::string* m_ptr;
                    std::function<void()> m_callback;
                
                public:
                    ObservableStringRef(std::string* ref, std::function<void()> callback);
                
                    operator const std::string*() const;
                
                    const std::string* operator->() const;
                    const std::string& operator*() const;
                    bool operator==(const std::string& str) const;
                
                    ObservableStringRef& operator=(std::string newPtr);
            };

            void iterativeProcessor(Node& object, std::function<void(Node*)> process);
            std::vector<Node*> iterativeChildrenParse(const Node& object, std::function<bool(Node*)> condition) const;
            
            std::vector<Attribute> m_attributes;
            std::vector<std::unique_ptr<Node>> m_children;
            std::vector<index::Index*> m_indices;
            bool m_isInTree = false;
            
            template <typename T>
            void processConstructorArgs(T&& arg);
            void processConstructorAttribute(Attribute&& attribute);
            template <typename T>
            void processConstructorObjectMove(T&& child) requires (isNode<T>);
            std::unique_ptr<Node> removeChild(Node* childToRemove, Node& currentRoot);
            void addIndex(index::Index* index);
            void removeIndex(index::Index* index);
            void indexParse(std::function<void(index::Index*)>);
            // Default: "\t"
            static std::string indentationSequence;
            // Default: false
            static bool sortAttributes;
        public:
            template <typename... Args>
            explicit Node(Args&&... args) requires (isValidNodeConstructorType<Args>&& ...);
            explicit Node(std::vector<Attribute> attributes, std::vector<std::unique_ptr<Node>>&& children);
            explicit Node(const Node& other) = delete;
            Node& operator=(const Node& other) = delete;
            explicit Node(Node&& other);
            explicit Node();
            virtual ~Node();

            virtual const std::string& getTagName() const = 0;
            virtual bool isVoid() const = 0;

            bool isInTree() const;
            // Returns a static list of the direct children
            std::vector<Node*> getChildren() const;
            size_t getChildrenCount() const;
            // Returns a static list of all children in the tree that fullfil the condition
            std::vector<Node*> getChildrenByClassName(const std::string& className) const;
            std::vector<Node*> getChildrenByTagName(const std::string& tagName) const;
            std::vector<Node*> getChildrenByName(const std::string& name) const;
            std::vector<Node*> getChildrenById(const std::string& id) const;
            std::vector<Node*> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;
            
            bool hasAttribute(const std::string& name) const;
            const std::vector<Attribute>& getAttributes() const;
            const std::string& getAttributeValue(const std::string& name) const;
            void setAttributeValue(const std::string& name, const std::string& newValue);
            void removeAttribute(const std::string& name);
            ObservableStringRef operator[](const std::string& name);

            Node* addChild(std::unique_ptr<Node> child);
            template <typename T>
            Node* addChild(T&& child) requires (isNode<T>);

            Node& operator+=(std::unique_ptr<Node> right);
            template <typename T>
            Node& operator+=(T&& right) requires (isNode<T>);
            // Checks equality by pointer to InternalObject
            // A && version is not needed, as it would be illogical to compare pointers with a temporary object
            bool operator==(Node& right);

            std::unique_ptr<Node> removeChild(Node* childToRemove);
            size_t size() const;
            
            virtual std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const;

            static void setIndentationSequence(const std::string& newSequence);
            static const std::string& getIndentationSequence();
            static void setSortAttributes(bool shouldSort);
            static bool getSortAttributes();
    };

    namespace index {
        // Must be created using an std::shared_ptr
        class Index {
            friend Node;
            private:
                Node* m_root;
                bool m_valid;
            protected:
                // Returns true if the object was inserted; else returns false
                virtual bool putIfNeeded(Node* object);
                // Returns true if the object was removed; else returns false
                virtual bool removeIfNeeded(Node* object) = 0;
                // Returns true if the object was updated; else returns false
                virtual bool update(Node* object) = 0;
                explicit Index(Node* root);
                void invalidate();
                void init();
            public:
                const Node* getRoot() const;
                bool isValid() const;
                virtual ~Index();
        };
    }
    
    class VoidNode: public Node {
        public:
            template <typename... Args>
            explicit VoidNode(Args&&... args) requires (isAttribute<Args>&& ...);
            explicit VoidNode(std::vector<Attribute> attributes);
            bool isVoid() const override;
    };

    namespace dtags {

        class GenericNode: public Node {
            private:
                const std::string m_tag;
                const bool m_isVoid;
            public: 
                template <typename... Args>
                explicit GenericNode(std::string tagName, bool isVoid, Args&&... args);
                explicit GenericNode(std::string tagName, bool isVoid);
                explicit GenericNode(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::unique_ptr<Node>>&& children);
                explicit GenericNode(Node&& other);
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };

        // Used to create a tree without a root
        class EmptyNode: public Node {
            public: 
                using Node::Node;
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };

        class Text: public Node {
            private:
                const std::string m_text;
                const bool m_escapeMultiByte;
            public:
                explicit Text(std::string text, bool escapeMultiByte = false); 
                explicit Text(const Text& other); 
                explicit Text(Text&& other); 
                bool isVoid() const override;
                const std::string& getText() const;
                const std::string& getTagName() const override;
                std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const override;
        };
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