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

bool Parser::is_lex_type(TypeOfLex type) {
    return (type == LEX_STRING || type == LEX_INT || type == LEX_BOOL || type == LEX_STRUCT);
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
    DEFS();
    
}

void Parser::DEFS() {
    st_int.reset();
    while(is_lex_type(curr_type)) {
        if(curr_type == LEX_INT) {
            DEF(LEX_INT);
        } else if(curr_type == LEX_STRING) {
            DEF(LEX_STRING);
        } else if(curr_type == LEX_BOOL) {
            DEF(LEX_BOOL);
        } else {
            DEF(LEX_STRUCT);
        }
//after returning from DEF(): curr_type == LEX_SEMICOLON => get new lexeme
        get_lex();
    }
//if we are here, we got new lexeme, which is not type => operators' part has started
}

void Parser::DEF(TypeOfLex type) {
    get_lex();
    if(curr_type != LEX_ID) {
        throw "Type without variable!";
    }
    st_int.push(curr_value);
    declareID(type);
    get_lex();
    while(curr_type == LEX_COMMA) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw "Missing variable!";
        }
        st_int.push(curr_value);
        declareID(type);
        get_lex();
    }
    if(curr_type != LEX_SEMICOLON) {
        throw curr_lex;
    }
}
