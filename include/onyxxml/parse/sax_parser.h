#pragma once

#include "../node.h"

namespace onyx::dynamic::parser {
/**
 * @brief This interface is to be implemented for usage with the SaxParser
 * class. No default implementations provided.
 *
 */
class SaxListener {
   public:
    /**
     * @brief Called when the document starts
     *
     */
    virtual void onStart() = 0;
    /**
     * @brief Called when text is encountered. Entities are expanded by default.
     *
     * @param text
     */
    virtual void onText(std::string text) = 0;
    /**
     * @brief Called when a comment is encountered.
     *
     * @param text
     */
    virtual void onComment(std::string text) = 0;
    /**
     * @brief Called when a CDATA is encountered.
     *
     */
    virtual void onCData(std::string text) = 0;
    /**
     * @brief Called when a processing instruction is encountered.
     *
     */
    virtual void onInstruction(std::string tag, std::string instruction) = 0;
    /**
     * @brief Called when a tag is opened. Even if a tag is self closing,
     * onTagClose() will eventually be called.
     *
     */
    virtual void onTagOpen(std::string name, bool isSelfClosing,
                           std::vector<Attribute> attributes) = 0;
    /**
     * @brief Called when a tag is closed.
     *
     */
    virtual void onTagClose(std::string name) = 0;
    /**
     * @brief Called when an XML Declaration is found.
     *
     */
    virtual void onXMLDeclaration(std::string version, std::string encoding,
                                  bool hasEncoding, bool isStandalone,
                                  bool hasStandalone) = 0;
    /**
     * @brief Called when a DOCTYPE is found. Passed unparsed.
     *
     */
    virtual void onDoctype(std::string text) = 0;
    /**
     * @brief Called when an exception is thrown in the parser. The exception is
     * passed to the method.
     *
     * @param e
     */
    virtual void onException(std::exception& e) = 0;
    /**
     * @brief Called when the document ends
     *
     */
    virtual void onEnd() = 0;
};

/**
 * @brief An almost-fully XML standard compliant non-validating
 * syntax-preserving UTF-8 supporting SAX parser. The parser correctly parses
 * all possible XML syntax constructs except DTDs. DTDs are read and passed
 * literally and are NOT syntax validated. Elements like the XML declaration and
 * comments are not ignored. All entities
 * and character escape sequences are expanded with exception of DTD defined
 * custom entities. All other behavior is fully standard-compliant. Newlines are
 * handled per standard rules ("\r\n", "\r" get expanded to "\n"). The parser
 * throws an exception on any non-standard XML and does not try to do error
 * correction.
 */
class SaxParser {
   private:
    /**
     * @brief A SaxListener object
     *
     */
    SaxListener& listener;

   public:
    /**
     * @brief Construct a new Sax Parser object
     *
     * @param listener
     */
    SaxParser(SaxListener& listener);
    /**
     * @brief Parse an XML string
     *
     * @param input
     */
    void parse(std::string_view input);
};
}  // namespace onyx::dynamic::parser