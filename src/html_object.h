#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <memory>
#include <functional>

namespace Templater {
    namespace html {

        class Attribute {
            private:
                std::string m_name;
                std::string m_value;
            public:
                explicit Attribute(std::string, std::string);
                explicit Attribute(std::string);
                const std::string& getName() const;
                const std::string& getValue() const;
        };

        class Object;
        class EmptyTag;

        template <typename T>
        concept isValidObjectConstructorType = std::derived_from<T, Object> || std::same_as<T, Attribute>;

        struct InternalObject {
            std::unordered_map<std::string, std::string> m_attributes;
            std::vector<std::shared_ptr<Object>> m_children;
            bool m_isInTree = false;

            ~InternalObject();
        };

        class Object {
            friend InternalObject;
            friend EmptyTag;
            private:
                std::shared_ptr<InternalObject> m_object;
                void recursiveChildrenParse(std::vector<std::shared_ptr<Object>>& children, const std::shared_ptr<Object> obj, const std::function<bool(std::shared_ptr<Object>)>& condition) const;
                
                template <typename T>
                void processConstructorArgs(T&& arg);
                void processConstructorAttribute(Attribute attribute);
                void processConstructorObject(Object& child);
                bool removeChild(Object& childToRemove, Object& currentRoot);
                // Default: "\t"
                static std::string identationSequence;
                // Default: false
                static bool sortAttributes;
            protected:
                virtual std::string serialise(std::string& identation) const;
            public:
                template <typename... Args>
                explicit Object(Args&&... args) requires (isValidObjectConstructorType<Args>&& ...);
                explicit Object(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
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
                
                const std::unordered_map<std::string, std::string>& getAttributes() const;
                const std::string& getAttributeValue(const std::string& name) const;
                void setAttributeValue(const std::string& name, const std::string& newValue);
                std::string& operator[](const std::string& name);

                void addChild(Object& child);
                void addChild(Object&& child);

                Object& operator+=(Object& right);
                Object& operator+=(Object&& right);
                // Checks equality by pointer to InternalObject
                // A && version is not needed, as it would be illogical to compare pointers with a temporary object
                bool operator==(Object& right);

                bool removeChild(Object& childToRemove);
                bool removeChild(const std::shared_ptr<Object>& childToRemove);

                virtual std::string serialise() const;

                static void setIdentationSequence(const std::string& newSequence);
                static const std::string& getIdentationSequence();
                static void setSortAttributes(bool shouldSort);
                static bool getSortAttributes();
        };

        class GenericObject: public Object {
            private:
                const std::string m_tag;
                const bool m_isVoid;
            public: 
                template <typename... Args>
                explicit GenericObject(std::string  tagName, bool isVoid, Args&&... args);
                explicit GenericObject(std::string  tagName, bool isVoid);
                explicit GenericObject(std::string  tagName, bool isVoid, std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
                const std::string& getTagName() const override;
                bool isVoid() const override;
                std::shared_ptr<Object> clone() const override;
        };

        // Used to create a tree without a root
        class EmptyTag: public Object {
            public: 
                template <typename... Args>
                explicit EmptyTag(Args&&... args);
                explicit EmptyTag(std::vector<Attribute> attributes, std::vector<std::shared_ptr<Object>> children);
                const std::string& getTagName() const override;
                bool isVoid() const override;
                std::shared_ptr<Object> clone() const override;
                std::string serialise() const override;
                std::string serialise(std::string& identation) const override;
        };

        class Text: public Object {
            private:
                const std::string m_text;
            public:
                explicit Text(std::string text); 
                bool isVoid() const override;
                std::shared_ptr<Object> clone() const override;
                std::string serialise() const override;
                std::string serialise(std::string& identation) const override;
                const std::string& getText() const;
                const std::string& getTagName() const override;
        };
    }
}

template <typename... Args>
Templater::html::Object::Object(Args&&... args) requires (Templater::html::isValidObjectConstructorType<Args>&& ...) { 
    m_object = std::make_shared<InternalObject>(InternalObject{{}, {}, false});

    (processConstructorArgs(std::forward<Args>(args)), ...);
}

template <typename T>
void Templater::html::Object::processConstructorArgs(T&& arg) {
    if constexpr (std::is_base_of_v<Object, std::decay_t<T>>) {
        processConstructorObject(arg);
    } else {
        processConstructorAttribute(std::forward<T>(arg));
    }
}

template <typename... Args>
Templater::html::GenericObject::GenericObject(std::string  tagName, bool isVoid, Args&&... args)
    : m_tag{std::move(tagName)}, m_isVoid{isVoid}, Templater::html::Object(std::forward<Args>(args)...) {}

    
template <typename... Args>
Templater::html::EmptyTag::EmptyTag(Args&&... args)
    : Templater::html::Object(std::forward<Args>(args)...) {}
