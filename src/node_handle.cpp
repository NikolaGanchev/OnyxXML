#include "node_handle.h"
#include "node.h"

namespace Templater::dynamic {

    void NodeHandle::reset() {
        if (this->isOwning && this->pointer) delete this->pointer;
        this->pointer  = nullptr;
        this->isOwning = false;
    }

    NodeHandle::NodeHandle(std::unique_ptr<Node> ptr)
      : pointer(ptr.release()), isOwning(true) {}

    NodeHandle::NodeHandle(Node* raw, bool owns)
      : pointer(raw), isOwning(owns){}

    NodeHandle::NodeHandle(NodeHandle&& o) noexcept
      : pointer(o.pointer), isOwning(o.isOwning) {
        o.pointer  = nullptr;
        o.isOwning = false;
    }
    
    NodeHandle& NodeHandle::operator=(NodeHandle&& o) noexcept {
        this->reset();
        this->pointer = o.pointer;
        o.pointer = nullptr;
        this->isOwning = o.isOwning;
        o.isOwning = false;
        return *this;
    }
    
    NodeHandle::~NodeHandle() {
      reset();
    }

    std::unique_ptr<Node> NodeHandle::toUnique() {
        if (!this->isOwning) throw std::logic_error("Handle does not own Node");
        this->isOwning = false;
        return std::unique_ptr<Node>(this->pointer);
    }
    
    Node* NodeHandle::get() { 
        return this->pointer; 
    }

    const Node* NodeHandle::get() const { 
        return this->pointer; 
    }

    Node& NodeHandle::operator*() { 
        return *this->pointer; 
    }

    Node* NodeHandle::operator->() { 
        return this->pointer; 
    }
    
    Node* NodeHandle::release() {
        isOwning = false;
        Node* tmp = this->pointer;
        this->pointer = nullptr;
        return tmp;
    }

    bool NodeHandle::owning() {
        return this->isOwning;
    }

    NodeHandle::operator bool() const {
        return this->pointer != nullptr;
    }
}