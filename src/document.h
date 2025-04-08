#pragma once

#include "node.h"
#include "nodes/text_node.h"
#include "nodes/empty_node.h"
#include <string>
#include <algorithm>
#include <string_view>
#include <fstream>

#define COMPILE_DOCUMENT(...) [](){ \
    using namespace Templater::compile::ctags;\
    static std::string str = Templater::compile::Document<__VA_ARGS__>::value();\
    return str; \
}();

#define RAW_TEMPLATE(...) std::string(#__VA_ARGS__)

namespace Templater::compile {

    /**
     * @brief A compile time string.
     * 
     * @tparam N The size of the string
     */
    template <size_t N>
    struct CompileString {
        /**
         * @brief The char array that holds the string.
         * 
         */
        char value[N];


        /**
         * @brief Constructs a CompileString via a given const char array.
         * 
         * @param str
         * 
         */
        constexpr CompileString(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }


        /**
         * @brief Returns the std::string constructed from this CompileString.
         * 
         * @return constexpr std::string 
         */
        constexpr std::string to_string() const {
            return std::string(value, N - 1);
        }
    

        /**
         * @brief Cast to std::string
         * 
         * @return std::string 
         */
        constexpr operator std::string() const {
            return to_string();
        }
    };

    namespace ctags {


        /**
         * @brief A compile time Attribute struct which has a Name and a Value.
         * 
         * @tparam Name 
         * @tparam Value 
         */
        template <CompileString Name, CompileString Value>
        struct Attribute {
            /**
             * @brief Construct a dynamic Attribute from a compile time Attribute.
             * 
             * @return std::unique_ptr<Templater::dynamic::Attribute> 
             */
            static std::unique_ptr<Templater::dynamic::Attribute> attr() {
                return std::make_unique<Templater::dynamic::Attribute>(Name, Value);
            }
        };

        /**
         * @brief Checks if the given template param is a compile time Attribute.
         * Since compile time Attributes are specialised with unpredictable inputs,
         * the check can only be done by checking for the existence of the attr() static function.
         * 
         * @tparam T 
         */
        template <typename T>
        concept isAttribute = requires(T) {
            { T::attr() } -> std::same_as<std::unique_ptr<Templater::dynamic::Attribute>>;
        };


        /**
         * @brief A compile time Text struct which has a Name and a Value.
         * 
         * @tparam Name 
         * @tparam Value 
         */
        template <CompileString Str>
        struct Text {
            /**
             * @brief Construct a dynamic Text Node from a compile time Text struct.
             * 
             * @return std::unique_ptr<Templater::dynamic::Node> 
             */
            static std::unique_ptr<Templater::dynamic::Node> value() {
                return std::make_unique<Templater::dynamic::dtags::Text>(Str);
            }
        };


        /**
         * @brief Given a child, which is either a compile time Attribute or a struct which has a ::value() function,
         * recursively constructs a dynamic Node tree.
         * All different calls are generated at compile time via specialisation.
         * 
         * @tparam Child 
         * @param node 
         */
        template <typename Child>
        void parseChildren(Templater::dynamic::Node* node) {
            if constexpr(isAttribute<Child>) {
                std::unique_ptr<Templater::dynamic::Attribute> attr = Child::attr();
                node->operator[](attr->getName()) = attr->getValue();
            }
            else {
                node->addChild(Child::value());
            }
        }
    }


    /**
     * @brief A struct for creating XML/HTML documents with an alternative syntax, using templates:
     * 
     * @code{.cpp}
     * std::string doc1 = Document<
     *      html<
     *          Attribute<"lang", "en">,
     *          head<>,
     *          body<Text<"Hello world!">>
     *      >>::value("\t", true);
     * @endcode
     * 
     * These Documents are currently evaluated at runtime on call to Document::value(const std::string& indentationSequence, bool sortAttributes) via 
     * constructing a dynamic tree and serialising it. Compilation will fail if the Document is misformed.
     * 
     * @tparam Children 
     */
    template <typename... Children>
    struct Document {
        /**
         * @brief Returns the serialised std::string from the templated arguments. Returns an empty string if there are no children.
         * 
         * @param indentationSequence 
         * @param sortAttributes 
         * @return std::string 
         */
        static std::string value(
            const std::string& indentationSequence = Templater::dynamic::Node::getIndentationSequence(), 
            bool sortAttributes = Templater::dynamic::Node::getSortAttributes()) {
            Templater::dynamic::dtags::EmptyNode obj;
            if constexpr (sizeof...(Children) == 0)
            {
                return std::string("");
            }
            else
            {
                (([&] {
                    if constexpr(Templater::compile::ctags::isAttribute<Children>) {
                        return "Error. Trying to read attribute as root node";
                    }
                    else {
                        obj.addChild(Children::value());
                    }
                }()), ...);
            }
            return obj.serialise(indentationSequence, sortAttributes);
        }
    };

}