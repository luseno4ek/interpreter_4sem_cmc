#include "Parser.hpp"
#include "Lexeme.hpp"

void Parser::get_lex() {
    curr_lex = scan.get_lex();
    curr_type = curr_lex.get_type();
    curr_value = curr_lex.get_value();
}

void Parser::analysis() {
    get_lex();
    PROG();
}

/*/////////////////////////////////////////*/

void Parser::declareID(TypeOfLex type) {
    int i;
    if(!st_int.isempty()) {
        i = st_int.pop();
        if(Identifiers[i].get_declare()) {
            throw "ID is declared twice!";
        }
        Identifiers[i].set_declare();
        Identifiers[i].set_type(type);
    }
}

/*/////////////////////////////////////////*/


void Parser::PROG() {
    if(curr_type == LEX_PROGRAM) {
        get_lex();
    } else {
        throw curr_lex;
    }
    if(curr_type == LEX_BEGIN) {
        get_lex();
    } else {
        throw curr_lex;
    }
    
}

void Parser::DEFS() {
    if(curr_type == LEX_STRING || curr_type == LEX_INT || curr_type == LEX_BOOL || curr_type == LEX_STRUCT) {
        
    }
}
