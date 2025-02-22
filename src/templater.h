#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <initializer_list>

namespace Templater {
    namespace html {

        class Attribute {
            private:
                std::string m_name;
                std::string m_value;
            public:
                explicit Attribute(std::string, std::string);
                explicit Attribute(std::string);
        };

        class Object {
            private:
                std::string m_tag;
                std::string m_id;
                std::unordered_map<std::string, std::string> m_attributes;
                std::vector<Object> m_children;
                bool isVoid;
            public:
                explicit Object(std::string);
                explicit Object(std::string, std::initializer_list<Attribute> attributes, std::initializer_list<Object> children);
                explicit Object(std::string, std::initializer_list<Object> children, std::initializer_list<Attribute> attributes);

                std::string& getTagName() const;
                
                std::vector<Object>& getChildren() const;
                std::vector<Object>& getChildrenByClassName() const;
                std::vector<Object>& getChildrenByTagName() const;
                std::vector<Object>& getChildrenByName() const;
                std::vector<Object>& getChildrenById() const;
                
                std::unordered_map<std::string, std::string> getAttributes() const;
                // TODO: Include override of array operator
                std::string& getAttributeValue(std::string& name) const;
                void setAttributeValue(std::string& name, std::string& newValue);

                void addChild(Object&);

                void removeChild(Object&);

                std::string serialise();
        };
    }
}
