#include "_dangerous_raw_text_node.h"

namespace Templater::dynamic::dtags {
    
    __DangerousRawText::__DangerousRawText(std::string text): text(text), Node{} {}

    __DangerousRawText::__DangerousRawText(const __DangerousRawText& other): text(other.text), Node{} {}
    __DangerousRawText::__DangerousRawText(__DangerousRawText&& other): text(std::move(other.text)), Node{} {}

    const std::string& __DangerousRawText::getTagName() const {
        static const std::string name = ".rawText";
        return name;
    }

    const std::string& __DangerousRawText::getText() const {
        return this->text;
    }

    bool __DangerousRawText::isVoid() const {
        return true;
    }

    std::string __DangerousRawText::serialise(const std::string& indentationSequence, bool sortAttributes) const {
        return this->text;
    }

}