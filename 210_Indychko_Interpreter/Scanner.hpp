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
        SYMBOL
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
    int look(const char* _buf, const char** list);
    ///gets char c from FILE* fp
    void gc();
    ///returns 1 if c from [a,z]
    bool isalpha(char c);
    ///returns 1 if c from [0,9]
    bool isdigit(char c);
public:
    Scanner(const char* program);
    Lex get_lex();
};

/*/////////////////////////////////////////*/

const char* Scanner::ServiceWords[] = {
    ""
    "program",
    "string",
    "int",
    "boolean",
    "struct",
    "if",
    "then",
    "else",
    "for",
    "while",
    "break",
    "goto",
    "false",
    "true",
    "not",
    "or",
    "and",
    "read",
    "write",
    NULL
};

/*/////////////////////////////////////////*/

const char* Scanner::LimSymbols[] = {
    ""
    "@",
    "{",
    "}",
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
TableIdent Labels(100);
TableString StringsData;

/*/////////////////////////////////////////*/

TypeOfLex Scanner::words[] = {
    LEX_NULL,
    LEX_PROGRAM,
    LEX_STRING, LEX_INT, LEX_BOOL, LEX_STRUCT,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_FOR, LEX_WHILE,
    LEX_BREAK, LEX_GOTO,
    LEX_FALSE, LEX_TRUE,
    LEX_NOT, LEX_OR, LEX_AND,
    LEX_READ, LEX_WRITE,
    LEX_NULL
};

/*/////////////////////////////////////////*/

TypeOfLex Scanner::symbols[] = {
    LEX_NULL,
    LEX_FIN, LEX_BEGIN, LEX_END,
    LEX_MULTIPLY, LEX_DIVISION, LEX_PLUS,
    LEX_MINUS, LEX_LESS, LEX_GR, LEX_LEQ,
    LEX_GEQ, LEX_EQ, LEX_NEQ, LEX_ASSIGN,
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON,
    LEX_LPAREN, LEX_RPAREN,
    LEX_NULL
};

#endif /* Scanner_hpp */
