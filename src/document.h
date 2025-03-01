#include <string>
#include <iostream>
#include <algorithm>
#include <string_view>

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
            static constexpr std::string attr() {
                return " " + std::string(Name) + "=\"" + std::string(Value) + "\"";
            }
        };

        template <typename T>
        concept isAttribute = requires(T) {
            { T::attr() } -> std::same_as<std::string>;
        };

        template <CompileString Str>
        struct Text {
            static constexpr std::string value(int indent_level = 0) {
                constexpr const char indent_char = '\t';
                std::string indent(indent_level, indent_char);
                return indent + std::string(Str);
            }
        };

        template <typename... Children>
        struct html {
            static constexpr std::string value(int indent_level = 0) {
                constexpr const char indent_char = '\t';
                std::string indent(indent_level, indent_char);
                if constexpr (sizeof...(Children) == 0)
                {
                    return std::string("<html>\n</html>\n");
                }
                else
                {   
                    std::string attributes;
                    std::string children_content;

                    (([&] {
                        if constexpr(isAttribute<Children>) {
                            attributes += Children::attr();
                        }
                        else {
                            children_content += Children::value(indent_level + 1);
                        }
                    }()), ...);

                    return indent + std::string("<html") + attributes + ">" + "\n" + children_content + "\n" + indent + std::string("</html>\n");
                }
            }
        };

        template <typename... Children>
        struct head {
            static constexpr std::string value(int indent_level = 0) {
                constexpr const char indent_char = '\t';
                std::string indent(indent_level, indent_char);
                if constexpr (sizeof...(Children) == 0)
                {
                    return std::string("<head>\n</head>\n");
                }
                else
                {   
                    std::string attributes;
                    std::string children_content;

                    (([&] {
                        if constexpr(isAttribute<Children>) {
                            attributes += Children::attr();
                        }
                        else {
                            children_content += Children::value(indent_level + 1);
                        }
                    }()), ...);

                    return indent + std::string("<head") + attributes + ">" + "\n" + children_content + "\n" + indent + std::string("</head>\n");
                }
            }
        };

        template <typename... Children>
        struct body {
            static constexpr std::string value(int indent_level = 0) {
                constexpr const char indent_char = '\t';
                std::string indent(indent_level, indent_char);
                if constexpr (sizeof...(Children) == 0)
                {
                    return std::string("<body>\n</body>\n");
                }
                else
                {   
                    std::string attributes;
                    std::string children_content;

                    (([&] {
                        if constexpr(isAttribute<Children>) {
                            attributes += Children::attr();
                        }
                        else {
                            children_content += Children::value(indent_level + 1);
                        }
                    }()), ...);

                    return indent + std::string("<body") + attributes + ">" + "\n" + children_content + "\n" + indent + std::string("</body>\n");
                }
            }
        };
    }

    template <typename... Children>
    struct Document {
        static constexpr std::string value() {
            std::string res;
            if constexpr (sizeof...(Children) == 0)
            {
                return std::string("");
            }
            else
            {
                res = (... + Children::value());
            }
            if (res[res.size() - 1] == '\n') res.pop_back();
            return res;
        }
    };

}