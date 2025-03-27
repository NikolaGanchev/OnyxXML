#include "index.h"

namespace Templater::dynamic::index {

    AttributeNameIndex::AttributeNameIndex(std::shared_ptr<Object> root, std::string& attributeName)
        : m_attributeName(attributeName), Index(root), m_index{} {}; 

    AttributeNameIndex::AttributeNameIndex(std::shared_ptr<Object> root, std::string&& attributeName)
        : m_attributeName(std::move(attributeName)), Index(root), m_index{} {}; 

    // Returns true if the object was inserted; else returns false
    bool AttributeNameIndex::putIfNeeded(std::shared_ptr<Object> object) {
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
    bool AttributeNameIndex::removeIfNeeded(std::shared_ptr<Object> object) {
        for (auto& [value, objects] : m_index) {
            for (auto it = objects.begin(); it != objects.end();) {
                if (std::shared_ptr<Object> obj = it->lock()) {
                    if (object.get() == obj.get()) {
                        objects.erase(it);
                        return true;
                    }
                    it++;
                } else {
                    objects.erase(it);
                }
            }
        }
        return false;
    }

    // Returns true if the object was updated; else returns false
    bool AttributeNameIndex::update(std::shared_ptr<Object> object) {
        if (object->hasAttributeValue(this->m_attributeName)) {
            for (auto& obj: getByValue(object->getAttributeValue(this->m_attributeName))) {
                if (obj.get() == object.get()) {
                    return false;
                }
            }
        }
        
        removeIfNeeded(object);
        putIfNeeded(object);
        return true;
    }


    std::vector<std::shared_ptr<Object>> AttributeNameIndex::getByValue(const std::string& value) {
        if (!m_index.contains(value)) {
            return {};
        }

        std::vector<std::shared_ptr<Object>> result{};

        auto& objects = m_index[value];
        for (auto it = objects.begin(); it != objects.end();) {
            if (std::shared_ptr<Object> obj = it->lock()) {
                result.push_back(obj);
                it++;
            } else {
                objects.erase(it);
            }
        }

        return result;
    }

    void AttributeNameIndex::init() {
        this->initInternal(shared_from_this());
    }
}