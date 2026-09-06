#include "indices/tag_index.h"

#include <string_view>

namespace onyx::dynamic::index {

TagIndex::TagIndex(Node* root) : Index(root), index{} {};

bool TagIndex::putIfNeeded(Node* node) {
    std::optional<std::string_view> uriOptional = node->getNamespaceName();
    std::string uri =
        uriOptional.has_value() ? std::string(uriOptional.value()) : "";
    const std::string& name = node->getTagName();

    auto& tagMap = this->index[uri];
    auto it = tagMap.find(name);
    if (it != tagMap.end()) {
        for (auto* obj : it->second) {
            if (obj == node) return false;
        }
        it->second.push_back(node);
    } else {
        tagMap[name] = {node};
    }

    return true;
}

bool TagIndex::removeIfNeeded(Node* node) {
    bool removed = false;

    for (auto uriIt = this->index.begin(); uriIt != this->index.end();) {
        auto& tagMap = uriIt->second;

        for (auto tagIt = tagMap.begin(); tagIt != tagMap.end();) {
            auto& nodes = tagIt->second;

            for (auto nodeIt = nodes.begin(); nodeIt != nodes.end();) {
                if (*nodeIt == node) {
                    nodeIt = nodes.erase(nodeIt);
                    removed = true;
                } else {
                    nodeIt++;
                }
            }

            if (nodes.empty()) {
                tagIt = tagMap.erase(tagIt);
            } else {
                tagIt++;
            }
        }

        if (tagMap.empty()) {
            uriIt = this->index.erase(uriIt);
        } else {
            uriIt++;
        }
    }

    return removed;
}

bool TagIndex::update(Node* node) {
    bool removed = removeIfNeeded(node);
    bool added = putIfNeeded(node);
    return removed || added;
}

const std::vector<Node*> TagIndex::getByTagName(const std::string& tagName) {
    return getByTagName("", tagName);
}

const std::vector<Node*> TagIndex::getByTagName(const std::string& namespaceUri,
                                                const std::string& tagName) {
    if (!this->isValid()) return {};

    auto uriIt = this->index.find(namespaceUri);
    if (uriIt == this->index.end()) return {};

    auto tagIt = uriIt->second.find(tagName);
    if (tagIt == uriIt->second.end()) return {};

    return tagIt->second;
}

const std::vector<Node*> TagIndex::getByLocalName(const std::string& tagName) {
    if (!this->isValid()) return {};

    std::vector<Node*> result;
    for (const auto& [uri, tagMap] : this->index) {
        auto tagIt = tagMap.find(tagName);
        if (tagIt != tagMap.end()) {
            result.insert(result.end(), tagIt->second.begin(),
                          tagIt->second.end());
        }
    }
    return result;
}

const std::vector<Node*> TagIndex::getByNamespace(
    const std::string& namespaceUri) {
    if (!this->isValid()) return {};

    auto uriIt = this->index.find(namespaceUri);
    if (uriIt == this->index.end()) return {};

    std::vector<Node*> result;
    for (const auto& [tag, nodes] : uriIt->second) {
        result.insert(result.end(), nodes.begin(), nodes.end());
    }
    return result;
}
}  // namespace onyx::dynamic::index