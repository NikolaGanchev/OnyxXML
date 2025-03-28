#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <cstring>

namespace Templater::dynamic {
    class Object;
    namespace index {
        class Index;
    }

    class Attribute {
        friend Object;
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
        class EmptyTag;
    }

    template <typename T>
    concept isValidObjectConstructorType = std::derived_from<T, Object> || std::same_as<T, Attribute>;
    
    template <typename T>
    concept isValidObjectConstructorTypePtr = std::same_as<T, std::unique_ptr<Object>> || std::same_as<T, Attribute>;

    template <typename T>
    concept isObject = std::derived_from<T, Object>;
    
    template <typename T>
    concept isAttribute = std::same_as<T, Attribute>;

    class Object {
        friend dtags::EmptyTag;
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

            void iterativeProcessor(Object& object, std::function<void(Object*)> process);
            std::vector<Object*> iterativeChildrenParse(const Object& object, std::function<bool(Object*)> condition) const;
            
            std::vector<Attribute> m_attributes;
            std::vector<std::unique_ptr<Object>> m_children;
            std::vector<index::Index*> m_indices;
            bool m_isInTree = false;
            
            template <typename T>
            void processConstructorArgs(T&& arg);
            void processConstructorAttribute(Attribute&& attribute);
            template <typename T>
            void processConstructorObjectMove(T&& child) requires (isObject<T>);
            std::unique_ptr<Object> removeChild(Object* childToRemove, Object& currentRoot);
            void addIndex(index::Index* index);
            void removeIndex(index::Index* index);
            void indexParse(std::function<void(index::Index*)>);
            // Default: "\t"
            static std::string indentationSequence;
            // Default: false
            static bool sortAttributes;
        public:
            template <typename... Args>
            explicit Object(Args&&... args) requires (isValidObjectConstructorType<Args>&& ...);
            explicit Object(std::vector<Attribute> attributes, std::vector<std::unique_ptr<Object>>&& children);
            explicit Object(const Object& other) = delete;
            Object& operator=(const Object& other) = delete;
            explicit Object(Object&& other);
            explicit Object();
            virtual ~Object();

            virtual const std::string& getTagName() const = 0;
            virtual bool isVoid() const = 0;

            bool isInTree() const;
            // Returns a static list of the direct children
            std::vector<Object*> getChildren() const;
            size_t getChildrenCount() const;
            // Returns a static list of all children in the tree that fullfil the condition
            std::vector<Object*> getChildrenByClassName(const std::string& className) const;
            std::vector<Object*> getChildrenByTagName(const std::string& tagName) const;
            std::vector<Object*> getChildrenByName(const std::string& name) const;
            std::vector<Object*> getChildrenById(const std::string& id) const;
            std::vector<Object*> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;
            
            bool hasAttributeValue(const std::string& name) const;
            const std::vector<Attribute>& getAttributes() const;
            const std::string& getAttributeValue(const std::string& name) const;
            void setAttributeValue(const std::string& name, const std::string& newValue);
            ObservableStringRef operator[](const std::string& name);

            Object* addChild(std::unique_ptr<Object> child);
            template <typename T>
            Object* addChild(T&& child) requires (isObject<T>);

            Object& operator+=(std::unique_ptr<Object> right);
            template <typename T>
            Object& operator+=(T&& right) requires (isObject<T>);
            // Checks equality by pointer to InternalObject
            // A && version is not needed, as it would be illogical to compare pointers with a temporary object
            bool operator==(Object& right);

            std::unique_ptr<Object> removeChild(Object* childToRemove);
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
            friend Object;
            private:
                Object* m_root;
                bool m_valid;
            protected:
                virtual bool putIfNeeded(Object* object);
                virtual bool removeIfNeeded(Object* object) = 0;
                virtual bool update(Object* object) = 0;
            public:
                explicit Index(Object* root);
                const Object* getRoot() const;
                bool isValid() const;
                void invalidate();
                void init();
        };
    }
    
    class VoidObject: public Object {
        public:
            template <typename... Args>
            explicit VoidObject(Args&&... args) requires (isAttribute<Args>&& ...);
            explicit VoidObject(std::vector<Attribute> attributes);
            bool isVoid() const override;
    };

    namespace dtags {

        class GenericObject: public Object {
            private:
                const std::string m_tag;
                const bool m_isVoid;
            public: 
                template <typename... Args>
                explicit GenericObject(std::string tagName, bool isVoid, Args&&... args);
                explicit GenericObject(std::string tagName, bool isVoid);
                explicit GenericObject(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::unique_ptr<Object>>&& children);
                explicit GenericObject(Object&& other);
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };

        // Used to create a tree without a root
        class EmptyTag: public Object {
            public: 
                using Object::Object;
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };

        class Text: public Object {
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
Templater::dynamic::Object::Object(Args&&... args) requires (Templater::dynamic::isValidObjectConstructorType<Args>&& ...)
    : m_attributes{}, m_children{}, m_isInTree{false}, m_indices{} { 
    (processConstructorArgs(std::forward<Args>(args)), ...);
}

template <typename T>
void Templater::dynamic::Object::processConstructorArgs(T&& arg) {
    if constexpr (std::is_base_of_v<Object, std::decay_t<T>>) {
        processConstructorObjectMove(std::move(arg));
    } else {
        processConstructorAttribute(std::move(arg));
    }
}

template <typename T>
Templater::dynamic::Object* Templater::dynamic::Object::addChild(T&& newChild) requires (isObject<T>) {
    if (isVoid()) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
        return nullptr;
    }
    std::unique_ptr<T> obj = std::make_unique<std::decay_t<T>>(std::forward<T>(newChild));
    (dynamic_cast<Object*>(obj.get()))->m_isInTree = true;
    
    this->indexParse([&obj](index::Index* id) -> void {
        (dynamic_cast<Object*>(obj.get()))->addIndex(id);
    });

    Object* objRef = obj.get();

    m_children.push_back(std::move(obj));

    return objRef;
}

template <typename T>
void Templater::dynamic::Object::processConstructorObjectMove(T&& child) requires (isObject<T>) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Object with a child that is already a child of another Object.");
    }

    std::unique_ptr<T> obj = std::make_unique<std::decay_t<T>>(std::forward<T>(child));
    (dynamic_cast<Object*>(obj.get()))->m_isInTree = true;
    
    m_children.push_back(std::move(obj));
}

template <typename T>
Templater::dynamic::Object& Templater::dynamic::Object::operator+=(T&& right) requires (isObject<T>) {
    addChild(std::move(right));
    return (*this);
}

template <typename... Args>
Templater::dynamic::dtags::GenericObject::GenericObject(std::string tagName, bool isVoid, Args&&... args)
    : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Object(std::move(args)...) {
        
    if (this->isVoid() && this->getChildrenCount() > 0) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
}

template <typename... Args>
Templater::dynamic::VoidObject::VoidObject(Args&&... args) requires (isAttribute<Args>&& ...)
    : Object(std::move(args)...) {}