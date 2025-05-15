#pragma once

#include "../node.h"
#include "../arena.h"
#include <stack>

namespace Templater::dynamic::parser {
    class DomParser;

    class ParseResult {
        friend DomParser;
        private:
            Arena arena;

            ParseResult();

            ParseResult(Arena arena, Node* root);
        public:
            Node* root;

            ParseResult(const ParseResult& other) = delete;
            ParseResult& operator=(const ParseResult& other) = delete;

            ParseResult(ParseResult&& other);
            ParseResult& operator=(ParseResult&& other);
    };

    class DomParser {
        private:
            static Arena parseDryRun(std::string_view input);
        public:
            static ParseResult parse(std::string_view input);
    };
}