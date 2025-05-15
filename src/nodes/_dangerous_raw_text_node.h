#pragma once

#include "node.h"

namespace Templater::dynamic::tags {
    /**
     * @brief A raw text Node. Susceptible to XML injections. Only use if needed explicitly.
     * 
     */
    class __DangerousRawText: public Node, public Node::SpecialSerializable {
        private:
            /**
             * @brief The text
             * 
             */
            std::string text;
        protected:
            void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const override;
            void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const override;
        public:
            /**
             * @brief Construct a new owning __DangerousRawText object by given text. Escaping is not done.
             * 
             * @param text 
             */
            explicit __DangerousRawText(std::string text); 


             /**
             * @brief Construct a new non-owning __DangerousRawText object by given text. Escaping is not done.
             * 
             * @param text 
             */
            explicit __DangerousRawText(NonOwningNodeTag, std::string text); 


            /**
             * @brief Construct a new __DangerousRawTex object by copy
             * 
             * @param other 
             */
            explicit __DangerousRawText(const __DangerousRawText& other); 


            /**
             * @brief Construct a new __DangerousRawTex object by move
             * 
             * @param other 
             */
            explicit __DangerousRawText(__DangerousRawText&& other) noexcept; 


            /**
             * @brief __DangerousRawText move assignment
             * 
             * @param other 
             */
            __DangerousRawText& operator=(__DangerousRawText&& other) noexcept; 


            bool isVoid() const override;

            /**
             * @brief The tag name of a comment node is the invalid xml tag name ".rawText". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;


            /**
             * @brief Get a dangerous text string. The text string is NOT escaped. It can lead to security vulnerabilities.
             * 
             * @return const std::string& 
             */
            const std::string& getText() const; 
            std::string serialize() const override;
            std::string serializePretty(const std::string& indentationSequence, bool sortAttributes) const override;            
            bool hasSpecialSerialization() const override;
            std::unique_ptr<Node> shallowCopy() const override;
            virtual bool shallowEquals(const Node& other) const override;
    };
}