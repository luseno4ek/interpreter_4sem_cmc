#include "Identificator.hpp"
#include <string>

Ident::Ident()
: declare(false), assign(false) {}

Ident::~Ident() { delete[] name; }

char* Ident::get_name() { return name; }

void Ident::set_name(char *buf) {
    name = new char[strlen(buf) + 1];
    strcpy(name, buf);
}

TypeOfLex Ident::get_type() { return type; }

