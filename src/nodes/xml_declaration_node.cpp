#include "nodes/xml_declaration_node.h"

#include "text.h"

namespace onyx::dynamic::tags {

XmlDeclaration::XmlDeclaration(std::string versionInfo, std::string encoding,
                               bool standalone, bool escapeMultiByte)
    : versionInfo(versionInfo),
      encoding(encoding),
      hadEncoding(true),
      standalone(standalone),
      hadStandalone(true),
      ProcessingInstruction("xml", "", escapeMultiByte) {}

XmlDeclaration::XmlDeclaration(std::string versionInfo, bool escapeMultiByte)
    : versionInfo(versionInfo),
      encoding("UTF-8"),
      hadEncoding(false),
      standalone(false),
      hadStandalone(false),
      ProcessingInstruction("xml", "", escapeMultiByte) {}

XmlDeclaration::XmlDeclaration(std::string versionInfo, std::string encoding,
                               bool hasEncoding, bool standalone,
                               bool hasStandalone, bool escapeMultiByte)
    : versionInfo(versionInfo),
      encoding(encoding),
      hadEncoding(hasEncoding),
      standalone(standalone),
      hadStandalone(hasStandalone),
      ProcessingInstruction("xml", "", escapeMultiByte) {}

XmlDeclaration::XmlDeclaration(NonOwningNodeTag, std::string versionInfo,
                               std::string encoding, bool hasEncoding,
                               bool standalone, bool hasStandalone,
                               bool escapeMultiByte)
    : versionInfo(versionInfo),
      encoding(encoding),
      hadEncoding(hasEncoding),
      standalone(standalone),
      hadStandalone(hasStandalone),
      ProcessingInstruction(NonOwning, "xml", "", escapeMultiByte) {}

XmlDeclaration::XmlDeclaration(NonOwningNodeTag, std::string versionInfo,
                               std::string encoding, bool standalone,
                               bool escapeMultiByte)
    : versionInfo(versionInfo),
      encoding(encoding),
      hadEncoding(true),
      standalone(standalone),
      hadStandalone(true),
      ProcessingInstruction(NonOwning, "xml", "", escapeMultiByte) {}

XmlDeclaration::XmlDeclaration(NonOwningNodeTag, std::string versionInfo,
                               bool escapeMultiByte)
    : versionInfo(versionInfo),
      hadEncoding(false),
      encoding("UTF-8"),
      standalone(false),
      hadStandalone(false),
      ProcessingInstruction(NonOwning, "xml", "", escapeMultiByte) {}

XmlDeclaration::XmlDeclaration(const XmlDeclaration& other)
    : versionInfo(other.versionInfo),
      encoding(other.encoding),
      standalone(other.standalone),
      hadEncoding(other.hadEncoding),
      hadStandalone(other.hadStandalone),
      ProcessingInstruction(other) {}

XmlDeclaration::XmlDeclaration(XmlDeclaration&& other) noexcept
    : versionInfo(std::move(other.versionInfo)),
      encoding(std::move(other.encoding)),
      standalone(other.standalone),
      hadEncoding(other.hadEncoding),
      hadStandalone(other.hadStandalone),
      ProcessingInstruction(std::move(other)) {}

XmlDeclaration& XmlDeclaration::operator=(XmlDeclaration&& other) noexcept {
    if (this == &other) return *this;
    this->versionInfo = std::move(other.versionInfo);
    this->encoding = std::move(other.encoding);
    this->standalone = other.standalone;
    this->hadEncoding = other.hadEncoding;
    this->hadStandalone = other.hadStandalone;
    ProcessingInstruction::operator=(std::move(other));

    return *this;
}

const std::string& XmlDeclaration::getVersionInfo() const {
    return this->versionInfo;
}

const std::string& XmlDeclaration::getEncoding() const {
    return this->encoding;
}

bool XmlDeclaration::getStandalone() const { return this->standalone; }

const std::string& XmlDeclaration::getTagName() const {
    static const std::string name = ".xml";
    return name;
}

std::string XmlDeclaration::serialize() const {
    return "<?xml version=\"" +
           text::escape(this->versionInfo, this->shouldEscapeMultiByte()) +
           "\"" +
           (this->hadEncoding
                ? (" encoding=\"" +
                   text::escape(this->encoding, this->shouldEscapeMultiByte()) +
                   "\"")
                : "") +
           (this->hadStandalone
                ? (" standalone=\"" +
                   std::string(this->standalone ? "yes\"" : "no\""))
                : "") +
           "?>";
}

std::string XmlDeclaration::serializePretty(
    const std::string& indentationSequence, bool sortAttributes) const {
    return serialize();
}

bool XmlDeclaration::hasSpecialSerialization() const { return true; }

std::unique_ptr<Node> XmlDeclaration::shallowCopy() const {
    return std::make_unique<XmlDeclaration>(this->versionInfo, this->encoding,
                                            this->standalone,
                                            this->shouldEscapeMultiByte());
}

void XmlDeclaration::specialSerialize(
    std::vector<Node::SerializationNode>& stack,
    std::ostringstream& result) const {
    result << "<?xml version=\""
           << text::escape(this->versionInfo, this->shouldEscapeMultiByte())
           << "\""
           << (this->hadEncoding
                   ? (" encoding=\"" +
                      text::escape(this->encoding,
                                   this->shouldEscapeMultiByte()) +
                      "\"")
                   : "")
           << (this->hadStandalone
                   ? (" standalone=\"" +
                      std::string(this->standalone ? "yes\"" : "no\""))
                   : "")
           << "?>";
    stack.pop_back();
}

void XmlDeclaration::specialSerializePretty(
    std::vector<Node::SerializationNode>& stack, std::ostringstream& result,
    std::string& indentation, const std::string& indentationSequence,
    bool sortAttributes) const {
    result << "<?xml version=\""
           << text::escape(this->versionInfo, this->shouldEscapeMultiByte())
           << "\" "
           << (this->hadEncoding
                   ? (" encoding=\"" +
                      text::escape(this->encoding,
                                   this->shouldEscapeMultiByte()) +
                      "\"")
                   : "")
           << (this->hadStandalone
                   ? (" standalone=\"" +
                      std::string(this->standalone ? "yes\"" : "no\""))
                   : "")
           << "?>\n";
    stack.pop_back();
}

bool XmlDeclaration::shallowEquals(const Node& _other) const {
    if (!ProcessingInstruction::shallowEquals(_other)) return false;
    const XmlDeclaration* other = dynamic_cast<const XmlDeclaration*>(&_other);

    return this->hadEncoding == other->hadEncoding &&
           this->hadStandalone == other->hadStandalone &&
           this->standalone == other->standalone &&
           this->versionInfo == other->versionInfo &&
           this->encoding == other->encoding;
}
}  // namespace onyx::dynamic::tags