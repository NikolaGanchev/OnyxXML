#include <string>
#include <iostream>
#include <algorithm>
#include <string_view>
#include "html_object.h"

namespace Templater::html {

    template <size_t N>
    struct CompileString {
        char value[N];

        constexpr CompileString(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }

        constexpr std::string to_string() const {
            return std::string(value, N - 1);
        }
    
        constexpr operator std::string() const {
            return to_string();
        }
    };

    constexpr std::string operator+(std::string_view lhs, std::string_view rhs) {
        return std::string(lhs) + std::string(rhs);
    }

    namespace ctags {

        template <CompileString Name, CompileString Value>
        struct Attribute {
            static constexpr std::shared_ptr<Templater::html::Attribute> attr() {
                return std::make_shared<Templater::html::Attribute>(Name, Value);
            }
        };

        template <typename T>
        concept isAttribute = requires(T) {
            { T::attr() } -> std::same_as<std::shared_ptr<Templater::html::Attribute>>;
        };

        template <CompileString Str>
        struct Text {
            static constexpr std::shared_ptr<Object> value() {
                return Templater::html::Text(Str).clone();
            }
        };

        template <typename Child>
        constexpr void parseChildren(Object& node) {
            if constexpr(isAttribute<Child>) {
                std::shared_ptr<Templater::html::Attribute> attr = Child::attr();
                node[attr->getName()] = attr->getValue();
            }
            else {
                std::shared_ptr<Object> attr = Child::value();
                node.addChild(*Child::value());
            }
        }
    }

    template <typename... Children>
    struct Document {
        static constexpr std::string value(const std::string& identationSequence = Object::getIdentationSequence(), bool sortAttributes = Object::getSortAttributes()) {
            EmptyTag obj;
            if constexpr (sizeof...(Children) == 0)
            {
                return std::string("");
            }
            else
            {
                (([&] {
                    if constexpr(Templater::html::ctags::isAttribute<Children>) {
                        return "Error. Trying to read attribute as root node";
                    }
                    else {
                        obj.addChild(*Children::value());
                    }
                }()), ...);
            }
            return obj.serialise(identationSequence, sortAttributes);
        }
    };

}