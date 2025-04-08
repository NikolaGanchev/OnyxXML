#include "comment_node.h"

namespace Templater::dynamic::dtags {
    const std::string& Comment::getTagName() const {
        static const std::string name = ".comment";
        return name;
    }
}