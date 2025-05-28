#include "doctype_node.h"

#include "text.h"

namespace Templater::dynamic::tags {
const std::string& Doctype::getTagName() const {
    static const std::string name = ".doctype";
    return name;
}

std::string Doctype::serialize() const {
    return "<!DOCTYPE " +
           text::escapeMultiByte(this->getText(),
                                 this->shouldEscapeMultiByte()) +
           ">";
}

std::string Doctype::serializePretty(const std::string& indentationSequence,
                                     bool sortAttributes) const {
    return serialize();
}

bool Doctype::hasSpecialSerialization() const { return true; }

std::unique_ptr<Node> Doctype::shallowCopy() const {
    return std::make_unique<Doctype>(this->getText(),
                                     this->shouldEscapeMultiByte());
}

void Doctype::specialSerialize(std::vector<Node::SerializationNode>& stack,
                               std::ostringstream& result) const {
    result << "<!DOCTYPE "
           << text::escapeMultiByte(this->getText(),
                                    this->shouldEscapeMultiByte())
           << ">";
    stack.pop_back();
}

void Doctype::specialSerializePretty(
    std::vector<Node::SerializationNode>& stack, std::ostringstream& result,
    std::string& indentation, const std::string& indentationSequence,
    bool sortAttributes) const {
    result << "<!DOCTYPE "
           << text::escapeMultiByte(this->getText(),
                                    this->shouldEscapeMultiByte())
           << ">\n";
    stack.pop_back();
}
}  // namespace Templater::dynamic::tags