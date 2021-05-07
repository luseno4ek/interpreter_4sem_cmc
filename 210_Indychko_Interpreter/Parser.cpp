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

bool Parser::is_lex_type() {
    TypeOfLex t = curr_type;
    return (t == LEX_STRING || t == LEX_INT || t == LEX_BOOL || t == LEX_STRUCT);
}

bool Parser::is_lex_multiplication() {
    TypeOfLex t = curr_type;
    return (t == LEX_MULTIPLY || t == LEX_DIVISION || t == LEX_AND);
}

bool Parser::is_lex_summation() {
    TypeOfLex t = curr_type;
    return (t == LEX_OR || t == LEX_MINUS || t == LEX_PLUS);
}

bool Parser::is_lex_comparison() {
    TypeOfLex t = curr_type;
    return (t == LEX_EQ || t == LEX_NEQ || t == LEX_GEQ || t == LEX_LEQ || t == LEX_LESS || t == LEX_GR);
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

void Parser::check_operand_for_unminus() {
    if(st_lex.pop() != LEX_INT) {
        throw "Wrong operand for unary minus!";
    }
    st_lex.push(LEX_INT);
}

void Parser::check_operands_for_multiply() {
    TypeOfLex operand1, operand2, op;
    TypeOfLex check_type = LEX_INT, result_type = LEX_BOOL;
    operand2 = st_lex.pop();
    op = st_lex.pop();
    operand1 = st_lex.pop();
    if(op == LEX_DIVISION || op == LEX_MULTIPLY) {
        result_type = LEX_INT;
    }
    if(op == LEX_AND) {
        check_type = LEX_BOOL;
    }
    if(operand1 == operand2 && operand2 == check_type) {
        st_lex.push(result_type);
    } else {
        throw "Wrong types in operation!";
    }
}

void Parser::check_operands_for_summation() {
    TypeOfLex operand1, operand2, op;
    TypeOfLex check_type = LEX_INT, result_type = LEX_BOOL;
    operand2 = st_lex.pop();
    op = st_lex.pop();
    operand1 = st_lex.pop();
    if(op == LEX_PLUS || op == LEX_MINUS) {
        if(operand1 == LEX_STRING) {
            result_type = LEX_STRING;
            check_type = LEX_STRING;
        } else {
            result_type = LEX_INT;
        }
    }
    if(op == LEX_AND) {
        check_type = LEX_BOOL;
    }
    if(operand1 == operand2 && operand2 == check_type) {
        st_lex.push(result_type);
    } else {
        throw "Wrong types in operation!";
    }
}

void Parser::check_operands_for_comparison() {
    TypeOfLex operand1, operand2, op;
    operand2 = st_lex.pop();
    op = st_lex.pop();
    operand1 = st_lex.pop();
    if(operand1 != operand2) {
        throw "Wrong types in operation!";
    }
    if(operand1 == LEX_STRING || operand1 == LEX_BOOL) {
        if(op == LEX_LEQ || op == LEX_GEQ) {
            throw "Wrong types in operation!";
        }
    }
}

void Parser::check_is_expression_bool() {
    if(st_lex.pop() != LEX_BOOL) {
        throw "Expression is not bool!";
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
    DEFINITIONS();
    OPERATORS();
}

void Parser::DEFINITIONS() {
    st_int.reset();
    while(is_lex_type()) {
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
        if(curr_type != LEX_LPAREN) {
            throw curr_lex;
        }
        get_lex();
        EXPRESSION();
        check_is_expression_bool();
        if(curr_type != LEX_RPAREN) {
            throw curr_lex;
        }
        get_lex();
        OPERATORS();
        if(curr_type != LEX_ELSE) {
            throw curr_lex;
        }
        get_lex();
        OPERATORS();
    }
}

void Parser::EXPRESSION() {
    SUMMATION();
    if(is_lex_comparison()) {
        st_lex.push(curr_type);
        get_lex();
        SUMMATION();
        check_operands_for_comparison();
    }
    
}

void Parser::SUMMATION() {
    MULTIPLICATION();
    while(is_lex_summation()) {
        st_lex.push(curr_type);
        get_lex();
        MULTIPLICATION();
        check_operands_for_summation();
    }
    
}

void Parser::MULTIPLICATION() {
    OPERANDS();
    while(is_lex_multiplication()) {
        st_lex.push(curr_type);
        get_lex();
        OPERANDS();
        check_operands_for_multiply();
    }
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
    } else if(curr_type == LEX_MINUS) {
        get_lex();
        OPERANDS();
        check_operand_for_unminus();
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
