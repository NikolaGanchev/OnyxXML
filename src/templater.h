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

        struct InternalObject {
            std::unordered_map<std::string, std::string> m_attributes;
            std::vector<Object> m_children;
            bool m_isInTree = false;
        };

        class Object {
            private:
                std::shared_ptr<InternalObject> m_object;
                void recursiveChildrenParse(std::vector<Object>& children, const Object& obj, const std::function<bool(Object&)>& condition) const;
                bool isInTree() const;
            public:
                explicit Object(std::initializer_list<Attribute> attributes, std::initializer_list<Object> children);
                virtual ~Object();

                virtual const std::string& getTagName() const = 0;
                virtual bool isVoid() const = 0;
                
                std::vector<Object> getChildren() const;
                std::vector<Object> getChildrenByClassName(const std::string& className) const;
                std::vector<Object> getChildrenByTagName(const std::string& tagName) const;
                std::vector<Object> getChildrenByName(const std::string& name) const;
                std::vector<Object> getChildrenById(const std::string& id) const;
                std::vector<Object> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;
                
                const std::unordered_map<std::string, std::string>& getAttributes() const;
                const std::string& getAttributeValue(std::string& name) const;
                void setAttributeValue(std::string& name, std::string& newValue);
                std::string& operator[](const std::string& name);

                void addChild(Object&);
                Object& operator+(Object& right);
                Object& operator+=(Object& right);

                void removeChild(Object&);

                std::string serialise();
        };

        class GenericObject: public Object {
            private:
                const std::string m_tag;
                const bool m_isVoid;
            public: 
                explicit GenericObject(const std::string tagName, bool isVoid, std::initializer_list<Attribute> attributes, std::initializer_list<Object> children);
                const std::string& getTagName() const override;
                bool isVoid() const override;
        };
    }
}
