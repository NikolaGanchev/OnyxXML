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

    class Object {
        friend dtags::EmptyTag;
        private:
            struct Data {
                std::vector<Attribute> m_attributes;
                std::vector<std::shared_ptr<Object>> m_children;
                std::vector<std::shared_ptr<Index>> indeces;
                bool m_isInTree = false;

                ~Data();
            };

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

            std::shared_ptr<Data> m_object;
            void recursiveChildrenParse(std::vector<std::shared_ptr<Object>>& children, const std::shared_ptr<Object> obj, const std::function<bool(std::shared_ptr<Object>)>& condition) const;
            
            template <typename T>
            void processConstructorArgs(T&& arg);
            void processConstructorAttribute(Attribute&& attribute);
            void processConstructorObject(Object&& child);
            template <typename T>
            void processConstructorArgs(T& arg);
            void processConstructorAttribute(const Attribute& attribute);
            void processConstructorObject(Object& child);
            bool removeChild(Object& childToRemove, Object& currentRoot);
            // Default: "\t"
            static std::string indentationSequence;
            // Default: false
            static bool sortAttributes;
        public:
            template <typename... Args>
            explicit Object(Args&&... args) requires (isValidObjectConstructorType<Args>&& ...);
            template <typename... Args>
            explicit Object(Args&... args) requires (isValidObjectConstructorType<Args>&& ...);
            explicit Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
            explicit Object(const Object& other);
            explicit Object(Object&& other);
            explicit Object();
            virtual ~Object();
            virtual std::shared_ptr<Object> clone() const = 0; 

            virtual const std::string& getTagName() const = 0;
            virtual bool isVoid() const = 0;

            bool isInTree() const;
            // Returns a static list of the direct children
            const std::vector<std::shared_ptr<Object>> getChildren() const;
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

            void addChild(Object& child);
            void addChild(Object&& child);

            Object& operator+=(Object& right);
            Object& operator+=(Object&& right);
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

    namespace dtags {

        class GenericObject: public Object {
            private:
                const std::string m_tag;
                const bool m_isVoid;
            public: 
                template <typename... Args>
                explicit GenericObject(std::string  tagName, bool isVoid, Args&&... args);
                template <typename... Args>
                explicit GenericObject(std::string  tagName, bool isVoid, Args&... args) requires (isValidObjectConstructorType<Args>&& ...);
                explicit GenericObject(std::string  tagName, bool isVoid);
                explicit GenericObject(std::string  tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
                explicit GenericObject(const Object& other);
                explicit GenericObject(Object&& other);
                const std::string& getTagName() const override;
                bool isVoid() const override;
                std::shared_ptr<Object> clone() const override;
        };

        // Used to create a tree without a root
        class EmptyTag: public Object {
            public: 
                using Object::Object;
                const std::string& getTagName() const override;
                bool isVoid() const override;
                std::shared_ptr<Object> clone() const override;
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
                std::shared_ptr<Object> clone() const override;
                const std::string& getText() const;
                const std::string& getTagName() const override;
                std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const override;
        };
    }
}

template <typename... Args>
Templater::dynamic::Object::Object(Args&&... args) requires (Templater::dynamic::isValidObjectConstructorType<Args>&& ...) { 
    m_object = std::make_shared<Data>(Data{{}, {}, {}, false});

    (processConstructorArgs(std::forward<Args>(args)), ...);
}

template <typename... Args>
Templater::dynamic::Object::Object(Args&... args) requires (isValidObjectConstructorType<Args>&& ...) {    
    m_object = std::make_shared<Data>(Data{{}, {}, {}, false});

    (processConstructorArgs(std::forward<Args>(args)), ...);
}

template <typename T>
void Templater::dynamic::Object::processConstructorArgs(T&& arg) {
    if constexpr (std::is_base_of_v<Object, std::decay_t<T>>) {
        processConstructorObject(std::move(arg));
    } else {
        processConstructorAttribute(std::move(arg));
    }
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
Templater::dynamic::dtags::GenericObject::GenericObject(std::string  tagName, bool isVoid, Args&&... args)
    : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Templater::dynamic::Object(std::forward<Args>(args)...) {}


template <typename... Args>
Templater::dynamic::dtags::GenericObject::GenericObject(std::string  tagName, bool isVoid, Args&... args) requires (isValidObjectConstructorType<Args>&& ...) 
: m_tag{std::move(tagName)}, m_isVoid{isVoid}, Templater::dynamic::Object(std::forward<Args>(args)...) {}
