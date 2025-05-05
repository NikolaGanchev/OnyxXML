#pragma once

#include "../document.h"

namespace Templater::compile { 
    template <typename... Children>
    struct DocumentWithBindings {
        using doc = Document<Children...>;

        template<typename... Rest>
        static std::string serializeWithBindings(const char* name, const Templater::dynamic::Node& node, Rest&... rest) {
            static_assert(sizeof...(Rest) % 2 == 0, "Arguments must be in key-value pairs.");
            std::string serialized = std::string(doc::serialize().data());
            evaluateDynamicBindingsRec(serialized, name, node, rest...);
            return serialized;
        }

        template<typename T, typename... Rest>
        static std::unique_ptr<Templater::dynamic::Node> dynamicTreeWithBindings(const char* name, T&& node, Rest&&... rest) requires(Templater::dynamic::isValidNodeChild<T>) {
            std::unique_ptr<Templater::dynamic::Node> obj = doc::dynamicTree();

            auto dynamicBindingNodes = obj->getChildrenByTagName(DocumentUtils::dynamicBindingTagName);

            std::unordered_map<std::string, Templater::dynamic::Node*> dynamicBindingsMap;

            for (auto& node: dynamicBindingNodes) {
                dynamicBindingsMap.emplace(node->getAttributeValue("name"), node);
            }

            evaluateDynamicTreeRec(obj.get(), dynamicBindingsMap, name, node, rest...);

            return obj;
        }

        private:
            template<typename T, typename... Rest>
            static void evaluateDynamicTreeRec(Templater::dynamic::Node* result, 
                std::unordered_map<std::string, Templater::dynamic::Node*>& bindings, 
                const char* name, 
                T&& node, 
                Rest&&... rest) requires(Templater::dynamic::isValidNodeChild<T>) {
                evaluateDynamicTreeBinding(result, bindings, name, node);

                evaluateDynamicTreeRec(result, bindings, rest...);
            }

            static void evaluateDynamicTreeRec(Templater::dynamic::Node* result, std::unordered_map<std::string, Templater::dynamic::Node*>& bindings) {
                return;
            }

            template<typename T>
            static void evaluateDynamicTreeBinding(Templater::dynamic::Node* result, 
                std::unordered_map<std::string, Templater::dynamic::Node*>& bindings, 
                const char* name, 
                T&& node) requires(Templater::dynamic::isValidNodeChild<T>) {
                if (bindings.find(name) != bindings.end()) {
                    result->replaceChild(bindings[name], std::move(node));
                } else {
                    throw std::invalid_argument(std::string("No dynamic binding ") + name + " exists in map with size " + std::to_string(bindings.size()) + bindings.begin()->first + bindings.end()->first);
                }
            }

            template<typename... Rest>
            static void evaluateDynamicBindingsRec(std::string& result, const char* name, const Templater::dynamic::Node& node, Rest&... rest) {
                evaluateDynamicBinding(result, name, node);

                evaluateDynamicBindingsRec(result, rest...);
            }

            static void evaluateDynamicBindingsRec(std::string& result) {
                return;
            }

            static void evaluateDynamicBinding(std::string& result, const char* name, const Templater::dynamic::Node& node) {
                std::string search = std::string("<") + DocumentUtils::dynamicBindingTagName + " name=\"" + name + "\" />";
                int pos = result.find(search);
                if (pos == std::string::npos) throw std::invalid_argument(std::string("No dynamic binding ") + name + " exists");

                result.replace(pos, search.size(), node.serialize());
            }
    };
}