#include "index.h"

namespace Templater::dynamic::index {

    AttributeNameIndex::AttributeNameIndex(Node* root, std::string& attributeName)
        : attributeName(attributeName), Index(root), index{} {}; 

    AttributeNameIndex::AttributeNameIndex(Node* root, std::string&& attributeName)
        : attributeName(std::move(attributeName)), Index(root), index{} {}; 

    bool AttributeNameIndex::putIfNeeded(Node* node) {
        if (node->hasAttribute(this->attributeName)) {
            const std::string& value = node->getAttributeValue(this->attributeName);
            if (this->index.contains(value)) {
                // Protect from double insertion
                for (auto obj: this->index[value]) {
                    if (obj == node) return false;
                }

                index[value].push_back(node);
            } else {
                index[value] = { node };
            }
            return true;
        }
        return false;
    }

    bool AttributeNameIndex::removeIfNeeded(Node* node) {
        for (auto& [value, nodes] : this->index) {
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
        if (node->hasAttribute(this->attributeName)) {
            for (auto& obj: getByValue(node->getAttributeValue(this->attributeName))) {
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
        if (!this->index.contains(value) || !this->isValid()) {
            return {};
        }

        return this->index[value];
    }

    TagNameIndex::TagNameIndex(Node* root, std::string& tagName)
        : tagName(tagName), Index(root), index{} {}; 

    TagNameIndex::TagNameIndex(Node* root, std::string&& tagName)
        : tagName(std::move(tagName)), Index(root), index{} {}; 

    bool TagNameIndex::putIfNeeded(Node* node) {
        if (node->getTagName() == this->tagName) {
            for (auto obj: this->index) {
                if (obj == node) return false;
            }
            this->index.push_back(node);
            return true;
        }
        return false;
    }

    bool TagNameIndex::removeIfNeeded(Node* node) {
        for (auto obj = this->index.begin(); obj != this->index.end();) {
            if (*obj == node) {
                this->index.erase(obj);
                return true;
            }
            obj++;
        }
        
        return false;
    }

    // For all intents and purposes, tag names should be constant from node creation
    // If any subclass however allows tag name setting, then the set operation needs to call update()
    bool TagNameIndex::update(Node* node) {
        for (auto obj = this->index.begin(); obj != this->index.end();) {
            if (*obj == node) {
                if ((*obj)->getTagName() == node->getTagName()) return false;
                else {
                    this->index.erase(obj);
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
        
        return this->index;
    }

    TagIndex::TagIndex(Node* root)
        : Index(root), index{} {}; 

    bool TagIndex::putIfNeeded(Node* node) {
        const std::string& name = node->getTagName();

        if (this->index.contains(name)) {
            for (auto obj: this->index[name]) {
                if (obj == node) return false;
            }

            this->index[name].push_back(node);
        } else {
            this->index[name] = { node };
        }

        return true;
    }

    bool TagIndex::removeIfNeeded(Node* node) {
        const std::string& name = node->getTagName();

        if (this->index.contains(name)) {
            auto& nodes = this->index[name];

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

        if (this->index.contains(name)) {
            auto& nodes = this->index[name];

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
        if (!this->index.contains(tagName) || !this->isValid()) {
            return {};
        }

        return this->index[tagName];
    }

    
    CacheIndex::CacheIndex(Node* root)
        : Index(root), _cache{} {}; 

    bool CacheIndex::putIfNeeded(Node* node) {
        _cache.clear();
        return true;
    }

    bool CacheIndex::removeIfNeeded(Node* node) {
        _cache.clear();
        return true;
    }

    bool CacheIndex::update(Node* node) {
        _cache.clear();
        return true;
    }

}