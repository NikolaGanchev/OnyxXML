#pragma once

#include <string>
#include <algorithm>
#include <string_view>
#include "html_object.h"

namespace Templater::compile {

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
            static constexpr std::shared_ptr<Templater::dynamic::Attribute> attr() {
                return std::make_shared<Templater::dynamic::Attribute>(Name, Value);
            }
        };

        template <typename T>
        concept isAttribute = requires(T) {
            { T::attr() } -> std::same_as<std::shared_ptr<Templater::dynamic::Attribute>>;
        };

        template <CompileString Str>
        struct Text {
            static constexpr std::shared_ptr<Templater::dynamic::Object> value() {
                return Templater::dynamic::dtags::Text(Str).clone();
            }
        };

        template <typename Child>
        constexpr void parseChildren(Templater::dynamic::Object& node) {
            if constexpr(isAttribute<Child>) {
                std::shared_ptr<Templater::dynamic::Attribute> attr = Child::attr();
                node[attr->getName()] = attr->getValue();
            }
            else {
                node.addChild(*Child::value());
            }
        }
    }

    template <typename... Children>
    struct Document {
        static constexpr std::string value(
            const std::string& identationSequence = Templater::dynamic::Object::getIdentationSequence(), 
            bool sortAttributes = Templater::dynamic::Object::getSortAttributes()) {
            Templater::dynamic::dtags::EmptyTag obj;
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
                        obj.addChild(*Children::value());
                    }
                }()), ...);
            }
            return obj.serialise(identationSequence, sortAttributes);
        }
    };

}