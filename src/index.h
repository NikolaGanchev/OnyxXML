#pragma once

#include "node.h"
#include <unordered_set>
#include <unordered_map>
#include <any>

namespace Templater::dynamic::index {

    #define BEFRIEND_INDEX_CREATOR_FUNCTIONS template <typename T, typename... Args>\
                                            friend T Templater::dynamic::index::createIndex(Args... args) requires (isIndex<T>);\
                                            template <typename T, typename... Args>\
                                            friend T* Templater::dynamic::index::createIndexPointer(Args... args) requires (isIndex<T>);\
                                            template <typename T, typename... Args>\
                                            friend std::unique_ptr<T> Templater::dynamic::index::createIndexUniquePointer(Args... args) requires (isIndex<T>);\
                                            template <typename T, typename... Args>\
                                            friend std::shared_ptr<T> Templater::dynamic::index::createIndexSharedPointer(Args... args) requires (isIndex<T>);

    template <typename T>
    concept isIndex = std::derived_from<T, Index>;
    
    template <typename T, typename... Args>
    T createIndex(Args... args) requires (isIndex<T>); 

    template <typename T, typename... Args>
    T* createIndexPointer(Args... args) requires (isIndex<T>);

    template <typename T, typename... Args>
    std::unique_ptr<T> createIndexUniquePointer(Args... args) requires (isIndex<T>);

    template <typename T, typename... Args>
    std::shared_ptr<T> createIndexSharedPointer(Args... args) requires (isIndex<T>);

    // Indexes a single attribute name
    class AttributeNameIndex: public Index {
        private:
            const std::string m_attributeName;
            std::unordered_map<std::string, std::vector<Node*>> m_index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            bool update(Node* node) override;
            explicit AttributeNameIndex(Node* root, std::string& attributeName);
            explicit AttributeNameIndex(Node* root, std::string&& attributeName);
        public:
            const std::vector<Node*> getByValue(const std::string& value);
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };

    // Indexes a single tag name
    class TagNameIndex: public Index {
        private:
            const std::string m_tagName;
            std::vector<Node*> m_index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            bool update(Node* node) override;
            explicit TagNameIndex(Node* root, std::string& tagName);
            explicit TagNameIndex(Node* root, std::string&& tagName);
        public:
            const std::vector<Node*> get();
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };

    // Indexes all tags in a tree
    class TagIndex: public Index {
        private:
            std::unordered_map<std::string, std::vector<Node*>> m_index;
        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            bool update(Node* node) override;
            explicit TagIndex(Node* root);
        public:
            const std::vector<Node*> getByTagName(const std::string& tagName);
        
        BEFRIEND_INDEX_CREATOR_FUNCTIONS;
    };

    // Indexes all tags in a tree
    class CacheIndex: public Index {
        private:
            std::unordered_map<std::size_t, std::any> m_cache;

            template <typename Tuple, std::size_t... I>
            static std::size_t hashTuple(const Tuple& tuple, std::index_sequence<I...>);

            // Generate a unique hash from a member function pointer and its arguments
            template <typename Function, typename... Args>
            static std::size_t generateHash(Function f, Args&&... args);

        protected:
            bool putIfNeeded(Node* node) override;
            bool removeIfNeeded(Node* node) override;
            bool update(Node* node) override;
            explicit CacheIndex(Node* root);
        public:
            template <typename Function, typename... Args>
            auto cache(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...));

            template <typename Function, typename... Args>
            auto getCached(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...));

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
    std::size_t funcHash = std::hash<void*>{}(reinterpret_cast<void*>(f));
    std::size_t argsHash = hashTuple(argsTuple, std::index_sequence_for<Args...>{});
    return funcHash ^ argsHash;
}

template <typename Function, typename... Args>
auto Templater::dynamic::index::CacheIndex::cache(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) {
    std::size_t hashKey = generateHash(f, args...);

    if (m_cache.find(hashKey) != m_cache.end()) {
        return std::any_cast<decltype((this->getRoot()->*f)(std::forward<Args>(args)...))>(m_cache[hashKey]);
    }

    auto result = (this->getRoot()->*f)(std::forward<Args>(args)...);
    m_cache[hashKey] = result;
    return result;
}

template <typename Function, typename... Args>
auto Templater::dynamic::index::CacheIndex::getCached(Function f, Args&&... args) -> decltype((this->getRoot()->*f)(std::forward<Args>(args)...)) {
    std::size_t hashKey = generateHash(f, args...);

    if (m_cache.find(hashKey) != m_cache.end()) {
        return std::any_cast<decltype((this->getRoot()->*f)(std::forward<Args>(args)...))>(m_cache[hashKey]);
    }

    throw std::runtime_error("Trying to get unavailable value from cache");
}

template <typename Function, typename... Args>
bool Templater::dynamic::index::CacheIndex::isCached(Function f, Args&&... args) {
    std::size_t hashKey = generateHash(f, args...);

    return m_cache.find(hashKey) != m_cache.end();
}