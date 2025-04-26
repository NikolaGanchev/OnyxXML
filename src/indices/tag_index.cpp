#include "tag_index.h"

namespace Templater::dynamic::index {
    
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

    
    TagIndex::TagIndex(TagIndex&& other)
        : Index{std::move(other)}, index{std::move(other.index)} {}

    TagIndex& TagIndex::operator=(TagIndex&& other) {
        this->_assign_index_base(std::move(other));
        this->index = std::move(other.index);
        return *this;
    }
}