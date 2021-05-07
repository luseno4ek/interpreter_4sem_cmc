#include "Parser.hpp"

void Parser::GetLex() {
    curr_lex = scan.get_lex();
    curr_type = curr_lex.get_type();
    curr_value = curr_lex.get_value();
}
