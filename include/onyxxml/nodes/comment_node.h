#pragma once

#include "../node.h"
#include "text_node.h"

namespace Templater::dynamic::tags {

/**
 * @brief A comment Node. Automatically escapes the invalid string "--" into
 * "&#x2d;&#x2d;".
 *
 */
class Comment : public Text {
    using Text::Text;

   protected:
    void specialSerialize(std::vector<Node::SerializationNode>& stack,
                          std::ostringstream& result) const override;
    void specialSerializePretty(std::vector<Node::SerializationNode>& stack,
                                std::ostringstream& result,
                                std::string& indentation,
                                const std::string& indentationSequence,
                                bool sortAttributes) const override;

   public:
    /**
     * @brief The tag name of a comment node is the invalid xml tag name
     * ".comment". This signals it shouldn't be used as a tag and is a marker of
     * the class.
     *
     * @return const std::string&
     */
    const std::string& getTagName() const override;
    std::string serialize() const override;
    std::string serializePretty(const std::string& indentationSequence,
                                bool sortAttributes) const override;
    bool hasSpecialSerialization() const override;
    std::unique_ptr<Node> shallowCopy() const override;
};
}  // namespace Templater::dynamic::tags