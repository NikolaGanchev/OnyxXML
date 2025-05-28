#pragma once

#include "node.h"
#include "text_node.h"

namespace Templater::dynamic::tags {

/**
 * @brief A CDATA Node. Automatically escapes the invalid string "]]>" into
 * "&#x5d;&#x5d;&#x3e;".
 *
 */
class CData : public Text {
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
     * @brief The tag name of a CData node is the invalid xml tag name ".cdata".
     * This signals it shouldn't be used as a tag and is a marker of the class.
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