#pragma once

#include "node.h"
#include "text_node.h"

namespace Templater::dynamic::tags {

    /**
     * @brief A comment Node
     * 
     */
    class Comment: public Text {
        using Text::Text;
        private:
            /**
             * @brief The escape table to be passed to text::escape()
             * This is an escape table that only handles the hyphen character, '-'
             * Only the "--" sequence is forbidden inside comments, but escaping the 
             * whole string "--" would lead to performance issues.
             * For this reason, any '-' in the comment is escaped with the normal html escape sequence.
             * 
             */
            static constexpr std::array<const char*, 128> escapeTable = []() {
                std::array<const char*, 128> table{};
                table['-'] = "&#45;";
                return table;
            }();
        protected:
            void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const override;
            void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const override;
        public:
            /**
             * @brief The tag name of a comment node is the invalid xml tag name ".comment". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            std::string serialize() const override;
            std::string serializePretty(const std::string& indentationSequence, bool sortAttributes) const override;
            bool hasSpecialSerialization() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
}