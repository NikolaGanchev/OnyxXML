#include "indices/attribute_name_index.h"

#include "index.h"

namespace onyx::dynamic::index {
AttributeNameIndex::AttributeNameIndex(Node* root, std::string localName)
    : Index(root), namespaceUri{""}, localName(std::move(localName)), index{} {}

AttributeNameIndex::AttributeNameIndex(Node* root, std::string namespaceUri,
                                       std::string localName)
    : Index(root),
      namespaceUri{std::move(namespaceUri)},
      localName(std::move(localName)),
      index{} {}

AttributeNameIndex::AttributeNameIndex(Node* root, AnyNamespaceTag,
                                       std::string localName)
    : Index(root),
      namespaceUri{std::nullopt},
      localName(std::move(localName)),
      index{} {}

std::optional<std::string_view> AttributeNameIndex::findMatchingAttributeValue(
    Node* node) const {
    for (const auto& attr : node->getAttributes()) {
        if (attr.getNCNameWithoutNamespace() != this->localName) {
            continue;
        }

        if (!this->namespaceUri.has_value()) {
            return attr.getValue();
        }

        std::optional<std::string_view> resolvedUri =
            node->resolveAttributeNamespacePrefix(attr.getNamespacePrefix());
        std::string_view uri = resolvedUri ? resolvedUri.value() : "";

        if (uri == this->namespaceUri.value()) {
            return attr.getValue();
        }
    }

    return std::nullopt;
}

bool AttributeNameIndex::putIfNeeded(Node* node) {
    std::optional<std::string_view> attrVal = findMatchingAttributeValue(node);
    if (!attrVal.has_value()) {
        return false;
    }

    std::string value(attrVal.value());
    if (this->index.contains(value)) {
        for (auto* obj : this->index[value]) {
            if (obj == node) return false;
        }
        this->index[value].push_back(node);
    } else {
        this->index[value] = {node};
    }
    return true;
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
    std::optional<std::string_view> attrVal = findMatchingAttributeValue(node);
    if (attrVal) {
        std::string valStr(attrVal.value());
        if (this->index.contains(valStr)) {
            for (auto* obj : this->index[valStr]) {
                if (obj == node) {
                    return false;
                }
            }
        }
    }

    removeIfNeeded(node);
    putIfNeeded(node);
    return true;
}

const std::vector<Node*> AttributeNameIndex::getByValue(
    const std::string& value) {
    if (!this->index.contains(value) || !this->isValid()) {
        return {};
    }

    return this->index[value];
}
}  // namespace onyx::dynamic::index