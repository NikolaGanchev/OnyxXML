#pragma once

#include "object.h"
#include <unordered_set>
#include <unordered_map>

namespace Templater::dynamic::index {
    class AttributeNameIndex: public Index {
        private:
            const std::string m_attributeName;
            std::unordered_map<std::string, std::vector<std::weak_ptr<Object>>> m_index;
        protected:
            bool putIfNeeded(std::shared_ptr<Object> object) override;
            bool removeIfNeeded(std::shared_ptr<Object> object) override;
            bool update(std::shared_ptr<Object> object) override;
        public:
            explicit AttributeNameIndex(std::shared_ptr<Object> root, std::string& attributeName);
            explicit AttributeNameIndex(std::shared_ptr<Object> root, std::string&& attributeName);
            std::vector<std::shared_ptr<Object>> getByValue(const std::string& value);
            void init() override;
    };
}