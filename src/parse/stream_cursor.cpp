#include "stream_cursor.h"

StreamCursor::StreamCursor(std::istream& is) : stream(is), currentChar('\0') {
    advance();
}

char StreamCursor::operator*() const { 
    return currentChar; 
}

void StreamCursor::operator++() { 
    advance(); 
}

void StreamCursor::advance() {
    if (peekCount > 0) {
        currentChar = peekBuffer[0];
        for(int i = 0; i < 3; i++) {
            peekBuffer[i] = peekBuffer[i+1];
        }
        peekCount--;
    } else if (!stream.get(currentChar)) {
        currentChar = '\0';
    }
}

char StreamCursor::peek(int offset = 1) {
    while (peekCount < offset) {
        char c;
        if (stream.get(c)) {
            peekBuffer[peekCount++] = c;
        } else {
            return '\0';
        }
    }
    return peekBuffer[offset - 1];
}

bool StreamCursor::isEOF() const { return currentChar == '\0'; }