#ifndef Scanner_hpp
#define Scanner_hpp

#include "Lexeme.hpp"
#include <stdio.h>

class Scanner {
private:
    enum State {
        INIT,
        ID,
        NUMBER,
        COMMAND,
        ALE,
        DELIM,
        NEQ
    };
    static char* ServiceWords[];
    static char* LimSymbols[];
    static TypeOfLex words[];
    static TypeOfLex dlms[];
    State curr_state;
    FILE* fp;
    char c;
    char buf[20];
    int buf_top;
    void clear();
    void add();
    int look(const char* _buf, char** list);
    void gc();
public:
    Lex get_lex();
    Scanner(const char* program);
};



#endif /* Scanner_hpp */
