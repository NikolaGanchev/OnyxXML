#include "index.h"

namespace onyx::dynamic {

Node::Index::Index(Node* root) : root{root} {}

const Node* Node::Index::getRoot() const { return this->root; }

bool Node::Index::isValid() const { return this->root != nullptr; }

void Node::Index::invalidate() { this->root = nullptr; }

void Node::Index::init() { this->root->addIndex(this); }

void Node::Index::destroy() {
    if (this->isValid() && this->root) {
        this->root->iterativeProcessor([this](Node* obj) -> void {
            for (auto index = obj->indices.begin();
                 index != obj->indices.end();) {
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

Node::Index::~Index() { this->destroy(); }

void Node::Index::_move_index_base(Index&& other) {
    this->root = other.root;

    if (other.isValid()) {
        other.root->replaceIndex(&other, this);
    }

    other.root = nullptr;
}

void Node::Index::_assign_index_base(Node::Index&& other) {
    this->destroy();
    this->_move_index_base(std::move(other));
}

Node::Index::Index(Node::Index&& other) noexcept {
    this->_move_index_base(std::move(other));
}
}  // namespace onyx::dynamic