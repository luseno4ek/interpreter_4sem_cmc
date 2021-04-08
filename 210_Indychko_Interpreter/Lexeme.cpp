#include "Lexeme.hpp"

Lex::Lex(TypeOfLex _type, int _value)
: type_lex(_type), value_lex(_value) {}

TypeOfLex Lex::get_type() { return type_lex; }

int Lex::get_value() { return value_lex; }

