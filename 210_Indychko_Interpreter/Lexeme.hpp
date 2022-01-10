#ifndef Lexeme_hpp
#define Lexeme_hpp

#include <iostream>

enum TypeOfLex {
    LEX_NULL,
    LEX_PROGRAM,
    LEX_STRING, LEX_INT, LEX_BOOL, LEX_STRUCT, LEX_STRUCT_DEF,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_FOR, LEX_WHILE,
    LEX_BREAK, LEX_GOTO,
    LEX_FALSE, LEX_TRUE,
    LEX_NOT, LEX_OR, LEX_AND,
    LEX_READ, LEX_WRITE,
    LEX_FIN, LEX_BEGIN, LEX_END,
    LEX_MULTIPLY, LEX_DIVISION, LEX_PLUS,
    LEX_MINUS, LEX_LESS, LEX_GR, LEX_LEQ,
    LEX_GEQ, LEX_EQ, LEX_NEQ, LEX_ASSIGN, LEX_SEMICOLON,
    LEX_COMMA, LEX_DOT, LEX_COLON, LEX_LPAREN, LEX_RPAREN,
    LEX_NUM,         // number
    LEX_ID,          // identificator
    LEX_STRING_DATA, // string
    LEX_UNARY_MINUS,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO,
    LEX_STRUCT_BASE = 100
};

class Lex {
private:
    TypeOfLex type_lex;
    long long value_lex;
public:
    Lex(TypeOfLex _type = LEX_NULL, long long _value = 0);
    TypeOfLex get_type();
    long long get_value();
    friend std::ostream& operator<<(std::ostream& s, Lex lexeme);
    int IndexOf(const TypeOfLex* arr, TypeOfLex x);
};

#endif /* Lexeme_hpp */
