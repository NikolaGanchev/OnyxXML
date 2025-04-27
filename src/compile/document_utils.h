#pragma once

#include "compile_attribute.h"
#include "compile_string.h"
#include "compile_string_utils.h"
#include "compile_attribute.h"
#include <string>
#include <string_view>
#include <array>

namespace Templater::compile {

    struct DocumentUtils {
        DocumentUtils() = delete;

        template <size_t N, typename... Children>
        static consteval std::array<char, N + 1> serializeNode(const char* tagName) {
            std::array<char, N + 1> result = {};
            bool passedAttr = false;
            size_t index = CompileStringUtils::placeStringInArray(result, "<", 0);
            index = CompileStringUtils::placeStringInArray(result, tagName, index);
            if constexpr (sizeof...(Children) == 0)
            {
                index = CompileStringUtils::placeStringInArray(result, ">", index);
            } else {
            
                (([&] {
                    if constexpr(Templater::compile::ctags::isAttribute<Children>) {
                        if (passedAttr) {
                            throw "Cannot add attribute after first child of node.";
                        }
                    }
                    else {
                    
                        if (!passedAttr) {
                            passedAttr = true;
                            result[index] = '>';
                            index++;
                        }
                    
                    }

                    std::array<char, Children::size() + 1> in = Children::serialize();
                    for (size_t i = 0; i < Children::size(); i++) {
                        result[index + i] = in[i];
                    }
                    index += Children::size();
                }()), ...);
            }
            index = CompileStringUtils::placeStringInArray(result, "</", index);
            index = CompileStringUtils::placeStringInArray(result, tagName, index);
            index = CompileStringUtils::placeStringInArray(result, ">\0", index);
            return result;
        } 
    
        template <size_t N, typename... Children>
        static consteval std::array<char, N + 1> serializeVoidNode(const char* tagName) {
            std::array<char, N + 1> result = {};
            bool passedAttr = false;
            size_t index = CompileStringUtils::placeStringInArray(result, "<", 0);
            index = CompileStringUtils::placeStringInArray(result, tagName, index);
            if constexpr (sizeof...(Children) != 0)
            {
                (([&] {
                    if constexpr(!Templater::compile::ctags::isAttribute<Children>) {
                        throw "Cannot add non-attribute child for void node.";
                    
                    }

                    std::array<char, Children::size() + 1> in = Children::serialize();
                    for (size_t i = 0; i < Children::size(); i++) {
                        result[index + i] = in[i];
                    }
                    index += Children::size();
                }()), ...);
            }

            index = CompileStringUtils::placeStringInArray(result, " />\0", index);
        
            return result;
        } 

        /**
         * @brief Given a child, which is either a compile time Attribute or a struct which has a ::dynamicTree() function,
         * recursively constructs a dynamic Node tree.
         * All different calls are generated at compile time via specialization.
         * 
         * @tparam Child 
         * @param node 
         */
        template <typename Child>
        static void parseChildren(Templater::dynamic::Node* node) {
            if constexpr(Templater::compile::ctags::isAttribute<Child>) {
                std::unique_ptr<Templater::dynamic::Attribute> attr = Child::dynamicAttribute();
                node->operator[](attr->getName()) = attr->getValue();
            }
            else {
                node->addChild(Child::dynamicTree());
            }
        }
    };
}