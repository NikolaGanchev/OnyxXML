#include "node.h"
#include <concepts>
#include <cstddef>

namespace Templater::dynamic::dtags {


    /**
     * @brief Checks if the type T is index iterable. An index iterable type is a container which has a ::size() method and a [] access operator.
     * 
     * @tparam T 
     */
    template<typename T>
    concept isIndexIterable = requires(T t) {
        { t.size() } -> std::convertible_to<std::size_t>;  // The type must have a size() method
        { t[0] } -> std::same_as<typename T::value_type&>;  // The type must support indexing
    };

    /**
     * @brief A ForEach Node meant to simplify creating multiple sequential Nodes based on an iterable type.
     * Example usage:
     * @code{.cpp}
     * ul forEachConstructed{
     *      ForEach{std::array<int, 5>{1, 2, 3, 4, 5}, [](int index, const std::array<int, 5>& container) {
     *          return li(Text(std::to_string(container[index])));
     *      }}
     * };
     * 
     * forEachConstructed.serialize() returns:
     * <ul>
     *  <li>0</li>
     *  <li>1</li>
     *  <li>2</li>
     *  <li>3</li>
     *  <li>4</li>
     * </ul>
     *
     * @endcode
     * 
     */
    class ForEach: public Node, public Node::SpecialSerializable {
        protected:
            void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const override;
            void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const override;
        public:
            /**
             * @brief Construct a new ForEach object without any Nodes
             * 
             */
            ForEach();


            /**
             * @brief Construct a ForEach object via an index iterable container. An index iterable container is a container which has a ::size() method and a [] access operator.
             * The producer function can either return unique_ptr<Node> (if returning different Node subclasses) or any Node subclass (if returning only it)
             * Example usage with different node types:
             * @code{.cpp}
             * cdiv forEachConstructed{
             *  ForEach{std::array<int, 5>{1, 2, 3, 4, 5}, [](int index, const std::array<int, 5>& container) -> std::unique_ptr<Node> {
             *      if (index % 2 == 0) {
             *          return std::make_unique<cdiv>(Text(std::to_string(container[index])));
             *      } else {
             *          return std::make_unique<section>(Text(std::to_string(container[index])));
             *      }
             *  }}
             * };
             * @endcode
             * Note that the return type of std::unique_ptr<Node> needs to be explicitly declared in this case.
             * 
             * Example usage with the same node type:
             * @code{.cpp}
             * ul forEachConstructed{
             *      ForEach{std::array<int, 5>{1, 2, 3, 4, 5}, [](int index, const std::array<int, 5>& container) {
             *          return li(Text(std::to_string(container[index])));
             *      }}
             * };
             * @endcode
             * In the second example, the Node is moved which constrains the type.
             * 
             * @tparam T index iterable container
             * @tparam Func std::function<ReturnType(int index, const T& container)> where ReturnType can be either unique_ptr<Node> (if returning different Node subclasses) or any Node subclass (if returning only it)
             * @param container
             * @param producer 
             */
            template<typename T, typename Func>
            ForEach(const T& container, Func producer) requires(isIndexIterable<T>);
        

            /**
             * @brief The tag name of an empty node is the invalid xml tag name ".foreach". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            bool isVoid() const override;
            bool hasSpecialSerialization() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
    

    template<typename T, typename Func>
    ForEach::ForEach(const T& container, Func producer) requires(isIndexIterable<T>) {
        using ReturnType = std::invoke_result_t<Func, int, const T&>;

        std::function<ReturnType(int, const T&)> producer_;
        producer_ = std::forward<Func>(producer);

        for (size_t i = 0; i < container.size(); i++) {
            this->addChild(std::move(producer_(i, container)));
        }
    }
}
