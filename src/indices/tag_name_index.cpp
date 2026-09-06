#include "indices/tag_name_index.h"

#include <optional>
#include <string_view>

namespace onyx::dynamic::index {

TagNameIndex::TagNameIndex(Node* root, std::string tagName)
    : Index(root), namespaceUri{""}, tagName(std::move(tagName)), index{} {}

TagNameIndex::TagNameIndex(Node* root, std::string namespaceUri,
                           std::string tagName)
    : Index(root),
      namespaceUri{std::move(namespaceUri)},
      tagName(std::move(tagName)),
      index{} {}

TagNameIndex::TagNameIndex(Node* root, AnyNamespaceTag, std::string tagName)
    : Index(root),
      namespaceUri{std::nullopt},
      tagName(std::move(tagName)),
      index{} {}

bool TagNameIndex::putIfNeeded(Node* node) {
    if (node->getTagName() == this->tagName) {
        if (this->namespaceUri.has_value()) {
            std::optional<std::string_view> nodeUri = node->getNamespaceName();
            std::string_view finalUri =
                nodeUri.has_value() ? nodeUri.value() : "";
            if (this->namespaceUri != finalUri) return false;
        }
        for (auto obj : this->index) {
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

bool TagNameIndex::update(Node* node) {
    bool removed = removeIfNeeded(node);
    bool added = putIfNeeded(node);
    return removed || added;
}

const std::vector<Node*> TagNameIndex::get() {
    if (!this->isValid()) {
        return {};
    }

    return this->index;
}
}  // namespace onyx::dynamic::index