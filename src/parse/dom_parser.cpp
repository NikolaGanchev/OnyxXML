#include "dom_parser.h"
#include <vector>
#include "../nodes/generic_node.h"
#include "../nodes/empty_node.h"
#include "../nodes/text_node.h"
#include "../nodes/comment_node.h"

namespace Templater::dynamic::parser {
    ParseResult::ParseResult(): arena{0}, root{nullptr} {}

    ParseResult::ParseResult(Arena arena, Node* root): arena{std::move(arena)}, root{root} {}

    ParseResult::ParseResult(ParseResult&& other): arena{std::move(other.arena)} {
        this->root = other.root;
        other.root = nullptr;
    }

    ParseResult& ParseResult::operator=(ParseResult&& other) {
        this->arena = std::move(other.arena);
        this->root = other.root;
        other.root = nullptr;

        return *this;
    }

    bool isWhitespace(const char pos) {
        return (pos == ' ' || pos == '\t' || pos == '\r' || pos == '\n');
    }

    const char* skipWhitespace(const char* pos) {
        while (isWhitespace(*pos)) {
            pos++;
        }

        return pos;
    }

    bool isNameStartChar(const char ch) {
        return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || ch == ':';
    }  

    bool isNameChar(const char ch) {
        return (ch >= 'A' && ch <= 'Z') 
                || (ch >= 'a' && ch <= 'z') 
                || (ch >= '0' && ch <= '9')
                || ch == '_' 
                || ch == ':'
                || ch == '-'
                || ch == '.';
    }       

    std::string_view readName(const char* pos) {
        const char* localPos = pos;
        if (!isNameStartChar(*localPos)) return std::string_view();
        localPos++;
        while (isNameChar(*localPos)) {
            localPos++;
        }

        return std::string_view(pos, localPos-pos);
    }

    std::string_view extractText(const char* pos) {
        const char* localPos = pos;

        while (*localPos != '<') {
            if (*localPos == '\0') return std::string_view();
            localPos++;
        }

        return std::string_view(pos, localPos-pos);
    }

    std::string_view extractAttributeValue(const char* pos, char attributeQuote) {
        const char* localPos = pos;

        while (*localPos != attributeQuote) {
            if (*localPos == '\0') return std::string_view();
            localPos++;
        }

        return std::string_view(pos, localPos-pos);
    }

    const char* checkSequence(const char* toCheck, const char* pos) {
        const char* copy = pos;
        while (*toCheck != '\0' && *pos != '\0') {
            if (*toCheck != *pos) {
                return copy;
            }
            toCheck++;
            pos++;
        }

        if (*toCheck == '\0') return pos;
        
        return copy;
    }

    Arena DomParser::parseDryRun(std::string_view input) {
        std::vector<size_t> stack;

        size_t id = 0;

        const char* pos = input.data();

        Arena::Builder builder;

        std::hash<std::string_view> hasher;

        // Root
        builder.preallocate<tags::EmptyNode>();

        std::string_view root = ".empty";

        stack.push_back(hasher(root));

        pos = skipWhitespace(pos);

        while(*pos != '\0') {
            // Invariant - always at the start of either a tag or a sequence of text
            if (*pos != '<') {
                std::string_view text = extractText(pos);
                if (text.empty()) {
                    while (*pos != '\0') {
                        if (!isWhitespace(*pos)) throw std::invalid_argument("Invalid end after tag open");
                        pos++;
                    }
                    break;
                }
                pos += text.size();

                builder.preallocate<tags::Text>();

                continue;
            }

            // Invariant - always in tag name
            pos++;

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            bool isClosing = (*pos == '/');

            if (isClosing) pos++;

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            const char* commentCheckPos = checkSequence("!--", pos);
            if (commentCheckPos != pos) {
                pos = commentCheckPos;
                if (*pos == '\0') {
                    throw std::invalid_argument("Premature end of document");
                }
                const char* localPos = pos;

                while (!(*localPos == '-' && *(localPos + 1) != '\0' && *(localPos + 1) == '-')) {
                    localPos++;
                    if (*localPos == '\0') throw std::invalid_argument("Invalid comment without ending");
                }

                localPos += 2;

                if (*localPos != '>') throw std::invalid_argument("-- inside of comment not allowed");
                pos = localPos;
                pos++;
                builder.preallocate<tags::Comment>();
                continue;
            }

            // Invariant - pos always at tag name start
            std::string_view tagName = readName(pos);
            if (tagName.empty()) {
                throw std::invalid_argument("Invalid tag name");
            }
            pos += tagName.size();
            
            bool couldHaveAttributes = isWhitespace(*pos) && !isClosing;

            // Invariant - pos always after tag name end
            pos = skipWhitespace(pos);

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            if (couldHaveAttributes) {
                // Invariant - either at start of attribute or at >
                while (*pos != '>' && *pos != '/') {
                    std::string_view attributeName = readName(pos);
                    if (attributeName.empty()) {
                        throw std::invalid_argument("Invalid non-closing tag");
                    }
                    pos += attributeName.size();

                    // Invariant - after attribute name
                    pos = skipWhitespace(pos);

                    // Invariant - at = or error state
                    if (*pos == '\0' || *pos != '=') {
                        throw std::invalid_argument("No = after attribute");
                    }
                    pos++;
                    // Invariant - after attribute =
                    pos = skipWhitespace(pos);
                    char attributeQuote = '\0';
                    if (*pos == '\0') {
                        throw std::invalid_argument("Premature end at attribute");
                    }

                    if (*pos == '\'' || *pos == '\"') {
                        attributeQuote = *pos;
                    } else {
                        throw std::invalid_argument("No quote (\" or \') after attribute =");
                    }
                    pos++;

                    // Invariant - after attribute value opening quote
                    std::string_view attributeValue = extractAttributeValue(pos, attributeQuote);
                    if (attributeValue.empty() && *pos != attributeQuote) {
                        throw std::invalid_argument("Improperly closed attribute value");
                    }
                    pos += attributeValue.size();

                    // Invariant - pos is at closing quote
                    pos++;

                    // Invariant - pos is just after closing quote
                    // Allows non-standard <tagName name="value"/> where the backslash is exactly after the quote
                    if (!isWhitespace(*pos) && *pos != '>' && *pos != '/') {
                        throw std::invalid_argument("No whitespace after attribute closing quote");
                    }

                    // Continues to either >, /> or another attribute
                    pos = skipWhitespace(pos);
                    if (*pos == '\0') {
                        throw std::invalid_argument("Premature end after attribute");
                    }
                }
            }

            bool isSelfClosing = false;
            // Invariant - pos is always at / or >
            if (*pos == '>') {
                pos++;
            } else if (*pos == '/') {
                if (isClosing) {
                    throw std::invalid_argument("Trying to double-close closing tag");
                }
                pos++;
                if (*pos != '>') {
                    throw std::invalid_argument("Invalid tag close - must have > after /");
                }
                isSelfClosing = true;
                pos++;
            } else {
                throw std::invalid_argument("No tag close");
            }

            // Invariant - pos always after valid tag close
            if (!isClosing) {
                // Invariant - pos is after > of a non-closing tag
                builder.preallocate<tags::GenericNode>();

                // Invariant - top stack node is always current parent
                if (!isSelfClosing) {
                    stack.push_back(hasher(tagName));
                }
            } else {
                // Invariant - when closing node, the current parent (stack top) must be of the node type
                size_t thisNode = stack.back();
                if (thisNode != hasher(tagName)) {
                    throw std::invalid_argument("Closing unopened tag");
                }
                if (stack.size() == 1) {
                    throw std::invalid_argument("Closing non-existent tags");
                }
                stack.pop_back();
            }
        }

        // Invariant - stack may only contain the root
        if (stack.size() != 1 && stack[0] != hasher(root)) {
            throw std::invalid_argument("Unclosed tags left");
        }

        return builder.build(); // first argument is temporarily nullptr until further improvements
    }

    ParseResult DomParser::parse(std::string_view input) {
        std::vector<Node*> stack;
        std::vector<std::string_view> attributeNames;
        std::vector<std::string_view> attributeValues;

        const char* pos = input.data();

        Arena arena = parseDryRun(input);

        Node* root = arena.allocate<tags::EmptyNode>();

        stack.push_back(root);

        pos = skipWhitespace(pos);

        while(*pos != '\0') {
            attributeNames.clear();
            attributeValues.clear();
            // Invariant - always at the start of either a tag or a sequence of text
            if (*pos != '<') {
                std::string_view text = extractText(pos);
                if (text.empty()) {
                    while (*pos != '\0') {
                        if (!isWhitespace(*pos)) throw std::invalid_argument("Invalid end after tag open");
                        pos++;
                    }
                    break;
                }
                pos += text.size();

                stack.back()->addChild(arena.allocate<tags::Text>(std::string(text)));

                continue;
            }

            // Invariant - always in tag name
            pos++;

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            bool isClosing = (*pos == '/');

            if (isClosing) pos++;

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            const char* commentCheckPos = checkSequence("!--", pos);
            if (commentCheckPos != pos) {
                pos = commentCheckPos;
                if (*pos == '\0') {
                    throw std::invalid_argument("Premature end of document");
                }
                const char* localPos = pos;

                while (!(*localPos == '-' && *(localPos + 1) != '\0' && *(localPos + 1) == '-')) {
                    localPos++;
                    if (*localPos == '\0') throw std::invalid_argument("Invalid comment without ending");
                }

                std::string_view commentText(pos, localPos-pos);

                localPos += 2;

                if (*localPos == '\0' || *localPos != '>') throw std::invalid_argument("-- inside of comment not allowed");
                pos = localPos;
                pos++;
                Node* newNode = arena.allocate<tags::Comment>(std::string(commentText));
                stack.back()->addChild(newNode);

                continue;
            }

            // Invariant - pos always at tag name start
            std::string_view tagName = readName(pos);
            if (tagName.empty()) {
                throw std::invalid_argument("Invalid tag name");
            }
            pos += tagName.size();
            
            bool couldHaveAttributes = isWhitespace(*pos) && !isClosing;

            // Invariant - pos always after tag name end
            pos = skipWhitespace(pos);

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            if (couldHaveAttributes) {
                // Invariant - either at start of attribute or at >
                while (*pos != '>' && *pos != '/') {
                    std::string_view attributeName = readName(pos);
                    if (attributeName.empty()) {
                        throw std::invalid_argument("Invalid non-closing tag");
                    }
                    pos += attributeName.size();

                    // Invariant - after attribute name
                    pos = skipWhitespace(pos);

                    // Invariant - at = or error state
                    if (*pos == '\0' || *pos != '=') {
                        throw std::invalid_argument("No = after attribute");
                    }
                    pos++;
                    // Invariant - after attribute =
                    pos = skipWhitespace(pos);
                    char attributeQuote = '\0';
                    if (*pos == '\0') {
                        throw std::invalid_argument("Premature end at attribute");
                    }

                    if (*pos == '\'' || *pos == '\"') {
                        attributeQuote = *pos;
                    } else {
                        throw std::invalid_argument("No quote (\" or \') after attribute =");
                    }
                    pos++;

                    // Invariant - after attribute value opening quote
                    std::string_view attributeValue = extractAttributeValue(pos, attributeQuote);
                    if (attributeValue.empty() && *pos != attributeQuote) {
                        throw std::invalid_argument("Improperly closed attribute value");
                    }
                    pos += attributeValue.size();

                    // Invariant - pos is at closing quote
                    pos++;

                    // Invariant - pos is just after closing quote
                    // Allows non-standard <tagName name="value"/> where the backslash is exactly after the quote
                    if (!isWhitespace(*pos) && *pos != '>' && *pos != '/') {
                        throw std::invalid_argument("No whitespace after attribute closing quote");
                    }

                    attributeNames.push_back(attributeName); 
                    attributeValues.push_back(attributeValue); 

                    // Continues to either >, /> or another attribute
                    pos = skipWhitespace(pos);
                    if (*pos == '\0') {
                        throw std::invalid_argument("Premature end after attribute");
                    }
                }
            }

            bool isSelfClosing = false;
            // Invariant - pos is always at / or >
            if (*pos == '>') {
                pos++;
            } else if (*pos == '/') {
                if (isClosing) {
                    throw std::invalid_argument("Trying to double-close closing tag");
                }
                pos++;
                if (*pos != '>') {
                    throw std::invalid_argument("Invalid tag close - must have > after /");
                }
                isSelfClosing = true;
                pos++;
            } else {
                throw std::invalid_argument("No tag close");
            }

            // Invariant - pos always after valid tag close
            if (!isClosing) {
                // Invariant - pos is after > of a non-closing tag
                Node* newNode = arena.allocate<tags::GenericNode>(std::string(tagName), isSelfClosing);

                // Invariant - attributeNames and attributeValues have the same size
                auto& attributes = newNode->attributes;
                for (int i = 0; i < attributeNames.size(); i++) {
                    attributes.emplace_back(std::string(attributeNames[i]), std::string(attributeValues[i]));
                }

                // Invariant - top stack node is always current parent
                stack.back()->addChild(newNode);
                if (!isSelfClosing) {
                    stack.push_back(newNode);
                }
            } else {
                // Invariant - when closing node, the current parent (stack top) must be of the node type
                Node* thisNode = stack.back();
                if (thisNode->getTagName() != tagName) {
                    throw std::invalid_argument("Closing unopened tag");
                }
                if (stack.size() == 1) {
                    throw std::invalid_argument("Closing non-existent tags");
                }
                stack.pop_back();
            }
        }

        // Invariant - stack may only contain the root
        if (stack.size() != 1 && stack[0] != root) {
            throw std::invalid_argument("Unclosed tags left");
        }

        if (root->getChildrenCount() == 1) {
            Node* newRoot = root->removeChild(root->getChildren()[0]).release();
            root = std::move(newRoot);
        }

        return ParseResult{std::move(arena), root};
    }
}