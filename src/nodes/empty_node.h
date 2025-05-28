#pragma once

#include "node.h"

namespace Templater::dynamic::tags {

/**
 * @brief A node which provides no serialization structure. Used as a root to
 * partial trees with adjacent children. Useful for generating partial XML
 * fragments.
 *
 */
class EmptyNode : public Node, public Node::SpecialSerializable {
   protected:
    void specialSerialize(std::vector<Node::SerializationNode>& stack,
                          std::ostringstream& result) const override;
    void specialSerializePretty(std::vector<Node::SerializationNode>& stack,
                                std::ostringstream& result,
                                std::string& indentation,
                                const std::string& indentationSequence,
                                bool sortAttributes) const override;

   public:
    using Node::Node;

    /**
     * @brief The tag name of an empty node is the invalid xml tag name
     * ".empty". This signals it shouldn't be used as a tag and is a marker of
     * the class.
     *
     * @return const std::string&
     */
    const std::string& getTagName() const override;
    bool isVoid() const override;
    bool hasSpecialSerialization() const override;
    std::unique_ptr<Node> shallowCopy() const override;
};
}  // namespace Templater::dynamic::tags