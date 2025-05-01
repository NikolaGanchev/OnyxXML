#include "node.h"
#include <concepts>
#include <cstddef>

namespace Templater::dynamic::dtags {

    /**
     * @brief  Checks if the type T can be used to call Node::addChild(), i.e., if T is a subclass of Node or std::unique_ptr<Node>.
     * 
     * @tparam T 
     */
    template<typename T>
    concept isValidNodeChild = std::same_as<T, std::unique_ptr<Node>> || std::derived_from<std::decay_t<T>, Node>;


    /**
     * @brief A Node that upon a condition decides which of two given Nodes to add as a child.
     * The child can be passed as either a move or an std::unique_ptr<Node>.
     * Example usage:
     * @code{.cpp}
     * // Node&& and Node&&
     * cdiv nodeTrue1{
     *     If(5 > 2, 
     *         Text("True"),
     *         Text("False"))
     * };
     * 
     * // Node&& and std::unique_ptr<Node> 
     * std::unique_ptr<Node> textTrue1 = std::make_unique<Text>("False");
     * cdiv nodeTrue2{
     *     If(5 > 2, 
     *         Text("True"),
     *         std::move(textTrue1))
     * };
     *  
     * // std::unique_ptr<Node> and Node&&
     * std::unique_ptr<Node> textTrue2 = std::make_unique<Text>("True");
     * cdiv nodeTrue3{
     *     If(5 > 2, 
     *         std::move(textTrue2),
     *         Text("False"))
     * };
     *  
     * // std::unique_ptr<Node> and std::unique_ptr<Node>
     * std::unique_ptr<Node> textTrue3 = std::make_unique<Text>("True");
     * std::unique_ptr<Node> textFalse3 = std::make_unique<Text>("True");
     * cdiv nodeTrue3{
     *     If(5 > 2, 
     *         std::move(textTrue3),
     *         std::move(textFalse3))
     * };
     * @endcode
     * 
     * All of the above serialize to the same output: @code{.xml}<div>True</div>@endcode
     */
    class If: public Node, public Node::SpecialSerializable {
        protected:
            void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const override;
            void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const override;
        public:
            If();

            /**
             * @brief Constructs an If Node
             * 
             * @tparam T type of the first Node - either Node subclass or std::unique_ptr<Node>
             * @tparam K type of the second Node - either Node subclass or std::unique_ptr<Node>
             * @param condition 
             * @param nodeTrue node if true
             * @param nodeFalse node if false
             */
            template <typename T, typename K>
            If(bool condition, T&& nodeTrue, K&& nodeFalse) requires (isValidNodeChild<T> && isValidNodeChild<K>);

            /**
             * @brief The tag name of an If node is the invalid xml tag name ".if". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            bool isVoid() const override;
            bool hasSpecialSerialization() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };

    template <typename T, typename K>
    If::If(bool condition, T&& nodeTrue, K&& nodeFalse) requires (isValidNodeChild<T> && isValidNodeChild<K>) {
        if (condition) {
            this->addChild(std::move(nodeTrue));
        } else {
            this->addChild(std::move(nodeFalse));
        }
    }

}
