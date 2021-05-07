#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include "Lexeme.hpp"
#include "Scanner.hpp"
#include "MyStack.hpp"

class Parser {
private:
//current lexeme:
    Lex curr_lex;
    TypeOfLex curr_type;
    int curr_value;
//scanner for getting lexemes:
    Scanner scan;
    MyStack<int, 100> st_int;
    MyStack<TypeOfLex, 100> st_lex;
    void get_lex();
//procedures for recursive descent:
    void PROG();
    void DEFS();
    void DEF(TypeOfLex type);
    void OPER();
//semantic analysis:
    void declareID(TypeOfLex type);
//helpful procedures:
    bool is_lex_type(TypeOfLex type);
public:
    void analysis();
};

#endif /* Parser_hpp */
