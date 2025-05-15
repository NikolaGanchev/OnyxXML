#pragma once

#include "node.h"

namespace Templater::dynamic::tags {


    /**
     * @brief A text Node
     * 
     */
    class Text: public Node, public Node::SpecialSerializable {
        private:
            /**
             * @brief The text
             * 
             */
            std::string text;


            /**
             * @brief Whether unicode sequences should be escaped. Set at construction.
             * 
             */
            bool escapeMultiByte;
        protected:    
            void specialSerialize(std::vector<Node::SerializationNode>& stack, std::ostringstream& result) const override;
            void specialSerializePretty(std::vector<Node::SerializationNode>& stack, std::ostringstream& result, std::string& indentation, const std::string& indentationSequence, bool sortAttributes) const override;
        public:
            /**
             * @brief Construct a new owning Text object by given text and whether unicode sequences should be escaped. 
             * Unicode sequences can be unsafe in environments that do not support UTF-8 and can be used to create injections. 
             * Unicode escaping escapes any unicode sequence to an XML/HTML entity. 
             * Due to the runtime cost and overall low risk this is false by default.
             * 
             * @param text 
             * @param escapeMultiByte 
             */
            explicit Text(std::string text, bool escapeMultiByte = false); 


             /**
             * @brief Construct a new non-owning Text object by given text and whether unicode sequences should be escaped. 
             * Unicode sequences can be unsafe in environments that do not support UTF-8 and can be used to create injections. 
             * Unicode escaping escapes any unicode sequence to an XML/HTML entity. 
             * Due to the runtime cost and overall low risk this is false by default.
             * 
             * @param text 
             * @param escapeMultiByte 
             */
            explicit Text(NonOwningNodeTag, std::string text, bool escapeMultiByte = false); 


            /**
             * @brief Construct a new Text object by copy
             * 
             * @param other 
             */
            explicit Text(const Text& other); 


            /**
             * @brief Construct a new Text object by move
             * 
             * @param other 
             */
            explicit Text(Text&& other) noexcept; 


            /**
             * @brief Text move assignment
             * 
             * @param other 
             */
            Text& operator=(Text&& other) noexcept; 


            bool isVoid() const override;


            /**
             * @brief Get the text string. The text string is automatically escaped.
             * 
             * @return const std::string& 
             */
            virtual const std::string& getText() const;


            /**
             * @brief Whether UTF-8 escaping should be done
             * 
             * @return true 
             * @return false 
             */
            bool shouldEscapeMultiByte() const;


            /**
             * @brief The tag name of a text node is the invalid xml tag name ".text". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            std::string serialize() const override;
            std::string serializePretty(const std::string& indentationSequence, bool sortAttributes) const override;
            bool hasSpecialSerialization() const override;
            std::unique_ptr<Node> shallowCopy() const override;
            virtual bool shallowEquals(const Node& other) const override;
    };
}