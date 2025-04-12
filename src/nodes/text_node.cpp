#include "text_node.h"
#include "text.h"

namespace Templater::dynamic::dtags {
    Text::Text(std::string text, bool escapeMultiByte): text(text), escapeMultiByte(escapeMultiByte), Node{} {}

    Text::Text(const Text& other): text(other.text), escapeMultiByte(other.escapeMultiByte), Node{} {}
    Text::Text(Text&& other): text(std::move(other.text)), escapeMultiByte(other.escapeMultiByte), Node{} {}

    const std::string& Text::getText() const {
        return this->text;
    }

    const std::string& Text::getTagName() const {
        static const std::string name = ".text";
        return name;
    }

    bool Text::isVoid() const {
        return true;
    }

    std::string Text::serialize() const {
        return text::escape(this->text, this->escapeMultiByte);
    }

    std::string Text::serializePretty(const std::string& indentationSequence, bool sortAttributes) const {
        return serialize();
    }

    void Text::specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const {
        result << text::escape(this->text, this->escapeMultiByte);
        stack.pop_back();
    }

    void Text::specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const {
        result << indentation << text::escape(this->text, this->escapeMultiByte) << "\n";
        stack.pop_back();
    }
    
    bool Text::hasSpecialSerialization() const {
        return true;
    }


    bool Text::shouldEscapeMultiByte() const {
        return this->escapeMultiByte;
    }


    std::unique_ptr<Node> Text::shallowCopy() const {
        return std::make_unique<Text>(this->getText(), this->escapeMultiByte);
    }

    
    bool Text::shallowEquals(const Node& _other) const {
        if (!Node::shallowEquals(_other)) return false;
        const Text* other = dynamic_cast<const Text*>(&_other);
        if (this->escapeMultiByte != other->escapeMultiByte) return false;

        return this->text == other->text;
    }
}