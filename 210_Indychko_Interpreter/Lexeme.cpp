#include "Lexeme.hpp"
#include "Scanner.hpp"
#include "cstring"
   
Lex::Lex(TypeOfLex _type, long long _value)
: type_lex(_type), value_lex(_value) {}

TypeOfLex Lex::get_type() { return type_lex; }

long long Lex::get_value() { return value_lex; }

int IndexOf(const TypeOfLex* arr, TypeOfLex x) {
    int i = 0;
    while (arr[i] != x){
        i++;
    }
    return i;
}

std::ostream& operator<<(std::ostream& s, Lex lexeme) {
    std::string t;
    if (lexeme.type_lex <= LEX_WRITE)
        t = Scanner::ServiceWords[IndexOf(Scanner::words,lexeme.type_lex) - 1];
    else if(lexeme.type_lex == LEX_ASSIGN)
        t = "Assign";
    else if (lexeme.type_lex >= LEX_FIN && lexeme.type_lex <= LEX_RPAREN)
        t =Scanner::LimSymbols[IndexOf(Scanner::symbols,lexeme.type_lex) - 1];
    else if (lexeme.type_lex == LEX_NUM)
        t = "NUM_" + std::to_string(lexeme.value_lex);
    else if (lexeme.type_lex == LEX_ID)
        t ="ID_"  + std::string(Scanner::Identifiers[lexeme.value_lex].get_name());
    else if (lexeme.type_lex == LEX_STRING_DATA)
        t = "STRING_DATA";
    else if (lexeme.type_lex == LEX_UNARY_MINUS)
        t = "-u";
    else if (lexeme.type_lex == POLIZ_GO)
        t = "!";
    else if (lexeme.type_lex == POLIZ_FGO)
        t = "!F";
    else if (lexeme.type_lex == POLIZ_ADDRESS)
        t = "&_" + std::string(Scanner::Identifiers[lexeme.value_lex].get_name());
    else if (lexeme.type_lex == POLIZ_LABEL)
        t = "mark_" + std::to_string(lexeme.value_lex);
    else
        throw lexeme;
    s << t << " ";
    return s;
}
