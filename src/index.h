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

    #define BEFRIEND_INDEX_CREATOR_FUNCTIONS template <typename T, typename... Args>\
                                        friend T Templater::dynamic::index::createIndex(Args... args) requires (isIndex<T>);\
                                        template <typename T, typename... Args>\
                                        friend T* Templater::dynamic::index::createIndexPointer(Args... args) requires (isIndex<T>);\
                                        template <typename T, typename... Args>\
                                        friend std::unique_ptr<T> Templater::dynamic::index::createIndexUniquePointer(Args... args) requires (isIndex<T>);\
                                        template <typename T, typename... Args>\
                                        friend std::shared_ptr<T> Templater::dynamic::index::createIndexSharedPointer(Args... args) requires (isIndex<T>);


    #define ADD_INDEX_MOVE_OPERATIONS(Access, ClassName, ...)\
        private:\
            template<typename... MemberPtrs>\
            void _add_index_move_members_helper(ClassName&& other, MemberPtrs... mptrs) noexcept {\
                ((this->*mptrs = std::move(other.*mptrs)), ...);\
            };\
        Access:\
            ClassName(ClassName&& other) noexcept: Index{std::move(other)} {\
                _add_index_move_members_helper(std::move(other), __VA_ARGS__);\
            };\
            ClassName& operator=(ClassName&& other) noexcept {\
                if (this == &other) return *this;\
                this->_assign_index_base(std::move(other));\
                _add_index_move_members_helper(std::move(other), __VA_ARGS__);\
                return *this;\
            };

        /**
     * @brief Indexes are non-owning indexings over a Node. 
     * A node with an Index also holds a non owning pointer to it. 
     * The index, with other words, is solely owned by the creator.
     * Upon destruction of a Node, it invalidates its own indices. 
     * Invalidated indices must not return any values if queried.
     * At destruction, the index, if valid, removes itself from the root node it is applied to.
     * As a rule, nodes do not query indices. An Index should have methods to return any required info it indexes.
     * The Index class is responsible for root keeping, invalidation and destruction. 
     * 
     * The Index class constructor is protected. Indices are to be created using "creator" functions, which are templated functions for instantiation of indices.
     * To use them, any subclass of Index should in its definition write the macro BEFRIEND_INDEX_CREATOR_FUNCTIONS; to add creators.
     * This macro expands to the relevant friend declarations.
     * Example usage of creators: 
     * @code{}
     *  index::AttributeNameIndex index = index::createIndex<index::AttributeNameIndex>(&obj, "id");
     * @endcode{}
     * 
     * Indices are non-copyable but movable. Each subclass must provide its own move constructor and move assignment operator.
     * Using the provided creators requires the existence of move constructors/assignment. Otherwise the code won't compile.
     * For convenience, the ADD_INDEX_MOVE_OPERATIONS(Access, ClassName, ...) macro is available. It requires the access (public/private/protected)
     * of the move operations, the class name of the Index and the references to any additional variables the subclass adds. 
     * These variables are default moved using std::move(). To enable this, a private _add_index_move_members_helper() method is added to the class.
     * Example of move operations adding:
     * @code{.cpp}
     * public:
     *  ADD_INDEX_MOVE_OPERATIONS(public, TagNameIndex, &TagNameIndex::tagName, &TagNameIndex::index);
     * @endcode
     * 
     * If any special move behaviour is required, the subclass should implement a custom function. For ease of use, the protected methods 
     * Index::_assign_index_base(Index&& other) and Index::Index(Index&& other) can be called by children to sort out the base class' move behaviour. 
     * It is expected that upon moving, the new object does not rebuild the index but takes it. The above referred functions internally switch the old
     * Index* to the new Index* in all relevant nodes. The object being moved should be left in invalid state, ie Index::isValid() should return false.
     * 
     * Subclasses are responsible for providing their own implementations of storage and getters, as well as decide when the Index should be updated.
     * Any index queries are, unless specified otherwise, only valid from the viewpoint of the root Node.
     * 
     */
    class Index {
        friend Node;
        private:
            /**
             * @brief The Node the Index was initialised with
             * 
             */
            Node* root;
            /**
             * @brief Whether the Index is valid or not
             * 
             */
            bool valid;
        protected:
            /**
             * @brief Indexes the given Node if it satisfies the conditions for indexing and is not already in the index.
             * Always called when a child is being added to a Node. 
             * 
             * @param object 
             * @return true The Node was indexed on this call
             * @return false The Node doesn't need to be indexed or is already in the index.
             */
            virtual bool putIfNeeded(Node* object) = 0;


            /**
             * @brief Removes the given Node if it is in the index.
             * Always called when a child is being removed from a Node, or a Node is being deconstructed. 
             * 
             * @param object 
             * @return true The Node was removed
             * @return false The Node wasn't removed
             */
            virtual bool removeIfNeeded(Node* object) = 0;


            /**
             * @brief Updates the given Node. The Node may be added or removed from the index, or its indexing changed.
             * Nodes have to call this anytime the node's properties are mutated. The update() function should satisfy the following:
             * 1. If the given Node is already in the expected position in the index, nothing is done.
             * 2. Else, update() is equivalent to calling removeIfNeeded() and putIfNeeded(). 
             * This may or may not change the index, but the update() function can choose to return true in both cases.
             * 
             * @param object 
             * @return true The Node's indexing could have been updated but is not guaranteed to have been
             * @return false The Node wasn't updated
             */
            virtual bool update(Node* object) = 0;


            /**
             * @brief Construct a new Index object. Indices are empty on construction and to index the given root, init() needs to be called.
             * 
             * @param root 
             */
            explicit Index(Node* root);

            
            /**
             * @brief Invalidates the Index. Invalidated indices do not return anything to queries. What empty value is returned is determined by subclasses.
             * Querying an invalid index should not cause any runtime errors/exceptions.
             * 
             */
            void invalidate();

            /**
             * @brief Destroy the Index object. Removes the index from the root Node and all its children.
             * 
             */
            void destroy();

            /**
             * @brief Initialises the index. Adds the index to the root Node and its children and fills the internal index with data.
             * 
             */
            void init();


            /**
             * @brief Moves other into this assuming this did not previously exist
             * 
             * @param other 
             */
            void _move_index_base(Index&& other);


            /**
             * @brief Assigns other into this
             * 
             * @param other 
             */
            void _assign_index_base(Index&& other);
            

            /**
             * @brief Construct a new Index object via move
             * 
             * @param other 
             */
            Index(Index&& other);
        public:
            /**
             * @brief Get the Root Node.
             * 
             * @return const Node* 
             */
            const Node* getRoot() const;


            /**
             * @brief Whether the Index is valid. Queries on invalid indices should return empty values, but not cause exceptions.
             * 
             * @return true 
             * @return false 
             */
            bool isValid() const;


            /**
             * @brief Destroy the Index object. Removes the index from the root Node and all its children.
             * 
             */
            virtual ~Index();

            Index(const Index&) = delete;
            Index& operator=(const Index&) = delete;
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