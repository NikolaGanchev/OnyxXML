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
     * @brief A compile time Placeholder struct.
     * Used to mark places in a template document where later dynamic content should be inserted.
     * If using Placeholder in a Document, PlaceholderDocument should instead be used as it provides the methods to properly insert the dynamic content.
     * Placeholder strings should be unique. If they aren't unique and the user properly provides the non-unique string twice when evaluating,
     * evaluation will probably succeed, but is not guaranteed to.
     * Note: If you use Placeholders with a regular Document, no exceptions will be thrown. Exercise caution.
     * 
     * @tparam Str id
     */
    template <CompileString Str>
    struct Placeholder {
        private:
            /**
             * @brief The compile time attribute the Placeholder will inject: name="<string>".
             * 
             */
            using nameAttribute = Attribute<"name", Str>;
        public:
        /**
         * @brief The invalid tag name ".templater::placeholder" is used to reduce chances of collisions.
         * 
         */
        static constexpr const char* TAG_NAME = ".templater::placeholder";


        /**
        * @brief The compile-time size of the Placeholder. Does not account for '\0';
        * 
        * @return size_t 
        */
        static consteval size_t size() {
            return Placeholder::nameAttribute::size() + std::string_view(TAG_NAME).size() + 4;
        }


        /**
         * @brief The Placeholder, evaluated at compile time. Of the form <.templater::placeholder name="<string>" />
         * 
         * @return std::array<char, size() + 1>
         */
        static consteval std::array<char, size() + 1> serialize() {
            return DocumentUtils::serializeVoidNode<size(), Attribute<"name", Str>>(TAG_NAME);
        }

        /**
         * @brief Construct a dynamic Placeholder Node from a compile time Placeholder struct.\
         * Generates a void GenericNode with TAG_NAME and the Attribute name="<string>".
         * 
         * @return std::unique_ptr<Templater::dynamic::Node> 
         */
        static std::unique_ptr<Templater::dynamic::Node> dynamicTree() {
            return std::make_unique<Templater::dynamic::dtags::GenericNode>(TAG_NAME, true, 
                Templater::dynamic::Attribute("name", std::string(Str)));
        }
    };
}