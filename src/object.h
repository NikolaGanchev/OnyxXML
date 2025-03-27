#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <cstring>

namespace Templater::dynamic {
    class Object;
    class Index;

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
    concept isValidObjectConstructorTypePtr = std::same_as<T, std::shared_ptr<Object>> || std::same_as<T, Attribute>;

    template <typename T>
    concept isObject = std::derived_from<T, Object>;
    
    template <typename T>
    concept isAttribute = std::same_as<T, Attribute>;

    class Object {
        friend dtags::EmptyTag;
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

            void iterativeChildrenProcessor(Object& object, std::function<void(std::shared_ptr<Object>)> process);
            std::vector<std::shared_ptr<Object>> iterativeChildrenParse(const Object& object, std::function<bool(std::shared_ptr<Object>)> condition) const;
            
            std::vector<Attribute> m_attributes;
            std::vector<std::shared_ptr<Object>> m_children;
            std::vector<std::shared_ptr<Index>> indeces;
            bool m_isInTree = false;
            
            template <typename T>
            void processConstructorArgs(T&& arg);
            void processConstructorAttribute(Attribute&& attribute);
            template <typename T>
            void processConstructorObjectMove(T&& child) requires (isObject<T>);
            template <typename T>
            void processConstructorArgs(T& arg);
            void processConstructorObject(std::shared_ptr<Object>& child);
            void processConstructorAttribute(const Attribute& attribute);
            bool removeChild(Object& childToRemove, Object& currentRoot);
            // Default: "\t"
            static std::string indentationSequence;
            // Default: false
            static bool sortAttributes;
        public:
            template <typename... Args>
            explicit Object(Args&&... args) requires (isValidObjectConstructorType<Args>&& ...);
            explicit Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            explicit Object(const Object& other) = delete;
            Object& operator=(const Object& other) = delete;
            explicit Object(Object&& other);
            explicit Object();
            virtual ~Object();

            virtual const std::string& getTagName() const = 0;
            virtual bool isVoid() const = 0;

            bool isInTree() const;
            // Returns a static list of the direct children
            const std::vector<std::shared_ptr<Object>> getChildren() const;
            size_t getChildrenCount() const;
            // Returns a static list of all children in the tree that fullfil the condition
            std::vector<std::shared_ptr<Object>> getChildrenByClassName(const std::string& className) const;
            std::vector<std::shared_ptr<Object>> getChildrenByTagName(const std::string& tagName) const;
            std::vector<std::shared_ptr<Object>> getChildrenByName(const std::string& name) const;
            std::vector<std::shared_ptr<Object>> getChildrenById(const std::string& id) const;
            std::vector<std::shared_ptr<Object>> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;
            
            bool hasAttributeValue(const std::string& name) const;
            const std::vector<Attribute>& getAttributes() const;
            const std::string& getAttributeValue(const std::string& name) const;
            void setAttributeValue(const std::string& name, const std::string& newValue);
            ObservableStringRef operator[](const std::string& name);

            void addChild(std::shared_ptr<Object> child);
            template <typename T>
            void addChild(T&& child) requires (isObject<T>);

            Object& operator+=(std::shared_ptr<Object>& right);
            template <typename T>
            Object& operator+=(T&& right) requires (isObject<T>);
            // Checks equality by pointer to InternalObject
            // A && version is not needed, as it would be illogical to compare pointers with a temporary object
            bool operator==(Object& right);

            void addIndex(std::shared_ptr<Index> index);
            void removeIndex(std::shared_ptr<Index> index);

            bool removeChild(Object& childToRemove);
            bool removeChild(const std::shared_ptr<Object>& childToRemove);
            size_t size() const;
            
            virtual std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const;

            static void setIndentationSequence(const std::string& newSequence);
            static const std::string& getIndentationSequence();
            static void setSortAttributes(bool shouldSort);
            static bool getSortAttributes();
    };

    class Index {
        public:
            virtual bool putIfNeeded(const Object& obj) = 0;
            virtual bool removeIfNeeded(const Object& obj) = 0;
            virtual bool update(const Object& obj) = 0;
    };
    
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
                explicit GenericObject(std::string tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
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
    : m_attributes{}, m_children{}, m_isInTree{false}, indeces{} { 
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
void Templater::dynamic::Object::addChild(T&& newChild) requires (isObject<T>) {
    if (isVoid()) {
        throw std::runtime_error("Void " + getTagName() + " cannot have children.");
    }
    if (newChild.isInTree()) {
        throw std::runtime_error("Attempted to add child to " + getTagName() + "  that is already a child of another Object.");
        return;
    }
    std::shared_ptr<T> obj = std::make_shared<std::decay_t<T>>(std::forward<T>(newChild));
    (dynamic_cast<Object*>(obj.get()))->m_isInTree = true;
    for (auto& index: indeces) {
        (dynamic_cast<Object*>(obj.get()))->addIndex(index);
    }
    m_children.push_back(obj);
}

template <typename T>
void Templater::dynamic::Object::processConstructorObjectMove(T&& child) requires (isObject<T>) {
    if (child.isInTree()) {
        throw std::runtime_error("Attempted to construct Object with a child that is already a child of another Object.");
    }

    std::shared_ptr<T> obj = std::make_shared<std::decay_t<T>>(std::forward<T>(child));
    
    (dynamic_cast<Object*>(obj.get()))->m_isInTree = true;
    m_children.push_back(obj);
}

template <typename T>
Templater::dynamic::Object& Templater::dynamic::Object::operator+=(T&& right) requires (isObject<T>) {
    addChild(std::move(right));
    return (*this);
}

template <typename T>
void Templater::dynamic::Object::processConstructorArgs(T& arg) {
    if constexpr (std::is_base_of_v<Object, std::decay_t<T>>) {
        processConstructorObject(arg);
    } else {
        processConstructorAttribute(arg);
    }
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