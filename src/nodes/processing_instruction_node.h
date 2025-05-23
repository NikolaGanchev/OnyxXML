#pragma once

#include "node.h"
#include "text_node.h"

namespace Templater::dynamic::tags {

    /**
     * @brief A ProcessingInstruction Node. Automatically escapes the invalid string "?>" into "&#x3f;&#x3e;".
     * 
     */
    class ProcessingInstruction: public Text {
        private:
            /**
             * @brief The processing instruction target
             * 
             */
            std::string target;
        protected:
            void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const override;
            void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const override;
        public:
            /**
             * @brief Construct a new owning ProcessingInstruction by given target and text
             * 
             * @param text 
             * @param target
             * @param escapeMultiByte 
             */
            explicit ProcessingInstruction(std::string target, std::string text, bool escapeMultiByte = false); 


             /**
             * Construct a new non-owning ProcessingInstruction by given target and text
             * 
             * @param target
             * @param text 
             * @param escapeMultiByte 
             */
            explicit ProcessingInstruction(NonOwningNodeTag, std::string target, std::string text, bool escapeMultiByte = false); 


            /**
             * @brief Construct a new ProcessingInstruction object by copy
             * 
             * @param other 
             */
            explicit ProcessingInstruction(const ProcessingInstruction& other); 


            /**
             * @brief Construct a new ProcessingInstruction object by move
             * 
             * @param other 
             */
            explicit ProcessingInstruction(ProcessingInstruction&& other) noexcept; 


            /**
             * @brief ProcessingInstruction move assignment
             * 
             * @param other 
             */
            ProcessingInstruction& operator=(ProcessingInstruction&& other) noexcept; 


            /**
             * @brief Get the target string. The target of a processing instruction is the tag name part of the instruction.
             * For example:
             * @code{.xml}
             * <?templater instrutcion?>
             * @endcode 
             * The target in the above snippet is "templater".
             * 
             * @return const std::string& 
             */
            const std::string& getTarget() const;


            /**
             * @brief The tag name of a processing instruction node is the invalid xml tag name ".pi". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            std::string serialize() const override;
            std::string serializePretty(const std::string& indentationSequence, bool sortAttributes) const override;
            bool hasSpecialSerialization() const override;
            std::unique_ptr<Node> shallowCopy() const override;
    };
}