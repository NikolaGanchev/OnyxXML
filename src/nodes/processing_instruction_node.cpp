#include "nodes/processing_instruction_node.h"

#include "text.h"

namespace onyx::dynamic::tags {

ProcessingInstruction::ProcessingInstruction(std::string target,
                                             std::string text,
                                             bool escapeMultiByte)
    : target{target}, Text{text, escapeMultiByte} {}
ProcessingInstruction::ProcessingInstruction(NonOwningNodeTag,
                                             std::string target,
                                             std::string text,
                                             bool escapeMultiByte)
    : target{target}, Text{NonOwning, text, escapeMultiByte} {}

ProcessingInstruction::ProcessingInstruction(const ProcessingInstruction& other)
    : target{other.target}, Text{other} {}
ProcessingInstruction::ProcessingInstruction(
    ProcessingInstruction&& other) noexcept
    : target{std::move(other.target)}, Text{other} {}

ProcessingInstruction& ProcessingInstruction::operator=(
    ProcessingInstruction&& other) noexcept {
    if (this == &other) return *this;
    this->target = std::move(other.target);
    Text::operator=(std::move(other));

    return *this;
}

const std::string& ProcessingInstruction::getTagName() const {
    static const std::string name = ".pi";
    return name;
}

std::string ProcessingInstruction::serialize() const {
    return "<?" + this->getTarget() + " " +
           text::escapeMultiByte(text::escapeSequence(this->getText(), "?>"),
                                 this->shouldEscapeMultiByte()) +
           "?>";
}

std::string ProcessingInstruction::serializePretty(
    const std::string& indentationSequence, bool sortAttributes) const {
    return serialize();
}

bool ProcessingInstruction::hasSpecialSerialization() const { return true; }

std::unique_ptr<Node> ProcessingInstruction::shallowCopy() const {
    return std::make_unique<ProcessingInstruction>(
        this->getTarget(), this->getText(), this->shouldEscapeMultiByte());
}

const std::string& ProcessingInstruction::getTarget() const {
    return this->target;
}

void ProcessingInstruction::specialSerialize(
    std::vector<Node::SerializationNode>& stack,
    std::ostringstream& result) const {
    result << "<?" << this->getTarget() << " "
           << text::escapeMultiByte(text::escapeSequence(this->getText(), "?>"),
                                    this->shouldEscapeMultiByte())
           << "?>";
    stack.pop_back();
}

void ProcessingInstruction::specialSerializePretty(
    std::vector<Node::SerializationNode>& stack, std::ostringstream& result,
    std::string& indentation, const std::string& indentationSequence,
    bool sortAttributes) const {
    result << indentation << "<?" << this->getTarget() << " "
           << text::escapeMultiByte(text::escapeSequence(this->getText(), "?>"),
                                    this->shouldEscapeMultiByte())
           << "?>\n";
    stack.pop_back();
}

bool ProcessingInstruction::shallowEquals(const Node& _other) const {
    if (!Text::shallowEquals(_other)) return false;
    const ProcessingInstruction* other =
        dynamic_cast<const ProcessingInstruction*>(&_other);

    return this->target == other->target;
}
}  // namespace onyx::dynamic::tags