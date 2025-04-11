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
            * @brief The compile-time size of the attribute string. Does not account for '\0';
            * The attribute string is formed as follows: ` name="value"`.
            * 
            * @return size_t 
            */
            static consteval size_t size() {
                return strlen(Name.value) + strlen(Value.value) + 4; // +4 for the space, =, and ""
            }


            /**
            * @brief The compile-time evaluation of the attribute string. Does not account for '\0';
            * The attribute string is formed as follows: ` name="value"`.
            * 
            * @return size_t 
            */
            static consteval std::array<char, size() + 1> serialize() {
                std::array<char, size() + 1> result = {};
                result[0] = ' ';
                int index = 1;
                for (int i = 0; i < strlen(Name.value); i++) {
                    result[index + i] = Name.value[i];
                }
                index += strlen(Name.value);
                result[index] = '=';
                result[index+1] = '\"';
                index += 2;
                for (int i = 0; i < strlen(Value.value); i++) {
                    result[index + i] = Value.value[i];
                }
                index += strlen(Value.value);
                result[index] = '\"';
                result[index+1] = '\0';
                return result;
            } 

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
            * @brief The compile-time size of the Text string. Does not account for '\0';
            * 
            * @return size_t 
            */
            static consteval size_t size() {
                return strlen(Str.value);
            }


            /**
             * @brief The Text string; evaluated at compile-time. Does not do any escaping.
             * 
             * @return std::array<char, size() + 1>
             */
            static consteval std::array<char, size() + 1> serialize() {
                std::array<char, size() + 1> result = {};
                for (int i = 0; i < strlen(Str.value); i++) {
                    result[i] = Str.value[i];
                }
                result[size()] = '\0';
                return result;
            } 

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

    template <size_t N, typename... Children>
    static consteval std::array<char, N + 1> serializeNode(const char* tagName) {
        std::array<char, N + 1> result = {};
        bool passedAttr = false;
        result[0] = '<';
        int index = 1;
        for (int i = 0; i < strlen(tagName); i++) {
            result[index + i] = tagName[i];
        }
        index += strlen(tagName);
        if constexpr (sizeof...(Children) == 0)
        {
            result[index] = '>';
            index++;
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
                for (int i = 0; i < Children::size(); i++) {
                    result[index + i] = in[i];
                }
                index += Children::size();
            }()), ...);
        }
        result[index] = '<';
        result[index+1] = '/';
        index += 2;
        for (int i = 0; i < strlen(tagName); i++) {
            result[index + i] = tagName[i];
        }
        index += strlen(tagName);
        result[index] = '>';
        result[index+1] = '\0';
        return result;
    } 

    template <size_t N, typename... Children>
    static consteval std::array<char, N + 1> serializeVoidNode(const char* tagName) {
        std::array<char, N + 1> result = {};
        bool passedAttr = false;
        result[0] = '<';
        int index = 1;
        for (int i = 0; i < strlen(tagName); i++) {
            result[index + i] = tagName[i];
        }
        index += strlen(tagName);
        if constexpr (sizeof...(Children) != 0)
        {
            (([&] {
                if constexpr(!Templater::compile::ctags::isAttribute<Children>) {
                    throw "Cannot add non-attribute child for void node.";

                }
                
                std::array<char, Children::size() + 1> in = Children::serialize();
                for (int i = 0; i < Children::size(); i++) {
                    result[index + i] = in[i];
                }
                index += Children::size();
            }()), ...);
        }
        result[index] = ' ';
        result[index+1] = '/';
        result[index+2] = '>';
        result[index+3] = '\0';

        return result;
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
            int index = 0;
            (([&] {
                if constexpr(Templater::compile::ctags::isAttribute<Children>) {
                    throw "Cannot add attribute as root node of Document.";
                }
                else {
                    std::array<char, Children::size() + 1> in = Children::serialize();
                    for (int i = 0; i < Children::size(); i++) {
                        result[index + i] = in[i];
                    }
                    index += Children::size();
                }
            }()), ...);
            result[index] = '\0';
            return result;
        } 

        /**
         * @brief Returns the serialized std::string from the templated arguments. Returns an empty string if there are no children. Calculated at runtime.
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
            return obj.serializePretty(indentationSequence, sortAttributes);
        }
    };

}