#ifndef Scanner_hpp
#define Scanner_hpp

#include "Identifier.hpp"
#include <stdio.h>

class Scanner {
private:
    enum State {
        INIT,
        ID,
        NUMBER,
        STRING, 
        DOUBLESYMBOL, // !=, <=, >=, ==
        SYMBOL,
        COMMENTARY
    };
    State curr_state;
    FILE* fp;
    char c;
    char buf[80];
    int buf_top;
    ///clears the buffer
    void clear();
    ///adds char c to the top of buffer
    void add();
    ///looks if _buf is in list and returns its position (or 0)
    int look(const char* _buf, const char** list);
    ///gets char c from FILE* fp
    void gc();
    ///returns 1 if c from [a, z]
    bool isalpha(char c);
    ///returns 1 if c from [0, 9]
    bool isdigit(char c);
public:
    Scanner(const char* program);
    Lex get_lex();
    void Print_Result();
    static const char* ServiceWords[];
    static const char* LimSymbols[];
    static TypeOfLex words[];
    static TypeOfLex symbols[];
    static TableIdent Identifiers;
    static TableIdent StringData;
};

#endif /* Scanner_hpp */
