#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include "Lexeme.hpp"
#include "Scanner.hpp"
#include "MyStack.hpp"
#include "Polizzz.hpp"

class Parser {
private:
/*/structures:
    struct StructFields {
        char* name;
        TypeOfLex type;
        StructFields(char* _name, TypeOfLex _type);
    };
    struct Struct {
        char* name;
        StructFields* fields;
        Struct(char* _name, StructFields* _fields = nullptr);
    }; */
//current lexeme:
    Lex curr_lex;
    TypeOfLex curr_type;
    int curr_value;
//check break in loops
    int curr_breakpoint = -1;
//scanner for getting lexemes:
    Scanner scan;
    MyStack<int, 100> st_int;
    MyStack<TypeOfLex, 100> st_lex;
    void get_lex();
//procedures for recursive descent:
    void PROG();
    void STRUCT_DEF();
   // StructFields* DEF_IN_STRUCT();
    void DEFINITIONS();
    void DEF(TypeOfLex type);
    void VAR(TypeOfLex type);
    void CONST();
    void OPERATORS();
    void EXPRESSION();
    void SUMMATION();
    void MULTIPLICATION();
    void OPERANDS();
//semantic analysis:
    void declareID(TypeOfLex type);
    void check_types_equality();
    void check_id_declaration();
    void check_operand_for_not();
    void check_operand_for_unminus();
    void check_operands_for_multiply();
    void check_operands_for_summation();
    void check_operands_for_comparison();
    void check_is_expression_bool();
    void check_id_for_read();
//helpful procedures:
    bool is_lex_type();
    bool is_lex_multiplication();
    bool is_lex_summation();
    bool is_lex_comparison();
    void check_rparen();
    void check_lparen();
    void check_semicolon();
    void add_breakpoint(int loop_end);
public:
    Poliz poliz;
    void analysis();
 //   TableIdent structures[100];
    Parser(const char* file_name);
};

#endif /* Parser_hpp */
