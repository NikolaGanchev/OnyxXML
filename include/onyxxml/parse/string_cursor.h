#pragma once
#include <string>
#include <string_view>

struct StringCursor {
    using StringType = std::string_view;
    const char* ptr;

    StringCursor(const char* p);

    char operator*() const;
    void operator++();
    
    char peek(int offset = 1) const;
    
    StringType getStringFrom(const char* start) const;
    
    bool isEOF() const;
};