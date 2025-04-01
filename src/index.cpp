#include "index.h"

namespace Templater::dynamic::index {

    AttributeNameIndex::AttributeNameIndex(Node* root, std::string& attributeName)
        : m_attributeName(attributeName), Index(root), m_index{} {}; 

    AttributeNameIndex::AttributeNameIndex(Node* root, std::string&& attributeName)
        : m_attributeName(std::move(attributeName)), Index(root), m_index{} {}; 

    bool AttributeNameIndex::putIfNeeded(Node* node) {
        if (node->hasAttribute(this->m_attributeName)) {
            const std::string& value = node->getAttributeValue(this->m_attributeName);
            if (this->m_index.contains(value)) {
                // Protect from double insertion
                for (auto obj: m_index[value]) {
                    if (obj == node) return false;
                }

                m_index[value].push_back(node);
            } else {
                m_index[value] = { node };
            }
            return true;
        }
        return false;
    }

    bool AttributeNameIndex::removeIfNeeded(Node* node) {
        for (auto& [value, nodes] : m_index) {
            for (auto it = nodes.begin(); it != nodes.end();) {
                if (node == *it) {
                    nodes.erase(it);
                    return true;
                }
                it++;
            }
        }
        return false;
    }

    bool AttributeNameIndex::update(Node* node) {
        if (node->hasAttribute(this->m_attributeName)) {
            for (auto& obj: getByValue(node->getAttributeValue(this->m_attributeName))) {
                if (obj == node) {
                    return false;
                }
            }
        }
        
        removeIfNeeded(node);
        putIfNeeded(node);
        return true;
    }


    const std::vector<Node*> AttributeNameIndex::getByValue(const std::string& value) {
        if (!m_index.contains(value) || !this->isValid()) {
            return {};
        }

        return m_index[value];
    }

    TagNameIndex::TagNameIndex(Node* root, std::string& tagName)
        : m_tagName(tagName), Index(root), m_index{} {}; 

    TagNameIndex::TagNameIndex(Node* root, std::string&& tagName)
        : m_tagName(std::move(tagName)), Index(root), m_index{} {}; 

    bool TagNameIndex::putIfNeeded(Node* node) {
        if (node->getTagName() == this->m_tagName) {
            for (auto obj: m_index) {
                if (obj == node) return false;
            }
            m_index.push_back(node);
            return true;
        }
        return false;
    }

    bool TagNameIndex::removeIfNeeded(Node* node) {
        for (auto obj = m_index.begin(); obj != m_index.end();) {
            if (*obj == node) {
                m_index.erase(obj);
                return true;
            }
            obj++;
        }
        
        return false;
    }

    // For all intents and purposes, tag names should be constant from node creation
    // If any subclass however allows tag name setting, then the set operation needs to call update()
    bool TagNameIndex::update(Node* node) {
        for (auto obj = m_index.begin(); obj != m_index.end();) {
            if (*obj == node) {
                if ((*obj)->getTagName() == node->getTagName()) return false;
                else {
                    m_index.erase(obj);
                    return true;
                }
            }
        }

        return false;
    }

    const std::vector<Node*> TagNameIndex::get() {
        if (!this->isValid()) {
            return {};
        }
        
        return m_index;
    }

    TagIndex::TagIndex(Node* root)
        : Index(root), m_index{} {}; 

    bool TagIndex::putIfNeeded(Node* node) {
        const std::string& name = node->getTagName();

        if (this->m_index.contains(name)) {
            for (auto obj: m_index[name]) {
                if (obj == node) return false;
            }

            m_index[name].push_back(node);
        } else {
            m_index[name] = { node };
        }

        return true;
    }

    bool TagIndex::removeIfNeeded(Node* node) {
        const std::string& name = node->getTagName();

        if (this->m_index.contains(name)) {
            auto& nodes = m_index[name];

            for (auto obj = nodes.begin(); obj != nodes.end();) {
                if (*obj == node) {
                    nodes.erase(obj);
                    return true;
                }
                obj++;
            }
        }

        return false;
    }

    // For all intents and purposes, tag names should be constant from node creation
    // If any subclass however allows tag name setting, then the set operation needs to call update()
    bool TagIndex::update(Node* node) {
        const std::string& name = node->getTagName();

        if (this->m_index.contains(name)) {
            auto& nodes = m_index[name];

            for (auto obj = nodes.begin(); obj != nodes.end();) {
                if (*obj == node) {
                    return false;
                }
            }
        }
        
        removeIfNeeded(node);
        putIfNeeded(node);
        return true;
    }


    const std::vector<Node*> TagIndex::getByTagName(const std::string& tagName) {
        if (!m_index.contains(tagName) || !this->isValid()) {
            return {};
        }

        return m_index[tagName];
    }

    
    CacheIndex::CacheIndex(Node* root)
        : Index(root), m_cache{} {}; 

    bool CacheIndex::putIfNeeded(Node* node) {
        m_cache.clear();
        return true;
    }

    bool CacheIndex::removeIfNeeded(Node* node) {
        m_cache.clear();
        return true;
    }

    bool CacheIndex::update(Node* node) {
        m_cache.clear();
        return true;
    }

}