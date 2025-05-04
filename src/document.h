#pragma once

#include "node.h"
#include "nodes/empty_node.h"
#include <string>
#include <string_view>
#include "compile/compile_attribute.h"
#include "compile/compile_comment.h"
#include "compile/compile_text.h"
#include "compile/document_utils.h"
#include <initializer_list>
#include <utility>
#include "compile/dynamic_binding.h"

#define COMPILE_DOCUMENT(...) [](){ \
    using namespace Templater::compile::ctags;\
    static std::string str = Templater::compile::Document<__VA_ARGS__>::dynamicTree();\
    return str; \
}();

#define RAW_TEMPLATE(...) std::string(#__VA_ARGS__)

namespace Templater::compile {

    /**
     * @brief A struct for creating XML/HTML documents with an alternative syntax, using templates:
     * 
     * @code{.cpp}
     * std::string doc1 = Document<
     *      html<
     *          Attribute<"lang", "en">,
     *          head<>,
     *          body<Text<"Hello world!">>
     *      >>;
     * @endcode
     * 
     * A Document can be either fully compile-time evaluated by calling serialize() or runtime-evaluated using value(const std::string& indentationSequence, bool sortAttributes)
     * 
     * @tparam Children 
     */
    template <typename... Children>
    struct Document {

        /**
         * @brief The compile-time size of the Document's string. Does not account for '\0';
         * 
         * @return size_t 
         */
        static consteval size_t size() {
            size_t size = 0;
            ((size += Children::size()), ...);
            return size;
        }


        /**
         * @brief The compile-time generated non-formatted string built for this Document. Does not do escaping.
         * 
         * @return std::array<char, size() + 1> The std::array containing the string
         */
        static consteval std::array<char, size() + 1> serialize() {
            std::array<char, size() + 1> result = {};
            size_t index = 0;
            (([&] {
                if constexpr(Templater::compile::ctags::isAttribute<Children>) {
                    throw "Cannot add attribute as root node of Document.";
                }
                else {
                    std::array<char, Children::size() + 1> in = Children::serialize();
                    for (size_t i = 0; i < Children::size(); i++) {
                        result[index + i] = in[i];
                    }
                    index += Children::size();
                }
            }()), ...);
            result[index] = '\0';
            return result;
        } 


        static std::string toString() {
            return std::string(serialize().data());
        }

        template<typename... Rest>
        static std::string serializeWithBindings(const char* name, const Templater::dynamic::Node& node, Rest&... rest) {
            static_assert(sizeof...(Rest) % 2 == 0, "Arguments must be in key-value pairs.");
            std::string serialized = toString();
            evaluateDynamicBindingsRec(serialized, name, node, rest...);
            return serialized;
        }

        /**
         * @brief Returns the dynamic tree from the templated arguments. Calculated at runtime.
         * 
         * @return std::string 
         */
        static std::unique_ptr<Templater::dynamic::Node> dynamicTree() {
            std::unique_ptr<Templater::dynamic::dtags::EmptyNode> obj = std::make_unique<Templater::dynamic::dtags::EmptyNode>();

            if constexpr (sizeof...(Children) == 0)
            {
                return obj;
            }
            else
            {
                (([&] {
                    if constexpr(Templater::compile::ctags::isAttribute<Children>) {
                        throw "Trying to read attribute as root node";
                    }
                    else {
                        obj->addChild(Children::dynamicTree());
                    }
                }()), ...);
            }
            return obj;
        }

        template<typename T, typename... Rest>
        static std::unique_ptr<Templater::dynamic::Node> dynamicTreeWithBindings(const char* name, T&& node, Rest&&... rest) requires(Templater::dynamic::isValidNodeChild<T>) {
            std::unique_ptr<Templater::dynamic::Node> obj = dynamicTree();

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