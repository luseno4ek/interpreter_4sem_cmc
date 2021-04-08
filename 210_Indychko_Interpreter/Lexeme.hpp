#ifndef Lexeme_hpp
#define Lexeme_hpp

#include <stdio.h>

enum TypeOfLex {
    LEX_NULL,
    LEX_PROGRAM,
    LEX_END, LEX_BEGIN,
    LEX_STRING, LEX_INT,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_DO, LEX_WHILE,
    LEX_FALSE, LEX_TRUE,
    LEX_NOT, LEX_OR, LEX_AND,
    LEX_READ, LEX_WRITE,
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
    LEX_NUM,       // number
    LEX_ID         // identificator
};

class Lex {
private:
    TypeOfLex type_lex;
    int value_lex;
public:
    Lex(TypeOfLex _type = LEX_NULL, int _value = 0);
    TypeOfLex get_type();
    int get_value();
};

#endif /* Lexeme_hpp */
