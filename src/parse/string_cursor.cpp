#include "string_cursor.h"

StringCursor::StringCursor(const char* p) : ptr(p) {}

char StringCursor::operator*() const { 
    return *ptr; 
}

void StringCursor::operator++() { 
    ptr++; 
}

char StringCursor::peek(int offset) const { 
    return *(ptr + offset); 
}

StringCursor::StringType StringCursor::getStringFrom(const char* start) const {
    return std::string_view(start, ptr - start);
}

bool StringCursor::isEOF() const { 
    return *ptr == '\0'; 
}