#pragma once

#include <array>
#include <string>
#include <string_view>

#include "../nodes/xml_declaration_node.h"
#include "compile_string.h"
#include "compile_string_utils.h"

namespace onyx::compile::ctags {
/**
 * @brief A compile-time XmlDeclaration struct.
 *
 * @tparam Str
 */
template <CompileString Version, CompileString Encoding,
          CompileString Standalone>
struct XmlDeclaration {
    /**
     * @brief The compile-time size of the XmlDeclaration string. Does
     * not account for
     * '\0'
     *
     * @return size_t
     */
    static consteval size_t size() {
        return 6 + std::string_view(Version.value).size() + 11 +
               std::string_view(Encoding.value).size() + 12 +
               std::string_view(Standalone.value).size() + 13 +
               2;  // added size for '<?xml ' (6), 'version="" ' (11),
                   // 'encoding="" ' (12), 'standalone=""' (10) and ?> (2)
    }

    static consteval std::size_t placeholderCount() { return 0; }

    /**
     * @brief The XmlDeclaration string; evaluated at compile-time. Does
     * not do any escaping.
     *
     * @return std::array<char, size() + 1>
     */
    static consteval EvaluatedDocument<size() + 1, 0> serialize() {
        EvaluatedDocument<size() + 1, 0> result = {};
        evaluate(result);
        return result;
    }

    /**
     * @brief Evaluates the XmlDeclaration into an existing EvaluatedDocument
     *
     */
    template <std::size_t ContentSize, std::size_t PlaceholderCount>
    static consteval void evaluate(
        EvaluatedDocument<ContentSize, PlaceholderCount>& result) {
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           "<?xml version=\"");
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           Version.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           "\" encoding=\"");
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           Encoding.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           "\" standalone=\"");
        CompileStringUtils::placeStringInEvaluatedDocument(result,
                                                           Standalone.value);
        CompileStringUtils::placeStringInEvaluatedDocument(result, "\"?>");
        CompileStringUtils::placeStringInEvaluatedDocument(result, "\0");
    }

    /**
     * @brief Construct a dynamic XmlDeclaration Node from a compile-time
     * XmlDeclaration struct.
     *
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    static std::unique_ptr<onyx::dynamic::Node> dynamicTree() {
        return std::make_unique<onyx::dynamic::tags::XmlDeclaration>(
            Version, Encoding, std::string_view(Standalone.value) == "yes");
    }
};
}  // namespace onyx::compile::ctags