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

        template <typename T>
        concept isValidObjectConstructorType = std::derived_from<T, Object> || std::same_as<T, Attribute>;

        struct InternalObject {
            std::unordered_map<std::string, std::string> m_attributes;
            std::vector<std::shared_ptr<Object>> m_children;
            bool m_isInTree = false;
        };

        class Object {
            private:
                std::shared_ptr<InternalObject> m_object;
                void recursiveChildrenParse(std::vector<std::shared_ptr<Object>>& children, const std::shared_ptr<Object> obj, const std::function<bool(std::shared_ptr<Object>)>& condition) const;
                bool isInTree() const;
                
                template <typename T>
                void processConstructorArgs(T&& arg);
                void processConstructorAttribute(Attribute attribute);
                void processConstructorObject(Object& child);
                std::string serialise(std::string& identation) const;
                // Default: "\t"
                static std::string identationSequence;
                // Default: false
                static bool sortAttributes;
            public:
                template <typename... Args>
                explicit Object(Args&&... args) requires (isValidObjectConstructorType<Args>&& ...);
                explicit Object();
                virtual ~Object();
                virtual std::shared_ptr<Object> clone() const = 0;

                virtual const std::string& getTagName() const = 0;
                virtual bool isVoid() const = 0;
                
                const std::vector<std::shared_ptr<Object>>& getChildren() const;
                std::vector<std::shared_ptr<Object>> getChildrenByClassName(const std::string& className) const;
                std::vector<std::shared_ptr<Object>> getChildrenByTagName(const std::string& tagName) const;
                std::vector<std::shared_ptr<Object>> getChildrenByName(const std::string& name) const;
                std::vector<std::shared_ptr<Object>> getChildrenById(const std::string& id) const;
                std::vector<std::shared_ptr<Object>> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;
                
                const std::unordered_map<std::string, std::string>& getAttributes() const;
                const std::string& getAttributeValue(std::string& name) const;
                void setAttributeValue(std::string& name, std::string& newValue);
                std::string& operator[](const std::string& name);

                void addChild(Object&);
                Object& operator+(Object& right);
                Object& operator+=(Object& right);

                void removeChild(Object&);

                std::string serialise() const;

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
                const std::string& getTagName() const override;
                bool isVoid() const override;
                std::shared_ptr<Object> clone() const override;
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