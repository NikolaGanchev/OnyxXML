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

        struct InternalObject {
            const std::string m_tag;
            std::unordered_map<std::string, std::string> m_attributes;
            std::vector<std::shared_ptr<InternalObject>> m_children;
            const bool m_isVoid;
            bool m_isInTree = false;
        };

        class Object {
            private:
                std::shared_ptr<InternalObject> m_object;
                void recursiveChildrenParse(std::vector<Object>& children, const InternalObject& obj, const std::function<bool(InternalObject&)>& condition) const;
                bool isInTree() const;
            public:
                explicit Object(std::string, bool, std::initializer_list<Attribute> attributes, std::initializer_list<Object> children);
                Object(std::shared_ptr<InternalObject>);

                const std::string& getTagName() const;
                
                std::vector<Object> getChildren() const;
                std::vector<Object> getChildrenByClassName(const std::string& className) const;
                std::vector<Object> getChildrenByTagName(const std::string& tagName) const;
                std::vector<Object> getChildrenByName(const std::string& name) const;
                std::vector<Object> getChildrenById(const std::string& id) const;
                std::vector<Object> getChildrenByAttribute(const std::string& attribute, const std::string& value) const;
                
                const std::unordered_map<std::string, std::string>& getAttributes() const;
                // TODO: Include override of array operator
                const std::string& getAttributeValue(std::string& name) const;
                void setAttributeValue(std::string& name, std::string& newValue);

                void addChild(Object&);

                void removeChild(Object&);

                std::string serialise();
        };
    }
}
