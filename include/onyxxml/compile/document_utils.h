#pragma once

#include <array>
#include <string>
#include <string_view>

#include "compile_attribute.h"
#include "compile_string.h"
#include "compile_string_utils.h"
#include "node.h"

namespace onyx::compile {

/**
 * @brief Utilities for the compile Document api.
 *
 */
struct DocumentUtils {
    DocumentUtils() = delete;

    template <std::size_t ContentSize, std::size_t PlaceholderCount,
              typename... Children>
    static consteval void evaluateNode(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result,
        const char* tagName) {
        bool passedAttr = false;
        CompileStringUtils::placeStringInEvaluatedDocument(result, "<");
        CompileStringUtils::placeStringInEvaluatedDocument(result, tagName);
        if constexpr (sizeof...(Children) == 0) {
            CompileStringUtils::placeStringInEvaluatedDocument(result, ">");
        } else {
            (
                ([&]() consteval {
                    if constexpr (onyx::compile::ctags::isAttribute<Children>) {
                        if (passedAttr) {
                            throw "Cannot add attribute after first child of node.";
                        }
                    } else {
                        if (!passedAttr) {
                            passedAttr = true;
                            CompileStringUtils::placeStringInEvaluatedDocument(
                                result, ">");
                        }
                    }
                    Children::evaluate(result);
                }()),
                ...);
        }
        CompileStringUtils::placeStringInEvaluatedDocument(result, "</");
        CompileStringUtils::placeStringInEvaluatedDocument(result, tagName);
        CompileStringUtils::placeStringInEvaluatedDocument(result, ">\0");
    }

    template <std::size_t ContentSize, std::size_t PlaceholderCount,
              typename... Children>
    static consteval void evaluateVoidNode(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result,
        const char* tagName) {
        bool passedAttr = false;
        CompileStringUtils::placeStringInEvaluatedDocument(result, "<");
        CompileStringUtils::placeStringInEvaluatedDocument(result, tagName);
        if constexpr (sizeof...(Children) != 0) {
            (([&]() consteval {
                 if constexpr (!onyx::compile::ctags::isAttribute<Children>) {
                     throw "Cannot add non-attribute child for void node.";
                 }

                 Children::evaluate(result);
             }()),
             ...);
        }

        CompileStringUtils::placeStringInEvaluatedDocument(result, " />\0");
    }

    /**
     * @brief Given a child, which is either a compile time Attribute or a
     * struct which has a ::dynamicTree() function, recursively constructs a
     * dynamic Node tree. All different calls are generated at compile time via
     * specialization.
     *
     * @tparam Child
     * @param node
     */
    template <typename Child>
    static void parseChildren(onyx::dynamic::Node* node) {
        if constexpr (onyx::compile::ctags::isAttribute<Child>) {
            std::unique_ptr<onyx::dynamic::Attribute> attr =
                Child::dynamicAttribute();
            node->operator[](attr->getName()) = attr->getValue();
        } else {
            node->addChild(Child::dynamicTree());
        }
    }
};
}  // namespace onyx::compile