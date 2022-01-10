#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include "Lexeme.hpp"
#include "Scanner.hpp"
#include "MyStack.hpp"
#include "Polizzz.hpp"
#include <vector>

class Parser {
private:
//structures:
    struct StructFields {
        char* name;
        TypeOfLex type;
        StructFields(char* _name, TypeOfLex _type);
    };
    struct Struct {
        char* name;
        std::vector<StructFields> fields;
        Struct(char* _name);
    };
//current lexeme:
    Lex curr_lex;
    TypeOfLex curr_type;
    long long curr_value;
//check break in loops
    int curr_breakpoint = -1;
//scanner for getting lexemes:
    Scanner scan;
    MyStack<long long, 100> st_int;
    MyStack<TypeOfLex, 100> st_lex;
    MyStack<Lex, 3> st_fail;
    void get_lex();
//structures holding:
    std::vector<Struct> structs_definitions;
//procedures for recursive descent:
    void PROG();
    void STRUCT_DEF();
    std::vector<StructFields> STRUCT_FIELD_DEF();
    void DEFINITIONS();
    void DEF(Lex saved_lex);
    void VAR(Lex saved_lex);
    void CONST_INIT(Lex saved_var);
    void OPERATORS_BLOCK(bool inside_loop);
    void OPERATORS(bool inside_loop);
    void EXPRESSION();
    void SIMPLE_EXPRESSION();
    void SUMMATION();
    void MULTIPLICATION();
    void OPERANDS();
//semantic analysis:
    void declareID(TypeOfLex type);
    void check_types_equality();
    void check_id_declaration(Lex curr_lex);
    void check_operand_for_not();
    void check_operand_for_unminus();
    void check_operands_for_multiply();
    void check_operands_for_summation();
    void check_operands_for_comparison();
    void check_is_expression_bool();
    void check_id_for_read();
    void check_struct_fieds_def(const StructFields& _field, const std::vector<StructFields> _fields);
//helpful procedures:
    bool is_lex_type();
    bool is_lex_multiplication();
    bool is_lex_summation();
    bool is_lex_comparison();
    bool is_struct_name();
    bool is_operator_expression();
    void check_rparen();
    void check_lparen();
    void check_colon();
    void check_begin();
    void check_end();
    void check_semicolon();
    void add_breakpoint(int loop_end);
    void add_goto_labels();
public:
    Poliz poliz;
    void analysis();
    Parser(const char* file_name);
    int length_of_struct(long long struct_type) const;
};

#endif /* Parser_hpp */
