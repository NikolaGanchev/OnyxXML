#include "tag_name_index.h"

namespace Templater::dynamic::index {
    
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
}