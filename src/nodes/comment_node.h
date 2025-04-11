#pragma once

#include "node.h"
#include "text_node.h"

namespace Templater::dynamic::dtags {

    /**
     * @brief A comment Node
     * 
     */
    class Comment: public Text {
        using Text::Text;
        public:
            /**
             * @brief The tag name of a comment node is the invalid xml tag name ".comment". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            std::string serialise() const override;
            std::string serialisePretty(const std::string& indentationSequence, bool sortAttributes) const override;
            bool hasSpecialSerialisation() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
}