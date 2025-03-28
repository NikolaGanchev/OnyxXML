#pragma once

#include "object.h"
#include <unordered_set>
#include <unordered_map>

namespace Templater::dynamic::index {
    class AttributeNameIndex: public Index {
        private:
            const std::string m_attributeName;
            std::unordered_map<std::string, std::vector<Object*>> m_index;
        protected:
            bool putIfNeeded(Object* object) override;
            bool removeIfNeeded(Object* object) override;
            bool update(Object* object) override;
        public:
            explicit AttributeNameIndex(Object* root, std::string& attributeName);
            explicit AttributeNameIndex(Object* root, std::string&& attributeName);
            std::vector<Object*> getByValue(const std::string& value);
    };
}