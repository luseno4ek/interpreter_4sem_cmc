#include "Parser.hpp"
#include "Lexeme.hpp"

template<class T, int max_size>
MyStack<T, max_size>::MyStack()
: top(0) {}

template<class T, int max_size>
void MyStack<T, max_size>::reset() { top = 0; }

template<class T, int max_size>
bool MyStack<T, max_size>::isempty() { return top == 0; }

template<class T, int max_size>
bool MyStack<T, max_size>::isfull() { return top == max_size; }

template<class T, int max_size>
void MyStack<T, max_size>::push(T item) {
    if(isfull()) {
        throw "Stack is full!";
    }
    stack[top] = item;
    top++;
}

template<class T, int max_size>
T MyStack<T, max_size>::pop() {
    if(isempty()) {
        throw "Stack is empty!";
    }
    top--;
    return stack[top];
}

/*/////////////////////////////////////////*/

Parser::Parser(const char* file_name)
: scan(file_name), poliz(1000) {}

/*/////////////////////////////////////////*/

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

void Parser::check_lparen() {
    if(curr_type != LEX_LPAREN) {
        throw curr_lex;
    }
    get_lex();
}

void Parser::check_rparen() {
    if(curr_type != LEX_RPAREN) {
        throw curr_lex;
    }
    get_lex();
}

void Parser::check_semicolon() {
    if(curr_type != LEX_SEMICOLON) {
        throw curr_lex;
    }
    get_lex();
}

void Parser::add_breakpoint(int loop_end) {
    if(curr_breakpoint != -1) {
        poliz.update_lex(Lex(POLIZ_LABEL, loop_end), curr_breakpoint);
        curr_breakpoint = -1;
    }
}

/*/////////////////////////////////////////*/

void Parser::declareID(TypeOfLex type) {
    int i;
    if(!st_int.isempty()) {
        i = st_int.pop();
        if(Scanner::Identifiers[i].get_declare()) {
            throw "ID is declared twice!";
        }
        Scanner::Identifiers[i].set_declare();
        Scanner::Identifiers[i].set_type(type);
    }
}

void Parser::check_id_declaration() {
    if(Scanner::Identifiers[curr_value].get_declare()) {
        st_lex.push(Scanner::Identifiers[curr_value].get_type());
    } else {
        throw "Identifier is not declared!";
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

void Parser::check_id_for_read() {
    if(!Scanner::Identifiers[curr_value].get_declare()) {
        throw "Identifier is not declared!";
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
    STRUCT_DEF();
    OPERATORS_BLOCK();
}
/*
Parser::Struct::Struct(char* _name, StructFields* _fields)
: name(_name), fields(_fields) {}

void Parser::STRUCT_DEF() {
    while(curr_type == LEX_STRUCT) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw "Struct name was expected!";
        }
        // check if is declared
        
        Identifiers[curr_value].set_type(LEX_STRUCT_DEF);
        // Identifiers[curr_value].set_value = position_in_array_of_structures
        char* curr_name = Identifiers[curr_value].get_name();
        Struct curr_struct(curr_name);
        get_lex();
        if(curr_type != LEX_BEGIN) {
            throw "'{' was expected!";
        }
        get_lex();
        curr_struct.fields = DEF_IN_STRUCT();
        structures[curr_value] = curr_struct;
        if(curr_type != LEX_END) {
            throw "'}' was expected!";
        }
        check_semicolon();
    }
}

Parser::StructFields* Parser::DEF_IN_STRUCT() {
    
}
*/

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

void Parser::OPERATORS_BLOCK() {
    do {
        OPERATORS();
    } while (curr_type != LEX_END);
}

void Parser::OPERATORS() {
    int point0, point1, point2, point3;
    if(curr_type == LEX_IF) {
        get_lex();
        check_lparen();
        EXPRESSION();
        check_is_expression_bool();
        point2 = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_FGO));
        check_rparen();
        OPERATORS();
        point3 = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_GO));
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point2);
        if(curr_type != LEX_ELSE) {
            throw "'else' expected!";
        }
        get_lex();
        OPERATORS();
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point3);
    } else if(curr_type == LEX_WHILE) {
        get_lex();
        check_lparen();
        point0 = poliz.get_free();
        EXPRESSION();
        check_is_expression_bool();
        point1 = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_FGO));
        check_rparen();
        OPERATORS();
        poliz.put_lex(Lex(POLIZ_LABEL, point0));
        poliz.put_lex(Lex(POLIZ_GO));
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point1);
        add_breakpoint(poliz.get_free());
    } else if(curr_type == LEX_FOR) {
        get_lex();
        check_lparen();
        if(curr_type != LEX_SEMICOLON) {
            EXPRESSION();
        }
        check_semicolon();
        point1 = poliz.get_free();
        if(curr_type != LEX_SEMICOLON) {
            EXPRESSION();
            check_is_expression_bool();
        } else {
            poliz.put_lex(Lex(LEX_TRUE));
        }
        point0 = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_FGO));
        check_semicolon();
        point3 = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_GO));
        if(curr_type != LEX_RPAREN) {
            EXPRESSION();
        }
        poliz.put_lex(Lex(POLIZ_LABEL, point1));
        poliz.put_lex(Lex(POLIZ_GO));
        check_rparen();
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point3);
        OPERATORS();
        poliz.put_lex(Lex(POLIZ_LABEL, point3 + 2));
        poliz.put_lex(Lex(POLIZ_GO));
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point0);
        add_breakpoint(poliz.get_free());
    } else if(curr_type == LEX_BREAK) {
        curr_breakpoint = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_GO));
        get_lex();
        check_semicolon();
    } else if(curr_type == LEX_READ) {
        get_lex();
        check_lparen();
        if(curr_type != LEX_ID) {
            throw curr_lex;
        }
        check_id_for_read();
        poliz.put_lex(Lex(POLIZ_ADDRESS, curr_value));
        get_lex();
        check_rparen();
        poliz.put_lex(Lex(LEX_READ));
        check_semicolon();
    } else if(curr_type == LEX_WRITE) {
        get_lex();
        check_lparen();
        EXPRESSION();
        poliz.put_lex(Lex(LEX_WRITE));
        while(curr_type == LEX_COMMA) {
            get_lex();
            EXPRESSION();
            poliz.put_lex(Lex(LEX_WRITE));
        }
        check_rparen();
        check_semicolon();
        poliz.put_lex(Lex(LEX_SEMICOLON));
    } else if(curr_type == LEX_BEGIN) {
        get_lex();
        OPERATORS_BLOCK();
        if(curr_type != LEX_END) {
            throw "'}' expected!";
        }
        get_lex();
    } else if(curr_type == LEX_GOTO) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw "Identificator expected!";
        }
        if(Scanner::Identifiers[curr_value].get_declare()) {
            throw "Label is declared as variable!";
        }
        get_lex();
        check_semicolon();
    } else if(curr_type == LEX_ID) {
        get_lex();
        if(curr_type != LEX_COLON) {
            throw "':' expected!";
        }
        get_lex();
        OPERATORS();
    } else {
        EXPRESSION();
        check_semicolon();
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
