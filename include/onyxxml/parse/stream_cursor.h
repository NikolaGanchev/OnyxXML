#pragma once
#include <string>
#include <istream>
#include <vector>

struct StreamCursor {
    using StringType = std::string;
    std::istream& stream;
    char currentChar;
    
    char peekBuffer[4]; 
    int peekCount = 0;

    StreamCursor(std::istream& is);

    char operator*() const;

    void operator++();

    void advance();

    char peek(int offset = 1);
    
    bool isEOF() const;
};