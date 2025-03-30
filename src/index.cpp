#include "index.h"

namespace Templater::dynamic::index {

    AttributeNameIndex::AttributeNameIndex(Object* root, std::string& attributeName)
        : m_attributeName(attributeName), Index(root), m_index{} {}; 

    AttributeNameIndex::AttributeNameIndex(Object* root, std::string&& attributeName)
        : m_attributeName(std::move(attributeName)), Index(root), m_index{} {}; 

    bool AttributeNameIndex::putIfNeeded(Object* object) {
        if (object->hasAttributeValue(this->m_attributeName)) {
            const std::string& value = object->getAttributeValue(this->m_attributeName);
            if (this->m_index.contains(value)) {
                // Protect from double insertion
                for (auto obj: m_index[value]) {
                    if (obj == object) return false;
                }

                m_index[value].push_back(object);
            } else {
                m_index[value] = { object };
            }
            return true;
        }
        return false;
    }

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


    const std::vector<Object*> AttributeNameIndex::getByValue(const std::string& value) {
        if (!m_index.contains(value) || !this->isValid()) {
            return {};
        }

        return m_index[value];
    }

    TagNameIndex::TagNameIndex(Object* root, std::string& tagName)
        : m_tagName(tagName), Index(root), m_index{} {}; 

    TagNameIndex::TagNameIndex(Object* root, std::string&& tagName)
        : m_tagName(std::move(tagName)), Index(root), m_index{} {}; 

    bool TagNameIndex::putIfNeeded(Object* object) {
        if (object->getTagName() == this->m_tagName) {
            for (auto obj: m_index) {
                if (obj == object) return false;
            }
            m_index.push_back(object);
            return true;
        }
        return false;
    }

    bool TagNameIndex::removeIfNeeded(Object* object) {
        for (auto obj = m_index.begin(); obj != m_index.end();) {
            if (*obj == object) {
                m_index.erase(obj);
                return true;
            }
            obj++;
        }
        
        return false;
    }

    // For all intents and purposes, tag names should be constant from object creation
    // If any subclass however allows tag name setting, then the set operation needs to call update()
    bool TagNameIndex::update(Object* object) {
        for (auto obj = m_index.begin(); obj != m_index.end();) {
            if (*obj == object) {
                if ((*obj)->getTagName() == object->getTagName()) return false;
                else {
                    m_index.erase(obj);
                    return true;
                }
            }
        }

        return false;
    }

    const std::vector<Object*> TagNameIndex::get() {
        return m_index;
    }

    TagIndex::TagIndex(Object* root)
        : Index(root), m_index{} {}; 

    bool TagIndex::putIfNeeded(Object* object) {
        const std::string& name = object->getTagName();

        if (this->m_index.contains(name)) {
            for (auto obj: m_index[name]) {
                if (obj == object) return false;
            }

            m_index[name].push_back(object);
        } else {
            m_index[name] = { object };
        }

        return true;
    }

    bool TagIndex::removeIfNeeded(Object* object) {
        const std::string& name = object->getTagName();

        if (this->m_index.contains(name)) {
            auto& objects = m_index[name];

            for (auto obj = objects.begin(); obj != objects.end();) {
                if (*obj == object) {
                    objects.erase(obj);
                    return true;
                }
                obj++;
            }
        }

        return false;
    }

    // For all intents and purposes, tag names should be constant from object creation
    // If any subclass however allows tag name setting, then the set operation needs to call update()
    bool TagIndex::update(Object* object) {
        const std::string& name = object->getTagName();

        if (this->m_index.contains(name)) {
            auto& objects = m_index[name];

            for (auto obj = objects.begin(); obj != objects.end();) {
                if (*obj == object) {
                    return false;
                }
            }
        }
        
        removeIfNeeded(object);
        putIfNeeded(object);
        return true;
    }


    const std::vector<Object*> TagIndex::getByTagName(const std::string& tagName) {
        if (!m_index.contains(tagName) || !this->isValid()) {
            return {};
        }

        return m_index[tagName];
    }

    
    CacheIndex::CacheIndex(Object* root)
        : Index(root), m_cache{} {}; 

    bool CacheIndex::putIfNeeded(Object* object) {
        m_cache.clear();
        return true;
    }

    bool CacheIndex::removeIfNeeded(Object* object) {
        m_cache.clear();
        return true;
    }

    bool CacheIndex::update(Object* object) {
        m_cache.clear();
        return true;
    }

}