#include "dom_parser.h"
#include <vector>
#include "../nodes/generic_node.h"
#include "../nodes/empty_node.h"
#include "../nodes/text_node.h"
#include <iostream>

namespace Templater::dynamic::parser {
    ParseResult::ParseResult(): buffer{nullptr}, root{nullptr} {}

    ParseResult::ParseResult(char* buffer, std::unique_ptr<Node> root): buffer{buffer}, root{std::move(root)} {}

    ParseResult::ParseResult(ParseResult&& other) {
        this->buffer = other.buffer;
        other.buffer = nullptr;
        this->root = std::move(other.root);
    }

    ParseResult& ParseResult::operator=(ParseResult&& other) {
        root.reset();
        delete[] this->buffer;

        this->buffer = other.buffer;
        other.buffer = nullptr;
        this->root = std::move(other.root);

        return *this;
    }


    ParseResult::~ParseResult() {
        root.reset();
        delete[] buffer;
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

    std::string_view readTagName(const char* pos) {
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

    ParseResult DomParser::parse(std::string_view input) {
        std::vector<Node*> stack;

        const char* pos = input.data();

        std::unique_ptr<Node> root = std::make_unique<tags::EmptyNode>();

        stack.push_back(root.get());

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

                stack.back()->addChild(tags::Text(std::string(text)));

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

            // Invariant - pos always at tag name start
            std::string_view tagName = readTagName(pos);
            if (tagName.empty()) {
                throw std::invalid_argument("Invalid tag name");
            }
            pos += tagName.size();

            // Invariant - pos always after tag name end

            pos = skipWhitespace(pos);

            if (*pos == '\0') {
                throw std::invalid_argument("Premature end of document");
            }

            bool isSelfClosing = false;
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
                throw std::invalid_argument("Invalid tag close - tag is not closed at all");
            }

            // Invariant - pos always after valid tag close

            if (!isClosing) {
                // Invariant - pos is after > of a non-closing tag
                Node* newNode = new tags::GenericNode(std::string(tagName), isSelfClosing);

                // Invariant - top stack node is always current parent
                stack.back()->addChild(std::unique_ptr<Node>{newNode});
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
        if (stack.size() != 1 && stack[0] != root.get()) {
            throw std::invalid_argument("Unclosed tags left");
        }

        if (root->getChildrenCount() == 1) {
            std::unique_ptr<Node> newRoot = root->removeChild(root->getChildren()[0]);
            root = std::move(newRoot);
        }

        return ParseResult{nullptr, std::move(root)}; // first argument is temporarily nullptr until further improvements
    }
}