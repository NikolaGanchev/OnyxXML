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

    std::string Text::serialise(const std::string& indentationSequence, bool sortAttributes) const {
        return text::escape(this->text, this->escapeMultiByte);
    }
    
    
    bool Text::hasSpecialSerialisation() const {
        return true;
    }


    bool Text::shouldEscapeMultiByte() const {
        return this->escapeMultiByte;
    }


    std::unique_ptr<Node> Text::shallowCopy() const {
        return std::make_unique<Text>(this->getText(), this->escapeMultiByte);
    }
}