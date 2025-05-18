#include "comment_node.h"
#include "text.h"

namespace Templater::dynamic::tags {
    const std::string& Comment::getTagName() const {
        static const std::string name = ".comment";
        return name;
    }

    std::string Comment::serialize() const {
        return "<!--" + text::escape(text::escapeSequence(this->getText(), "--"), Comment::escapeTable, this->shouldEscapeMultiByte()) + "-->";
    }

    std::string Comment::serializePretty(const std::string& indentationSequence, bool sortAttributes) const {
        return serialize();
    }
    
    bool Comment::hasSpecialSerialization() const {
        return true;
    }
    
    std::unique_ptr<Node> Comment::shallowCopy() const {
        return std::make_unique<Comment>(this->getText(), this->shouldEscapeMultiByte());
    }

    void Comment::specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const {
        result << "<!--" << text::escape(text::escapeSequence(this->getText(), "--"), Comment::escapeTable, this->shouldEscapeMultiByte()) << "-->";
        stack.pop_back();
    }

    void Comment::specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const {
        result << indentation << "<!--" << text::escape(text::escapeSequence(this->getText(), "--"), Comment::escapeTable, this->shouldEscapeMultiByte()) << "-->\n";
        stack.pop_back();
    }
}