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

Parser::StructFields::StructFields(char* _name, TypeOfLex _type)
: type(_type) { name = strdup(_name); }

Parser::Struct::Struct(char* _name) {
    fields = {};
    name = strdup(_name);
}

/*/////////////////////////////////////////*/

Parser::Parser(const char* file_name)
: scan(file_name), poliz(1000) {}

/*/////////////////////////////////////////*/

void Parser::get_lex() {
    if (!st_fail.isempty()){
        curr_lex = st_fail.pop();
    } else {
        curr_lex = scan.get_lex();
    }
    curr_type = curr_lex.get_type();
    curr_value = curr_lex.get_value();
}

void Parser::analysis() {
    get_lex();
    PROG();
    add_goto_labels();
//    poliz.print();
}

/*/////////////////////////////////////////*/

bool Parser::is_lex_type() {
    TypeOfLex t = curr_type;
    return (t == LEX_STRING || t == LEX_INT || t == LEX_BOOL);
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

void Parser::check_begin() {
    if(curr_type != LEX_BEGIN) {
        throw curr_lex;
    }
    get_lex();
}

void Parser::check_end() {
    if(curr_type != LEX_END) {
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

void Parser::check_colon() {
    if(curr_type != LEX_COLON) {
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

bool Parser::is_struct_name() {
    return curr_type == LEX_ID && Scanner::Identifiers[curr_value].get_type() == LEX_STRUCT_DEF;
}

bool Parser::is_operator_expression() {
    return curr_type == LEX_NOT || curr_type == LEX_MINUS || curr_type == LEX_TRUE || curr_type == LEX_FALSE || curr_type == LEX_NUM || curr_type == LEX_STRING_DATA || curr_type == LEX_LPAREN || (curr_type == LEX_ID && Scanner::Identifiers[curr_value].get_declare());
}

void Parser::add_goto_labels() {
    Ident curr_ident;
    for (int i = 1; i < Scanner::Identifiers.get_size(); i++) {
        curr_ident = Scanner::Identifiers[i];
        if(curr_ident.get_is_goto_label()) {
            if (!curr_ident.get_assign()) {
                throw "Used unassigned label";
            }
            poliz.update_lex(Lex(POLIZ_LABEL, curr_ident.get_value()), curr_ident.get_goto_label());
        }
    }
}

int Parser::length_of_struct(long long struct_type) const {
    long long num_in_list = Scanner::Identifiers[struct_type].get_value();
    auto strc = structs_definitions[num_in_list];
    return (int)strc.fields.size();
}

/*/////////////////////////////////////////*/

void Parser::declareID(TypeOfLex type) {
    long long i;
    if(!st_int.isempty()) {
        i = st_int.pop();
        if(Scanner::Identifiers[i].get_declare()) {
            throw "ID is declared twice!";
        }
        Scanner::Identifiers[i].set_declare();
        Scanner::Identifiers[i].set_type(type);
    }
}

void Parser::check_id_declaration(Lex curr_lex) {
    if (Scanner::Identifiers[curr_lex.get_value()].get_declare()) {
        if (Scanner::Identifiers[curr_lex.get_value()].get_type() == LEX_STRUCT) {
            TypeOfLex curr = static_cast<TypeOfLex>(Scanner::Identifiers[curr_lex.get_value()].get_value()
                + LEX_STRUCT_BASE);
            st_lex.push(curr);
        } else {
            st_lex.push(Scanner::Identifiers[curr_lex.get_value()].get_type());
        }
    } else {
        throw "Identifier is not declared!";
    }
}


void Parser::check_types_equality() {
    TypeOfLex curr_lex_type = st_lex.pop();
    if(curr_lex_type != st_lex.pop()) {
        throw "Incompatible types!";
    }
    st_lex.push(curr_lex_type);
}

void Parser::check_operand_for_not() {
    if(st_lex.pop() != LEX_BOOL) {
        throw "Wrong type for operation 'not'!";
    }
    st_lex.push(LEX_BOOL);
    poliz.put_lex(Lex(LEX_NOT));
}

void Parser::check_operand_for_unminus() {
    if(st_lex.pop() != LEX_INT) {
        throw "Wrong operand for unary minus!";
    }
    st_lex.push(LEX_INT);
    poliz.put_lex(Lex(LEX_UNARY_MINUS));
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
        poliz.put_lex(Lex(op));
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
        poliz.put_lex(Lex(op));
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
    st_lex.push(LEX_BOOL);
    poliz.put_lex(Lex(op));
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

void Parser::check_struct_fieds_def(const StructFields &_field, const std::vector<StructFields> _fields) {
    for(StructFields fld_count : _fields) {
        if (!strcmp(_field.name, fld_count.name)) {
            throw "Redifinition in structure!";
        }
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
    STRUCT_DEF();
    DEFINITIONS();
    OPERATORS_BLOCK(false);
    check_end();
    get_lex();
    if(curr_type != LEX_FIN) {
        throw curr_lex;
    }
}

void Parser::STRUCT_DEF() {
    while(curr_type == LEX_STRUCT) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw curr_lex;
        }
        if(Scanner::Identifiers[curr_value].get_declare()) {
            throw "Redifinition of structure!";
        }
        Scanner::Identifiers[curr_value].set_type(LEX_STRUCT_DEF);
        Scanner::Identifiers[curr_value].set_value(structs_definitions.size());
        structs_definitions.emplace_back(Scanner::Identifiers[curr_value].get_name());
        get_lex();
        check_begin();
        structs_definitions.back().fields = STRUCT_FIELD_DEF();
        check_end();
        check_semicolon();
    }
}

std::vector<Parser::StructFields> Parser::STRUCT_FIELD_DEF() {
    std::vector<StructFields> fields;
    while(is_lex_type()) {
        TypeOfLex var_type = curr_type;
        do {
            get_lex();
            if(curr_type != LEX_ID) {
                throw curr_lex;
            }
            StructFields new_field(Scanner::Identifiers[curr_value].get_name(), var_type);
            check_struct_fieds_def(new_field, fields);
            fields.push_back(new_field);
            Scanner::Identifiers.pop();
            get_lex();
        } while(curr_type == LEX_COMMA);
        check_semicolon();
    }
    return fields;
}

void Parser::DEFINITIONS() {
    st_int.reset();
    while(is_lex_type() || is_struct_name()) {
        if(is_struct_name()) {
            DEF(Lex(LEX_STRUCT, curr_value));
        } else {
            DEF(curr_lex);
        }
                                    //after returning from DEF(): curr_type == LEX_SEMICOLON =>
        get_lex();                  //get new lexeme
    }                               //if we are here, we got new lexeme, which is not type =>
}                                   //operators' part has started

void Parser::DEF(Lex saved_lex) {
    get_lex();
    if(curr_type != LEX_ID) {
        throw "Type without variable!";
    } else {
        VAR(saved_lex);
    }
    while(curr_type == LEX_COMMA) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw "Missing variable!";
        }
        VAR(saved_lex);
    }
    if(curr_type != LEX_SEMICOLON) {
        throw curr_lex;
    }
}

void Parser::VAR(Lex saved_lex) {
    if(saved_lex.get_type() == LEX_STRUCT) {
        Scanner::Identifiers[curr_value].set_type(LEX_STRUCT);
        Scanner::Identifiers[curr_value].set_value(saved_lex.get_value());
        Scanner::Identifiers[curr_value].set_declare();
        const char* struct_name = Scanner::Identifiers[curr_value].get_name();
        long long num_struct = Scanner::Identifiers[saved_lex.get_value()].get_value();
            for(auto fld : structs_definitions[num_struct].fields) {
                const char* name = strcat(strcat(strdup(struct_name), "."), fld.name);
                int i = Scanner::Identifiers.add(name);
                Ident& new_id = Scanner::Identifiers[i];
                new_id.set_declare();
                new_id.set_type(fld.type);
                new_id.set_value(0);
            }
        get_lex();
    } else {
        st_int.push(curr_value);
        declareID(saved_lex.get_type());
        Lex save_var = curr_lex;
        get_lex();
        CONST_INIT(save_var);
    }
}

void Parser::CONST_INIT(Lex saved_var) {
    if (curr_type != LEX_ASSIGN || saved_var.get_type() == LEX_STRUCT) {
        return;
    }
    st_fail.push(curr_lex);
    st_fail.push(saved_var);
    get_lex();
    EXPRESSION();
}

void Parser::OPERATORS_BLOCK(bool inside_loop) {
    do {
        OPERATORS(inside_loop);
    } while (curr_type != LEX_END);
}

void Parser::OPERATORS(bool inside_loop) {
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
        OPERATORS(inside_loop);
        point3 = poliz.get_free();
        poliz.blank();
        poliz.put_lex(Lex(POLIZ_GO));
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point2);
        if(curr_type != LEX_ELSE) {
            throw "'else' expected!";
        }
        get_lex();
        OPERATORS(inside_loop);
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
        OPERATORS(true);
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
        OPERATORS(true);
        poliz.put_lex(Lex(POLIZ_LABEL, point3 + 2));
        poliz.put_lex(Lex(POLIZ_GO));
        poliz.update_lex(Lex(POLIZ_LABEL, poliz.get_free()), point0);
        add_breakpoint(poliz.get_free());
    } else if(curr_type == LEX_BREAK) {
        if(!inside_loop) {
            throw curr_lex;
        }
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
        OPERATORS_BLOCK(inside_loop);
        check_end();
    } else if(curr_type == LEX_GOTO) {
        get_lex();
        if(curr_type != LEX_ID) {
            throw "Identificator expected!";
        }
        Ident& curr_label = Scanner::Identifiers[curr_value];
        if(curr_label.get_declare()) {
            throw "Label is declared as variable!";
        }
        if (curr_label.get_assign()) {
            poliz.put_lex(Lex(POLIZ_LABEL, curr_label.get_value()));
        }
        else {
            curr_label.set_is_goto_label();
            curr_label.set_goto_label(poliz.get_free());
            poliz.blank();
        }
        poliz.put_lex(Lex(POLIZ_GO));
        get_lex();
        check_semicolon();
    } else if(curr_type == LEX_ID && !Scanner::Identifiers[curr_value].get_declare()) {
        if(Scanner::Identifiers[curr_value].get_assign()) {
            throw "Redefinition of label!";
        }
        Scanner::Identifiers[curr_value].set_value(poliz.get_free());
        Scanner::Identifiers[curr_value].set_assign();
        get_lex();
        check_colon();
        OPERATORS(inside_loop);
    } else if(is_operator_expression()) {
        EXPRESSION();
        check_semicolon();
        poliz.put_lex(Lex(LEX_SEMICOLON));
    } else {
        throw curr_lex;
    }
}

void Parser::EXPRESSION() {
    if (curr_type == LEX_ID) {
        Lex saved_lex = curr_lex;
        get_lex();
        if (curr_type != LEX_ASSIGN) {
            st_fail.push(curr_lex);
            st_fail.push(saved_lex);
            get_lex();
            SIMPLE_EXPRESSION();
            return;
        }
        check_id_declaration(saved_lex);
        poliz.put_lex(Lex(POLIZ_ADDRESS, saved_lex.get_value()));
        get_lex();
        EXPRESSION();
        check_types_equality();
        poliz.put_lex(Lex(LEX_ASSIGN));
    }
    else {
        SIMPLE_EXPRESSION();
    }
    
}

void Parser::SIMPLE_EXPRESSION() {
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
        check_id_declaration(curr_lex);
        poliz.put_lex(curr_lex);
        get_lex();
    } else if(curr_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        poliz.put_lex(curr_lex);
        get_lex();
    } else if(curr_type == LEX_STRING_DATA) {
        st_lex.push(LEX_STRING);
        poliz.put_lex(curr_lex);
        get_lex();
    } else if(curr_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.put_lex(curr_lex);
        get_lex();
    } else if(curr_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.put_lex(curr_lex);
        get_lex();
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
        get_lex();
    } else {
        throw curr_lex;
    }
}
