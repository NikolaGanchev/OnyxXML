#include "nodes/_dangerous_raw_text_node.h"

namespace onyx::dynamic::tags {

__DangerousRawText::__DangerousRawText(std::string text) : text(text), Node{} {}
__DangerousRawText::__DangerousRawText(NonOwningNodeTag, std::string text)
    : text(text), Node{NonOwning} {}

__DangerousRawText::__DangerousRawText(const __DangerousRawText& other)
    : text(other.text), Node{} {}
__DangerousRawText::__DangerousRawText(__DangerousRawText&& other) noexcept
    : text(std::move(other.text)), Node{} {}

__DangerousRawText& __DangerousRawText::operator=(
    __DangerousRawText&& other) noexcept {
    if (this == &other) return *this;
    this->text = std::move(other.text);
    Node::operator=(std::move(other));

    return *this;
}

const std::string& __DangerousRawText::getTagName() const {
    static const std::string name = ".rawText";
    return name;
}

const std::string& __DangerousRawText::getText() const { return this->text; }

bool __DangerousRawText::isVoid() const { return true; }

std::string __DangerousRawText::serialize() const { return this->text; }

std::string __DangerousRawText::serializePretty(
    const std::string& indentationSequence, bool sortAttributes) const {
    return this->text;
}

bool __DangerousRawText::hasSpecialSerialization() const { return true; }

std::unique_ptr<Node> __DangerousRawText::shallowCopy() const {
    return std::make_unique<__DangerousRawText>(this->getText());
}

bool __DangerousRawText::shallowEquals(const Node& _other) const {
    if (!Node::shallowEquals(_other)) return false;
    const __DangerousRawText* other =
        dynamic_cast<const __DangerousRawText*>(&_other);

    return this->text == other->text;
}

void __DangerousRawText::specialSerialize(
    std::vector<Node::SerializationNode>& stack,
    std::ostringstream& result) const {
    result << this->text;
    stack.pop_back();
}

void __DangerousRawText::specialSerializePretty(
    std::vector<Node::SerializationNode>& stack, std::ostringstream& result,
    std::string& indentation, const std::string& indentationSequence,
    bool sortAttributes) const {
    result << indentation << this->text << "\n";
    stack.pop_back();
}

std::string __DangerousRawText::getStringValue() const {
    return this->getText();
}

bool __DangerousRawText::hasShallowStringValue() const { return true; }
}  // namespace onyx::dynamic::tags