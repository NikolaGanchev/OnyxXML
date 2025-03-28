#include "index.h"

namespace Templater::dynamic::index {

    AttributeNameIndex::AttributeNameIndex(Object* root, std::string& attributeName)
        : m_attributeName(attributeName), Index(root), m_index{} {}; 

    AttributeNameIndex::AttributeNameIndex(Object* root, std::string&& attributeName)
        : m_attributeName(std::move(attributeName)), Index(root), m_index{} {}; 

    // Returns true if the object was inserted; else returns false
    bool AttributeNameIndex::putIfNeeded(Object* object) {
        if (object->hasAttributeValue(this->m_attributeName)) {
            const std::string& value = object->getAttributeValue(this->m_attributeName);
            if (this->m_index.contains(value)) {
                m_index[value].push_back(object);
            } else {
                m_index[value] = { object };
            }
            return true;
        }
        return false;
    }

    // Returns true if the object was removed; else returns false
    bool AttributeNameIndex::removeIfNeeded(Object* object) {
        for (auto& [value, objects] : m_index) {
            for (auto it = objects.begin(); it != objects.end();) {
                if (object == *it) {
                    objects.erase(it);
                    return true;
                }
                it++;
            }
        }
        return false;
    }

    // Returns true if the object was updated; else returns false
    bool AttributeNameIndex::update(Object* object) {
        if (object->hasAttributeValue(this->m_attributeName)) {
            for (auto& obj: getByValue(object->getAttributeValue(this->m_attributeName))) {
                if (obj == object) {
                    return false;
                }
            }
        }
        
        removeIfNeeded(object);
        putIfNeeded(object);
        return true;
    }


    std::vector<Object*> AttributeNameIndex::getByValue(const std::string& value) {
        if (!m_index.contains(value) || !this->isValid()) {
            return {};
        }

        std::vector<Object*> result{};

        auto& objects = m_index[value];
        for (auto it = objects.begin(); it != objects.end();) {
            result.push_back(*it);
            it++;
        }

        return result;
    }
}