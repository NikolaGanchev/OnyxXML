#pragma once

#include "compile_string.h"
#include "compile_string_utils.h"
#include "document_utils.h"
#include <string>
#include <string_view>
#include <array>
#include <concepts>
#include "../nodes/empty_node.h"
#include "../nodes/generic_node.h"

namespace Templater::compile::ctags {

    /**
     * @brief A compile time Dynamic binding struct.
     * Used to to embed "placeholders" in Document that later need to be filled.
     * 
     * 
     * @tparam Str
     */
    template <CompileString Str>
    struct Placeholder {
        private:
            using nameAttribute = Attribute<"name", Str>;
        public:
        static constexpr const char* TAG_NAME = ".templater::placeholder";
        /**
        * @brief The compile-time size of the Comment string. Does not account for '\0';
        * 
        * @return size_t 
        */
        static consteval size_t size() {
            return Placeholder::nameAttribute::size() + std::string_view(TAG_NAME).size() + 4;
        }


        /**
         * @brief The Comment string; evaluated at compile-time. Does not do any escaping.
         * 
         * @return std::array<char, size() + 1>
         */
        static consteval std::array<char, size() + 1> serialize() {
            return DocumentUtils::serializeVoidNode<size(), Attribute<"name", Str>>(TAG_NAME);
        }

        /**
         * @brief Construct a dynamic Comment Node from a compile time Comment struct.
         * 
         * @return std::unique_ptr<Templater::dynamic::Node> 
         */
        static std::unique_ptr<Templater::dynamic::Node> dynamicTree() {
            return std::make_unique<Templater::dynamic::dtags::GenericNode>(TAG_NAME, true, 
                Templater::dynamic::Attribute("name", std::string(Str)));
        }
    };
}