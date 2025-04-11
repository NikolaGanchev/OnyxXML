#include "comment_node.h"
#include "text.h"

namespace Templater::dynamic::dtags {
    const std::string& Comment::getTagName() const {
        static const std::string name = ".comment";
        return name;
    }

    std::string Comment::serialize() const {
        return "<!--" + text::escape(this->getText(), this->shouldEscapeMultiByte()) + "-->";
    }

    std::string Comment::serializePretty(const std::string& indentationSequence, bool sortAttributes) const {
        return "<!--" + text::escape(this->getText(), this->shouldEscapeMultiByte()) + "-->";
    }
    
    bool Comment::hasSpecialSerialization() const {
        return true;
    }
    
    std::unique_ptr<Node> Comment::shallowCopy() const {
        return std::make_unique<Comment>(this->getText(), this->shouldEscapeMultiByte());
    }
}