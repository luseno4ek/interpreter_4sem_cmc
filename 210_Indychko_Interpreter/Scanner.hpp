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
        COMMAND,
        ALE,
        DELIM,
        NEQ
    };
    static const char* ServiceWords[];
    static const char* LimSymbols[];
    static TypeOfLex words[];
    static TypeOfLex symbols[];
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
    int look(const char* _buf, char** list);
    ///gets char c from FILE* fp
    void gc();
public:
    Scanner(const char* program);
    Lex get_lex();
};

/*/////////////////////////////////////////*/

const char* Scanner::ServiceWords[] = {
    ""
    "program",
    "int",
    "string",
    "if",
    "else",
    "while",
    "do",
    "false",
    "true",
    "read",
    "write",
    "not",
    "and",
    "or",
    "continue",
    "boolean",
    NULL
};

/*/////////////////////////////////////////*/

const char* Scanner::LimSymbols[] = {
    ""
    "*",
    "/",
    "+",
    "-",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "=",
    ";",
    ",",
    ":",
    "(",
    ")",
    NULL
};

/*/////////////////////////////////////////*/

TableIdent Identifiers(100);

/*/////////////////////////////////////////*/

TypeOfLex Scanner::words[] = {
    LEX_NULL,
    LEX_PROGRAM,
    LEX_END, LEX_BEGIN,
    LEX_STRING, LEX_INT,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_DO, LEX_WHILE,
    LEX_FALSE, LEX_TRUE,
    LEX_NOT, LEX_OR, LEX_AND,
    LEX_READ, LEX_WRITE,
    LEX_NULL
};

/*/////////////////////////////////////////*/

TypeOfLex Scanner::symbols[] = {
    LEX_FIN,
    LEX_SEMICOLON, // ;
    LEX_COMMA,     // ,
    LEX_COLON,     // :
  //  LEX_ASSIGN,
  //  LEX_LPAREN,
  //  LEX_RPAREN,
  //  LEX_EQ,
  //  LEX_LSS, LEX_GTR,
    LEX_PLUS, LEX_MINUS,
  //  LEX_TIMES,
  //  LEX_SLASH, LEX_LEQ,
    LEX_NEQ,
  //  LEX_GEQ,
    LEX_NULL
};

#endif /* Scanner_hpp */
