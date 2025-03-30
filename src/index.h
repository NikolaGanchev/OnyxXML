#pragma once

#include "object.h"
#include <unordered_set>
#include <unordered_map>

namespace Templater::dynamic::index {

    #define BEFRIEND_INDEX_CREATOR_FUNCTION template <typename T, typename... Args>\
                                            friend T createIndex(Args... args) requires (isIndex<T>);

    template <typename T>
    concept isIndex = std::derived_from<T, Index>;
    
    template <typename T, typename... Args>
    T createIndex(Args... args) requires (isIndex<T>);

    class AttributeNameIndex: public Index {
        private:
            const std::string m_attributeName;
            std::unordered_map<std::string, std::vector<Object*>> m_index;
        protected:
            bool putIfNeeded(Object* object) override;
            bool removeIfNeeded(Object* object) override;
            bool update(Object* object) override;
            explicit AttributeNameIndex(Object* root, std::string& attributeName);
            explicit AttributeNameIndex(Object* root, std::string&& attributeName);
        public:
            const std::vector<Object*> getByValue(const std::string& value);
        
        BEFRIEND_INDEX_CREATOR_FUNCTION;
    };

    class TagNameIndex: public Index {
        private:
            const std::string m_tagName;
            std::vector<Object*> m_index;
        protected:
            bool putIfNeeded(Object* object) override;
            bool removeIfNeeded(Object* object) override;
            bool update(Object* object) override;
            explicit TagNameIndex(Object* root, std::string& tagName);
            explicit TagNameIndex(Object* root, std::string&& tagName);
        public:
            const std::vector<Object*> get();
        
        BEFRIEND_INDEX_CREATOR_FUNCTION;
    };
}


template <typename T, typename... Args>
T Templater::dynamic::index::createIndex(Args... args) requires (isIndex<T>) {
    T index(std::forward<Args>(args)...);
    index.init();

    return index;
}