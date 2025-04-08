#pragma once

#include "node.h"
#include <unordered_set>
#include <unordered_map>
#include <any>
#include <stdexcept>

namespace Templater::dynamic::index {

    /**
     * @brief Checks if the template parameter is a subclass of Index. 
     * 
     * @tparam T 
     */
    template <typename T>
    concept isIndex = std::derived_from<T, Index>;
    
    
    /**
     * @brief Creates an Index of type T using the provided args.
     * 
     * @tparam T 
     * @tparam Args 
     * @returns T The constructed Index
     */
    template <typename T, typename... Args>
    T createIndex(Args... args) requires (isIndex<T>); 


    /**
     * @brief Creates a raw pointer to an Index of type T using the provided args. It is the caller's responsibility to delete the pointer.
     * 
     * @tparam T 
     * @tparam Args 
     * @returns T* A pointer to the created Index
     */
    template <typename T, typename... Args>
    T* createIndexPointer(Args... args) requires (isIndex<T>);


    /**
     * @brief Creates a unique pointer to an Index of type T using the provided args and transfer ownership to the caller.
     * 
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, typename... Args>
    std::unique_ptr<T> createIndexUniquePointer(Args... args) requires (isIndex<T>);


    /**
     * @brief Creates a shared pointer to an Index of type T using the provided args and returns it to the user.
     * 
     * @tparam T 
     * @tparam Args 
     */
    template <typename T, typename... Args>
    std::shared_ptr<T> createIndexSharedPointer(Args... args) requires (isIndex<T>);



    /**
     * @brief An Index which keeps track of all nodes which have specific attribute name and is queried by values of that attribute.
     * The underlying index data structure is an std::unordered_map.
     * 
     */
    class AttributeNameIndex: public Index {
        private:
            /**
             * @brief The attribute name to index. Constant from creation.
             * 
             */
            const std::string attributeName;


            /**
             * @brief The index storage. 
             * The key is the attribute value, while the value is the std::vector of all indexed Nodes which have that attribute value.
             * 
             */
            std::unordered_map<std::string, std::vector<Node*>> index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            bool update(Node* node) override;


            /**
             * @brief Construct a new AttributeNameIndex object by a given root and attribute name which is copied.
             * 
             * @param root 
             * @param attributeName 
             */
            explicit AttributeNameIndex(Node* root, std::string& attributeName);


            /**
             * @brief Construct a new AttributeNameIndex object by a given root and attribute name which is moved.
             * 
             * @param root 
             * @param attributeName 
             */
            explicit AttributeNameIndex(Node* root, std::string&& attributeName);
        public:
            /**
             * @brief Get a const copy of the vector of all indexed Nodes which have the given attribute value.
             * Returns an empty vector if the Index is invalidated.
             * 
             * @param value 
             * @return const std::vector<Node*> 
             */
            const std::vector<Node*> getByValue(const std::string& value);
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };



    /**
     * @brief An Index which keeps track of all nodes with a specific tag name. The underlying index structure is an std::vector.
     * 
     */
    class TagNameIndex: public Index {
        private:
            /**
             * @brief The tag name to index. Constant from creation.
             * 
             */
            const std::string tagName;


            /**
             * @brief The data structure that holds all indexed Nodes with the given tag name.
             * 
             */
            std::vector<Node*> index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            

            /**
             * @brief In the general case, Nodes shouldn't change their tag name after creation.
             * However, in case of peculiar Node implementations, this method is fully implemented.
             * 
             * @param node 
             * @return true 
             * @return false 
             */
            bool update(Node* node) override;


            /**
             * @brief Construct a new TagNameIndex object by a given root and tag name which is copied.
             * 
             * @param root 
             * @param tagName 
             */
            explicit TagNameIndex(Node* root, std::string& tagName);

            
            /**
             * @brief Construct a new TagNameIndex object by a given root and tag name which is moved.
             * 
             * @param root 
             * @param tagName 
             */
            explicit TagNameIndex(Node* root, std::string&& tagName);
        public:
            /**
             * @brief Get a const copy of the vector of all indexed Nodes which have the given tag name.
             * Returns an empty vector if the Index is invalidated.
             * 
             * @return const std::vector<Node*> 
             */
            const std::vector<Node*> get();
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };


    /**
     * @brief An Index which indexes all Nodes in a tree by their tag name and provides constant access to a vector of Nodes with any given tag name.
     * The underlying index data structure is an std::unordered_map.
     * 
     */
    class TagIndex: public Index {
        private:
            /**
             * @brief The index storage. 
             * The key is the tag name, while the value is the std::vector of all indexed Nodes which have that tag name.
             */
            std::unordered_map<std::string, std::vector<Node*>> index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;


            /**
             * @brief In the general case, Nodes shouldn't change their tag name after creation.
             * However, in case of peculiar Node implementations, this method is fully implemented.
             * 
             * @param node 
             * @return true 
             * @return false 
             */
            bool update(Node* node) override;


            /**
             * @brief Construct a new TagIndex object by a given root.
             * 
             * @param root 
             */
            explicit TagIndex(Node* root);
        public:
            /**
             * @brief Get a const copy of the vector of all indexed Nodes which have the passed to the method tag name.
             * Returns an empty vector if the Index is invalidated.
             * 
             * @param tagName 
             * @return const std::vector<Node*> 
             */
            const std::vector<Node*> getByTagName(const std::string& tagName);
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };

    
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
             *  std::string serialised = index.cache(&GenericNode::serialise, "\t", true); // The serialisation is cached until the GenericObject is changed
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
}

template <typename T, typename... Args>
T Templater::dynamic::index::createIndex(Args... args) requires (isIndex<T>) {
    T index(std::forward<Args>(args)...);
    index.init();

    return index;
}

template <typename T, typename... Args>
T* Templater::dynamic::index::createIndexPointer(Args... args) requires (isIndex<T>) {
    T* index = new T(std::forward<Args>(args)...);
    index->init();

    return index;
}

template <typename T, typename... Args>
std::unique_ptr<T> Templater::dynamic::index::createIndexUniquePointer(Args... args) requires (isIndex<T>) {
    std::unique_ptr<T> index(new T(std::forward<Args>(args)...));
    index->init();

    return std::move(index);
}

template <typename T, typename... Args>
std::shared_ptr<T> Templater::dynamic::index::createIndexSharedPointer(Args... args) requires (isIndex<T>) {
    std::shared_ptr<T> index(new T(std::forward<Args>(args)...));
    index->init();

    return index;
}

template <typename Tuple, std::size_t... I>
std::size_t Templater::dynamic::index::CacheIndex::hashTuple(const Tuple& tuple, std::index_sequence<I...>) {
    std::size_t seed = 0;
    ((seed ^= std::hash<std::tuple_element_t<I, Tuple>>{}(std::get<I>(tuple))), ...);
    return seed;
}

template <typename Function, typename... Args>
std::size_t Templater::dynamic::index::CacheIndex::generateHash(Function f, Args&&... args) {
    std::tuple<std::decay_t<Args>...> argsTuple(std::forward<Args>(args)...);
    // *(void**)(void*)&f is equivalent to reinterpret_cast<void*>(f)
    std::size_t funcHash = std::hash<void*>{}(*(void**)(void*)&f);
    std::size_t argsHash = hashTuple(argsTuple, std::index_sequence_for<Args...>{});
    return funcHash ^ argsHash;
}

template <typename Function, typename... Args>
auto Templater::dynamic::index::CacheIndex::cache(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) {
    std::size_t hashKey = generateHash(f, args...);

    if (this->_cache.find(hashKey) != this->_cache.end()) {
        return std::any_cast<decltype((this->getRoot()->*f)(std::forward<Args>(args)...))>(this->_cache[hashKey]);
    }

    auto result = (this->getRoot()->*f)(std::forward<Args>(args)...);
    this->_cache[hashKey] = result;
    return result;
}

template <typename Function, typename... Args>
auto Templater::dynamic::index::CacheIndex::getCached(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) {
    std::size_t hashKey = generateHash(f, args...);

    if (this->_cache.find(hashKey) != this->_cache.end()) {
        return std::any_cast<decltype((this->getRoot()->*f)(std::forward<Args>(args)...))>(this->_cache[hashKey]);
    }

    throw std::runtime_error("Trying to get unavailable value from cache");
}

template <typename Function, typename... Args>
bool Templater::dynamic::index::CacheIndex::isCached(Function f, Args&&... args) {
    std::size_t hashKey = generateHash(f, args...);

    return this->_cache.find(hashKey) != this->_cache.end();
}