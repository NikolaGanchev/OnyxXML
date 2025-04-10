#pragma once

#include "node.h"

namespace Templater::dynamic::dtags {


    /**
     * @brief A text Node
     * 
     */
    class Text: public Node {
        private:
            /**
             * @brief The text
             * 
             */
            const std::string text;


            /**
             * @brief Whether unicode sequences should be escaped. Set at construction.
             * 
             */
            const bool escapeMultiByte;
        public:
            /**
             * @brief Construct a new Text object by given text and whether unicode sequences should be escaped. 
             * Unicode sequences can be unsafe in environments that do not support UTF-8 and can be used to create injections. 
             * Unicode escaping escapes any unicode sequence to an XML/HTML entity. 
             * Due to the runtime cost and overall low risk this is false by default.
             * The escaping is not done by this class; it is a suggestion to consumers.
             * 
             * @param text 
             * @param escapeMultiByte 
             */
            explicit Text(std::string text, bool escapeMultiByte = false); 


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
            explicit Text(Text&& other); 
            bool isVoid() const override;


            /**
             * @brief Get the text string. The text string is automatically escaped.
             * 
             * @return const std::string& 
             */
            virtual const std::string& getText() const;


            /**
             * @brief The tag name of a text node is the invalid xml tag name ".text". 
             * This signals it shouldn't be used as a tag and is a marker of the class.
             * 
             * @return const std::string& 
             */
            const std::string& getTagName() const override;
            std::string serialise(const std::string& indentationSequence = getIndentationSequence(), bool sortAttributes = getSortAttributes()) const override;
            bool hasSpecialSerialisation() const override;
    };
}