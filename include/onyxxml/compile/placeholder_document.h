#pragma once

#include "../document.h"
#include "compile_placeholder.h"

namespace onyx::compile {

/**
 * @brief Document struct adapted for dealing with template XML with
 * Placeholders. Document::serialize() and Document::dynamicTree() are replaced
 * with appropriate PlaceholderDocument::serializeWithPlaceholders() and
 * PlaceholderDocument::dynamicTreeWithPlaceholders().
 *
 * Example usage:
 * @code{.cpp}
 * using doc = PlaceholderDocument<
 *     html<
 *         head<>,
 *         body<
 *             Placeholder<"cd">,
 *             Placeholder<"ab">
 *         >
 *     >
 * >;
 *
 * tags::ul valueToBind{
 *     tags::li(tags::Text("1")),
 *     tags::li(tags::Text("2")),
 *     tags::li(tags::Text("3")),
 * };
 *
 *
 * tags::section valueToBind2{
 *     tags::article{
 *         tags::p{},
 *         tags::p{},
 *         tags::span{}
 *     }
 * };
 *
 * std::string res1 = doc::serializeWithPlaceholders("cd", valueToBind,
 *                                    "ab", valueToBind2);
 * std::string res2 = doc::dynamicTreeWithPlaceholders("cd", valueToBind,
 *                                    "ab", valueToBind2)->serialize();
 * @endcode
 * Both res1 and res2 in the above example are equal to:
 * @code{.xml}
 * <html><head></head><body><ul><li>1</li><li>2</li><li>3</li></ul><section><article><p></p><p></p><span></span></article></section></body></html>
 * @endcode
 *
 */
template <typename... Children>
struct PlaceholderDocument {
   private:
    /**
     * @brief The internal Document with the same children.
     *
     */
    using doc = Document<Children...>;

   public:
    /**
     * @brief Evaluate at runtime the output from the compile time
     * Document::serialize(). Searches for and replaces the Placeholder with a
     * given name with the serialization of the given Node.
     *
     * @tparam Rest
     * @param name Placeholder name
     * @param node The Node to use the serialization of
     * @param rest
     * @return std::string The evaluated string
     */
    template <typename... Rest>
    static std::string serializeWithPlaceholders(
        const char* name, const onyx::dynamic::Node& node, Rest&... rest) {
        static_assert(sizeof...(Rest) % 2 == 0,
                      "Arguments must be in key-value pairs.");
        std::string serialized = std::string(doc::serialize().data());
        evaluatePlaceholdersRec(serialized, name, node, rest...);
        return serialized;
    }

    /**
     * @brief Evaluate at runtime the dynamic tree with placeholders from
     * Document::dynamicTree(). Searches for and replaces the Placeholder Nodes
     * with a given name with the given node. Ownership of given nodes is taken
     * over.
     *
     * @tparam T
     * @tparam Rest
     * @param name Placeholder name
     * @param node The Node to insert in the tree
     * @param rest
     * @return std::unique_ptr<onyx::dynamic::Node>
     */
    template <typename T, typename... Rest>
    static std::unique_ptr<onyx::dynamic::Node>
    dynamicTreeWithPlaceholders(const char* name, T&& node, Rest&&... rest)
        requires(onyx::dynamic::isValidNodeChild<T>)
    {
        std::unique_ptr<onyx::dynamic::Node> obj = doc::dynamicTree();

        auto placeholderNodes =
            obj->getChildrenByTagName(ctags::Placeholder<"">::TAG_NAME);

        std::unordered_map<std::string, onyx::dynamic::Node*>
            placeholdersMap;

        for (auto& node : placeholderNodes) {
            placeholdersMap.emplace(node->getAttributeValue("name"), node);
        }

        evaluateDynamicTreeRec(obj.get(), placeholdersMap, name, node, rest...);

        return obj;
    }

   private:
    /**
     * @brief The recursion step for dynamic tree evaluation.
     *
     * @tparam T
     * @tparam Rest
     */
    template <typename T, typename... Rest>
    static void evaluateDynamicTreeRec(
        onyx::dynamic::Node* result,
        std::unordered_map<std::string, onyx::dynamic::Node*>&
            placeholders,
        const char* name, T&& node, Rest&&... rest)
        requires(onyx::dynamic::isValidNodeChild<T>)
    {
        evaluateDynamicTreePlaceholder(result, placeholders, name, node);

        evaluateDynamicTreeRec(result, placeholders, rest...);
    }

    /**
     * @brief The base recursion step for dynamic tree evaluation.
     *
     * @param result
     * @param placeholders
     */
    static void evaluateDynamicTreeRec(
        onyx::dynamic::Node* result,
        std::unordered_map<std::string, onyx::dynamic::Node*>&
            placeholders) {
        return;
    }

    /**
     * @brief Replaces a single dynamic tree Placeholder with the given Node.
     *
     * @tparam T
     */
    template <typename T>
    static void evaluateDynamicTreePlaceholder(
        onyx::dynamic::Node* result,
        std::unordered_map<std::string, onyx::dynamic::Node*>&
            placeholders,
        const char* name, T&& node)
        requires(onyx::dynamic::isValidNodeChild<T>)
    {
        if (placeholders.find(name) != placeholders.end()) {
            result->replaceChild(placeholders[name], std::move(node));
            placeholders.erase(name);
        } else {
            throw std::invalid_argument(std::string("No dynamic binding ") +
                                        name + " exists in map with size " +
                                        std::to_string(placeholders.size()) +
                                        placeholders.begin()->first +
                                        placeholders.end()->first);
        }
    }

    /**
     * @brief The recursion step for serialization evaluation.
     *
     * @tparam Rest
     * @param result
     * @param name
     * @param node
     * @param rest
     */
    template <typename... Rest>
    static void evaluatePlaceholdersRec(std::string& result, const char* name,
                                        const onyx::dynamic::Node& node,
                                        Rest&... rest) {
        evaluatePlaceholder(result, name, node);

        evaluatePlaceholdersRec(result, rest...);
    }

    /**
     * @brief The base step for serialization evaluation.
     *
     * @param result
     */
    static void evaluatePlaceholdersRec(std::string& result) { return; }

    /**
     * @brief Replace a single Placeholder of the serialization with the
     * serialization of the given Node.
     *
     * @param result
     * @param name
     * @param node
     */
    static void evaluatePlaceholder(std::string& result, const char* name,
                                    const onyx::dynamic::Node& node) {
        std::string search = std::string("<") +
                             ctags::Placeholder<"">::TAG_NAME + " name=\"" +
                             name + "\" />";
        int pos = result.find(search);
        if (pos == std::string::npos)
            throw std::invalid_argument(std::string("No dynamic binding ") +
                                        name + " exists");

        result.replace(pos, search.size(), node.serialize());
    }
};
}  // namespace onyx::compile