#pragma once

#include "../node.h"
#include "processing_instruction_node.h"

namespace Templater::dynamic::tags {

/**
 * @brief A XmlDeclaration Node. Contains data about the XML version of the
 * file, the encoding and whether the file contains external markup
 * declarations. Version info is always available; encoding and standalone can
 * be skipped and have default values of "UTF-8" and false, respectively.
 *
 */
class XmlDeclaration : public ProcessingInstruction {
   private:
    /**
     * @brief The version of XML
     *
     */
    std::string versionInfo;

    /**
     * @brief The encoding used
     *
     */
    std::string encoding;

    /**
     * @brief Whether the document contains external markup declarations
     *
     */
    bool standalone;

    /**
     * @brief  Whether the declaration had an encoding attribute
     *
     */
    bool hadEncoding;

    /**
     * @brief Whether the declaration had a standalone attribute
     *
     */
    bool hadStandalone;

   protected:
    void specialSerialize(std::vector<Node::SerializationNode>& stack,
                          std::ostringstream& result) const override;
    void specialSerializePretty(std::vector<Node::SerializationNode>& stack,
                                std::ostringstream& result,
                                std::string& indentation,
                                const std::string& indentationSequence,
                                bool sortAttributes) const override;

   public:
    /**
     * @brief Construct a new XmlDeclaration object
     *
     * @param versionInfo
     * @param encoding
     * @param standalone
     * @param escapeMultiByte
     */
    explicit XmlDeclaration(std::string versionInfo, std::string encoding,
                            bool standalone, bool escapeMultiByte = false);

    /**
     * @brief Construct a new Xml Declaration object
     *
     * @param versionInfo
     * @param encoding
     * @param hasEncoding
     * @param standalone
     * @param hasStandalone
     * @param escapeMultiByte
     */
    explicit XmlDeclaration(std::string versionInfo, std::string encoding,
                            bool hasEncoding, bool standalone,
                            bool hasStandalone, bool escapeMultiByte = false);

    /**
     * @brief Construct a new XmlDeclaration object
     *
     * @param versionInfo
     * @param escapeMultiByte
     */
    explicit XmlDeclaration(std::string versionInfo,
                            bool escapeMultiByte = false);

    /**
     * @brief Construct a new Xml Declaration object
     *
     * @param versionInfo
     * @param encoding
     * @param standalone
     * @param escapeMultiByte
     */
    explicit XmlDeclaration(NonOwningNodeTag, std::string versionInfo,
                            std::string encoding, bool standalone,
                            bool escapeMultiByte = false);

    /**
     * @brief Construct a new non-owning XmlDeclaration object
     *
     * @param versionInfo
     * @param escapeMultiByte
     */
    explicit XmlDeclaration(NonOwningNodeTag, std::string versionInfo,
                            bool escapeMultiByte = false);

    /**
     * @brief Construct a new Xml Declaration object
     *
     * @param versionInfo
     * @param encoding
     * @param hasEncoding
     * @param standalone
     * @param hasStandalone
     * @param escapeMultiByte
     */
    explicit XmlDeclaration(NonOwningNodeTag, std::string versionInfo,
                            std::string encoding, bool hasEncoding,
                            bool standalone, bool hasStandalone,
                            bool escapeMultiByte = false);

    /**
     * @brief Construct a new XmlDeclaration object by copy
     *
     * @param other
     */
    explicit XmlDeclaration(const XmlDeclaration& other);

    /**
     * @brief Construct a new XmlDeclaration object by move
     *
     * @param other
     */
    explicit XmlDeclaration(XmlDeclaration&& other) noexcept;

    /**
     * @brief XmlDeclaration move assignment
     *
     * @param other
     */
    XmlDeclaration& operator=(XmlDeclaration&& other) noexcept;

    /**
     * @brief Get the Version Info string
     *
     * @return const std::string&
     */
    const std::string& getVersionInfo() const;

    /**
     * @brief Get the encoding name string
     *
     * @return const std::string&
     */
    const std::string& getEncoding() const;

    /**
     * @brief Whether the XML file contains external markup declarations
     *
     * @return true
     * @return false
     */
    bool getStandalone() const;

    /**
     * @brief The tag name of a processing instruction node is the invalid xml
     * tag name ".xml". This signals it shouldn't be used as a tag and is a
     * marker of the class.
     *
     * @return const std::string&
     */
    const std::string& getTagName() const override;
    std::string serialize() const override;
    std::string serializePretty(const std::string& indentationSequence,
                                bool sortAttributes) const override;
    bool hasSpecialSerialization() const override;
    std::unique_ptr<Node> shallowCopy() const override;
    virtual bool shallowEquals(const Node& other) const override;
};
}  // namespace Templater::dynamic::tags