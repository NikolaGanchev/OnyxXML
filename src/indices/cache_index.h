#include "index.h"

namespace Templater::dynamic::index {
    
    /**
     * @brief An index which tracks whether the tree it has been given has been changed.
     * A function of the root and its arguments can be passed to the cache() method and the result will be cached or returned from the cache.
     * If the tree changes in any way, this Index will invalidate its inner cache and delete all entries inside it.
     * The underlying data structure is an std::unordered_map.
     */
    class CacheIndex: public Index {
        private:
            /**
             * @brief The std::unordered_map holding the cache. 
             * The key is a hash derived from the function pointer and the specific arguments. 
             * The value is the value that the function returned on its initial call.
             * 
             */
            std::unordered_map<std::size_t, std::any> _cache;

            /**
             * @brief Generates a hash based on a tuple using templates.
             * Given a variadic list of arguments, this function will build a tuple from them and a hash from it.
             * 
             * @tparam Tuple 
             * @tparam I 
             * @param tuple 
             * @return std::size_t 
             */
            template <typename Tuple, std::size_t... I>
            static std::size_t hashTuple(const Tuple& tuple, std::index_sequence<I...>);


            /**
             * @brief This function takes a function pointer and a variadic list of arguments and generates a unique hash based on the function pointer value and the arguments.
             * It uses hashTuple(const Tuple&, std::index_sequence<I...>) in the process
             * 
             * @param f 
             * @param args 
             * @return template <typename Function, typename... Args> 
             */
            template <typename Function, typename... Args>
            static std::size_t generateHash(Function f, Args&&... args);

        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            bool update(Node* node) override;

            /**
             * @brief Construct a new CacheIndex object by a given root.
             * 
             * @param root 
             */
            explicit CacheIndex(Node* root);
        public:
            /**
             * @brief Given a function and arguments, this function generates a hash for the function and the specific arguments.
             * Using this hash, it checks its internal cache for a result of a previous call with the same hash.
             * If no such call is found, the function calls the passed function onto the root (generating a compile-time error if that is impossible),
             * caching the result and returning it to the user. Does not handle hash collisions.
             * 
             * For example: 
             * @code{.cpp}
             *  using namespace Templater::dynamic::dtags;
             *
             *  GenericNode obj{
             *      "html", false,
             *      Attribute("lang", "en"),
             *      Attribute("theme", "dark"),
             *      GenericNode("head", false)
             *  };
             *
             *  index::CacheIndex index = index::createIndex<index::CacheIndex>(&obj);
             * 
             *  std::string serialized = index.cache(&GenericNode::serializePretty, "\t", true); // The serialization is cached until the GenericObject is changed
             * @endcode 
             * 
             * @tparam Function 
             * @tparam Args 
             * @param f The function reference-
             * @param args 
             * @return decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) 
             */
            template <typename Function, typename... Args>
            auto cache(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...));


            /**
             * @brief Like cache(), but throws a runtime error if the call is not in the cache.
             * Given a function and arguments, this function generates a hash for the function and the specific arguments.
             * Using this hash, it checks its internal cache for a result of a previous call with the same hash.
             * If no such call is found, the function throws a runtime error.
             * 
             * @tparam Function 
             * @tparam Args 
             * @param f 
             * @param args 
             * @return decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) 
             */
            template <typename Function, typename... Args>
            auto getCached(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...));


            /**
             * @brief Given a function and arguments, this function generates a hash for the function and the specific arguments.
             * Using this hash, it checks its internal cache for a result of a previous call with the same hash and returns whether there is one.
             * 
             * @tparam Function 
             * @tparam Args 
             * @param f 
             * @param args 
             * @return true The call is in the cache
             * @return false The call is not in the cache
             */
            template <typename Function, typename... Args>
            bool isCached(Function f, Args&&... args);
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };


    template <typename Tuple, std::size_t... I>
    std::size_t CacheIndex::hashTuple(const Tuple& tuple, std::index_sequence<I...>) {
        std::size_t seed = 0;
        ((seed ^= std::hash<std::tuple_element_t<I, Tuple>>{}(std::get<I>(tuple))), ...);
        return seed;
    }

    template <typename Function, typename... Args>
    std::size_t CacheIndex::generateHash(Function f, Args&&... args) {
        std::tuple<std::decay_t<Args>...> argsTuple(std::forward<Args>(args)...);
        // *(void**)(void*)&f is equivalent to reinterpret_cast<void*>(f)
        std::size_t funcHash = std::hash<void*>{}(*(void**)(void*)&f);
        std::size_t argsHash = hashTuple(argsTuple, std::index_sequence_for<Args...>{});
        return funcHash ^ argsHash;
    }

    template <typename Function, typename... Args>
    auto CacheIndex::cache(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) {
        std::size_t hashKey = generateHash(f, args...);

        if (this->_cache.find(hashKey) != this->_cache.end()) {
            return std::any_cast<decltype((this->getRoot()->*f)(std::forward<Args>(args)...))>(this->_cache[hashKey]);
        }

        auto result = (this->getRoot()->*f)(std::forward<Args>(args)...);
        this->_cache[hashKey] = result;
        return result;
    }

    template <typename Function, typename... Args>
    auto CacheIndex::getCached(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) {
        std::size_t hashKey = generateHash(f, args...);

        if (this->_cache.find(hashKey) != this->_cache.end()) {
            return std::any_cast<decltype((this->getRoot()->*f)(std::forward<Args>(args)...))>(this->_cache[hashKey]);
        }

        throw std::runtime_error("Trying to get unavailable value from cache");
    }

    template <typename Function, typename... Args>
    bool CacheIndex::isCached(Function f, Args&&... args) {
        std::size_t hashKey = generateHash(f, args...);

        return this->_cache.find(hashKey) != this->_cache.end();
    }
}