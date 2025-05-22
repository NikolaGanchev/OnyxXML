#include "cdata_node.h"
#include "text.h"

namespace Templater::dynamic::tags {
    const std::string& CData::getTagName() const {
        static const std::string name = ".cdata";
        return name;
    }

    std::string CData::serialize() const {
        return "<![CDATA[" + text::escapeMultiByte(text::escapeSequence(this->getText(), "]]>"), this->shouldEscapeMultiByte()) + "]]>";
    }

    std::string CData::serializePretty(const std::string& indentationSequence, bool sortAttributes) const {
        return serialize();
    }
    
    bool CData::hasSpecialSerialization() const {
        return true;
    }
    
    std::unique_ptr<Node> CData::shallowCopy() const {
        return std::make_unique<CData>(this->getText(), this->shouldEscapeMultiByte());
    }

    void CData::specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const {
        result << "<![CDATA[" << text::escapeMultiByte(text::escapeSequence(this->getText(), "]]>"), this->shouldEscapeMultiByte()) << "]]>";
        stack.pop_back();
    }

    void CData::specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const {
        result << indentation << "<![CDATA[" << text::escapeMultiByte(text::escapeSequence(this->getText(), "]]>"), this->shouldEscapeMultiByte()) << "]]>\n";
        stack.pop_back();
    }
}