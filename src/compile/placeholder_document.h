#pragma once

#include "../document.h"
#include "compile_placeholder.h"

namespace Templater::compile { 

    template <typename... Children>
    struct PlaceholderDocument {
        using doc = Document<Children...>;

        template<typename... Rest>
        static std::string serializeWithPlaceholders(const char* name, const Templater::dynamic::Node& node, Rest&... rest) {
            static_assert(sizeof...(Rest) % 2 == 0, "Arguments must be in key-value pairs.");
            std::string serialized = std::string(doc::serialize().data());
            evaluatePlaceholdersRec(serialized, name, node, rest...);
            return serialized;
        }

        template<typename T, typename... Rest>
        static std::unique_ptr<Templater::dynamic::Node> dynamicTreeWithPlaceholders(const char* name, T&& node, Rest&&... rest) requires(Templater::dynamic::isValidNodeChild<T>) {
            std::unique_ptr<Templater::dynamic::Node> obj = doc::dynamicTree();

            auto placeholderNodes = obj->getChildrenByTagName(ctags::Placeholder<"">::TAG_NAME);

            std::unordered_map<std::string, Templater::dynamic::Node*> placeholdersMap;

            for (auto& node: placeholderNodes) {
                placeholdersMap.emplace(node->getAttributeValue("name"), node);
            }

            evaluateDynamicTreeRec(obj.get(), placeholdersMap, name, node, rest...);

            return obj;
        }

        private:
            template<typename T, typename... Rest>
            static void evaluateDynamicTreeRec(Templater::dynamic::Node* result, 
                std::unordered_map<std::string, Templater::dynamic::Node*>& placeholders, 
                const char* name, 
                T&& node, 
                Rest&&... rest) requires(Templater::dynamic::isValidNodeChild<T>) {
                evaluateDynamicTreePlaceholder(result, placeholders, name, node);

                evaluateDynamicTreeRec(result, placeholders, rest...);
            }

            static void evaluateDynamicTreeRec(Templater::dynamic::Node* result, 
                                                std::unordered_map<std::string, Templater::dynamic::Node*>& placeholders) {
                return;
            }

            template<typename T>
            static void evaluateDynamicTreePlaceholder(Templater::dynamic::Node* result, 
                std::unordered_map<std::string, Templater::dynamic::Node*>& placeholders, 
                const char* name, 
                T&& node) requires(Templater::dynamic::isValidNodeChild<T>) {
                if (placeholders.find(name) != placeholders.end()) {
                    result->replaceChild(placeholders[name], std::move(node));
                } else {
                    throw std::invalid_argument(std::string("No dynamic binding ") + name + " exists in map with size " + std::to_string(placeholders.size()) + placeholders.begin()->first + placeholders.end()->first);
                }
            }

            template<typename... Rest>
            static void evaluatePlaceholdersRec(std::string& result, const char* name, const Templater::dynamic::Node& node, Rest&... rest) {
                evaluatePlaceholder(result, name, node);

                evaluatePlaceholdersRec(result, rest...);
            }

            static void evaluatePlaceholdersRec(std::string& result) {
                return;
            }

            static void evaluatePlaceholder(std::string& result, const char* name, const Templater::dynamic::Node& node) {
                std::string search = std::string("<") + ctags::Placeholder<"">::TAG_NAME + " name=\"" + name + "\" />";
                int pos = result.find(search);
                if (pos == std::string::npos) throw std::invalid_argument(std::string("No dynamic binding ") + name + " exists");

                result.replace(pos, search.size(), node.serialize());
            }
    };
}