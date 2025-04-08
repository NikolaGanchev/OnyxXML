#include "index.h"

namespace Templater::dynamic::index {

    Index::Index(Node* root): root{root}, valid{true} {}

    const Node* Index::getRoot() const {
        return this->root;
    }

    bool Index::isValid() const {
        return this->valid;
    }

    void Index::invalidate() {
        this->valid = false;
    }

    void Index::init() {
        this->root->addIndex(this);
    }

    Index::~Index() {
        if (this->valid) {
            this->root->iterativeProcessor(*this->root, [this](Node* obj) -> void {
                for (auto index = obj->indices.begin(); index != obj->indices.end();) {
                    if (this == *index) {
                        obj->indices.erase(index);
                        break;
                    } else {
                        index++;
                    }
                }
            });
        }
    }
}