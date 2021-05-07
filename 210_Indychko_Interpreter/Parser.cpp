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

void Parser::check_types_equality() {
    if(st_lex.pop() != st_lex.pop()) {
        throw "Incompatible types!";
    }
}

void Parser::check_operand_for_not() {
    if(st_lex.pop() != LEX_BOOL) {
        throw "Wrong type for operation 'not'!";
    }
    st_lex.push(LEX_BOOL);
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
    DEFINITIONS();
    OPERATORS();
}

void Parser::DEFINITIONS() {
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
        }                           //after returning from DEF(): curr_type == LEX_SEMICOLON =>
        get_lex();                  //get new lexeme
    }                               //if we are here, we got new lexeme, which is not type =>
}                                   //operators' part has started

void Parser::CONST() {
    if(curr_type == LEX_NUM) {
        st_lex.push(LEX_INT);
    } else if(curr_type == LEX_STRING_DATA) {
        st_lex.push(LEX_STRING);
    } else if(curr_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
    } else if(curr_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
    } else if(curr_type == LEX_PLUS || curr_type == LEX_MINUS) {
        get_lex();
        if(curr_type == LEX_NUM) {
            st_lex.push(LEX_INT);
        }
    } else {
        throw "Wrong type for constant initialization!";
    }
    get_lex();
}

void Parser::DEF(TypeOfLex type) {
    get_lex();
    if(curr_type != LEX_ID) {
        throw "Type without variable!";
    } else {
        VAR(type);
    }
    while(curr_type == LEX_COMMA) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw "Missing variable!";
        }
        VAR(type);
    }
    if(curr_type != LEX_SEMICOLON) {
        throw curr_lex;
    }
}

void Parser::VAR(TypeOfLex type) {
    st_int.push(curr_value);
    st_lex.push(type);
    declareID(type);
    get_lex();
    if(curr_type == LEX_ASSIGN) {
        get_lex();
        CONST();
        check_types_equality();
    } else {
        st_lex.pop();
    }
}

void Parser::OPERATORS() {
    if(curr_type == LEX_IF) {
        get_lex();
        EXPRESSION();
    }
}

void Parser::EXPRESSION() {
    SUMMATION();
    
}

void Parser::SUMMATION() {
    MULTIPLICATION();
    
}

void Parser::MULTIPLICATION() {
    OPERANDS();
    
}

void Parser::OPERANDS() {
    if(curr_type == LEX_ID) {
        check_id_declaration();
    } else if(curr_type == LEX_NUM) {
        st_lex.push(LEX_INT);
    } else if(curr_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
    } else if(curr_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
    } else if(curr_type == LEX_NOT) {
        get_lex();
        OPERANDS();
        check_operand_for_not();
    } else if(curr_type == LEX_LPAREN) {
        get_lex();
        EXPRESSION();
        if(curr_type != LEX_RPAREN) {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
    get_lex();
}
