#include "comment_node.h"
#include "text.h"

namespace Templater::dynamic::dtags {
    const std::string& Comment::getTagName() const {
        static const std::string name = ".comment";
        return name;
    }

    
    std::string Comment::serialise(const std::string& indentationSequence, bool sortAttributes) const {
        return "<!--" + text::escape(this->getText()) + "-->";
    }
    
    bool Comment::hasSpecialSerialisation() const {
        return true;
    }
}