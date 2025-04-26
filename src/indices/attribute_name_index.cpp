#include "attribute_name_index.h"

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
}