#include "index.h"

namespace Templater::dynamic::index {

    Index::Index(Node* root): root{root}, valid{true} {}

    const Node* Index::getRoot() const {
        return this->valid ? this->root: nullptr;
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

    void Index::destroy() {
        if (this->valid && this->root) {
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

    Index::~Index() {
        this->destroy();
    }
    
    void Index::_move_index_base(Index&& other) {
        this->root = other.root;
        this->valid = other.valid;

        if (other.isValid()) {
            other.root->replaceIndex(&other, this);
        }

        other.valid = false;
        other.root = nullptr;
    }

    void Index::_assign_index_base(Index&& other) {
        this->destroy();
        this->_move_index_base(std::move(other));
    }
    
    Index::Index(Index&& other) {
        this->_move_index_base(std::move(other));
    }
}