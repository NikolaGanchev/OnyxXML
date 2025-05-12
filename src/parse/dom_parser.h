#pragma once

#include "../node.h"

namespace Templater::dynamic::parser {
    class DomParser;

    class ParseResult {
        friend DomParser;
        private:
            char* buffer;

            ParseResult();

            ParseResult(char* buffer, std::unique_ptr<Node> root);
        public:
            std::unique_ptr<Node> root;

            ParseResult(const ParseResult& other) = delete;
            ParseResult& operator=(const ParseResult& other) = delete;

            ParseResult(ParseResult&& other);
            ParseResult& operator=(ParseResult&& other);

            ~ParseResult();
    };

    class DomParser {
        public:
            static ParseResult parse(std::string_view input);
    };
}