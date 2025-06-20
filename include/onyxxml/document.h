#pragma once

#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>

#include "compile/compile_attribute.h"
#include "compile/compile_comment.h"
#include "compile/compile_text.h"
#include "compile/document_utils.h"
#include "node.h"
#include "nodes/empty_node.h"

#define RAW_XML(...) std::string(#__VA_ARGS__)

namespace onyx::compile {

/**
 * @brief A struct for creating XML/HTML documents with an alternative syntax,
 * using templates:
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
 * A Document can be either fully compile-time evaluated by calling serialize()
 * or runtime-evaluated using value(const std::string& indentationSequence, bool
 * sortAttributes)
 *
 * @tparam Children
 */
template <typename... Children>
struct Document {
    /**
     * @brief The compile-time size of the Document's string. Does not account
     * for '\0';
     *
     * @return size_t
     */
    static consteval size_t size() {
        size_t size = 0;
        ((size += Children::size()), ...);
        return size;
    }

    /**
     * @brief The compile-time generated non-formatted string built for this
     * Document. Does not do escaping.
     *
     * @return std::array<char, size() + 1> The std::array containing the string
     */
    static consteval std::array<char, size() + 1> serialize() {
        std::array<char, size() + 1> result = {};
        size_t index = 0;
        (([&] {
             if constexpr (onyx::compile::ctags::isAttribute<Children>) {
                 throw "Cannot add attribute as root node of Document.";
             } else {
                 std::array<char, Children::size() + 1> in =
                     Children::serialize();
                 for (size_t i = 0; i < Children::size(); i++) {
                     result[index + i] = in[i];
                 }
                 index += Children::size();
             }
         }()),
         ...);
        result[index] = '\0';
        return result;
    }

    /**
     * @brief Casts the compile time std::array from ::serialize() to an
     * std::string at runtime.
     *
     * @return std::string
     */
    static std::string toString() { return std::string(serialize().data()); }

    /**
     * @brief Returns the dynamic tree from the templated arguments. Calculated
     * at runtime.
     *
     * @return std::string
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        std::unique_ptr<onyx::dynamic::tags::EmptyNode> obj =
            std::make_unique<onyx::dynamic::tags::EmptyNode>();

        if constexpr (sizeof...(Children) == 0) {
            return obj;
        } else {
            (([&] {
                 if constexpr (onyx::compile::ctags::isAttribute<
                                   Children>) {
                     throw "Trying to read attribute as root node";
                 } else {
                     obj->addChild(Children::dynamicTree());
                 }
             }()),
             ...);
        }
        return obj;
    }
};

}  // namespace onyx::compile