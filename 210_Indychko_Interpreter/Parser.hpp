#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include <stack>
#include "Lexeme.hpp"
#include "Scanner.hpp"

class Parser {
//current lexeme:
    Lex curr_lex;
    TypeOfLex curr_type;
    int curr_value;
//scanner for getting lexemes:
    Scanner scan;
    std::stack<int> st_int;
    std::stack<TypeOfLex> st_lex;
    void GetLex();
//procedures for recursive descent:
    
    
//semantic analysis:
    
    
    
};

#endif /* Parser_hpp */
