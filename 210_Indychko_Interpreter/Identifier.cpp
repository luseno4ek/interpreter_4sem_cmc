#include "Identifier.hpp"
#include <cstring>
#include <cstdlib>

Ident::Ident()
: declare(false), assign(false) {}

Ident::~Ident() { }

char* Ident::get_name() { return name; }

void Ident::set_name(const char *buf) {
    name = new char[strlen(buf) + 1];
    strcpy(name, buf);
}

TypeOfLex Ident::get_type() { return type; }

void Ident::set_type(TypeOfLex _type) { type = _type; }

long long Ident::get_value() { return value; }

void Ident::set_value(long long _value) { value = _value; }

bool Ident::get_declare() { return declare; }

void Ident::set_declare() { declare = true; }

bool Ident::get_assign() { return assign; }

void Ident::set_assign() { assign = true; }

void Ident::set_goto_label(int _label) { goto_label = _label; }

int Ident::get_goto_label() { return goto_label; }

void Ident::set_is_goto_label() { is_goto_label = true; }

bool Ident::get_is_goto_label() { return is_goto_label; }

/*/////////////////////////////////////////*/

TableIdent::TableIdent(int max_size)
: last_pos(1), size(max_size)
{
    table = new Ident[size];
}

TableIdent::~TableIdent() { delete[] table; }

Ident& TableIdent::operator[](long long k) { return table[k]; }

int TableIdent::add(const char *buf) {
    for(int j = 1; j < last_pos; j++) {
        if(strcmp(table[j].get_name(), buf) == 0) {
            return j;               // we have found an identificator with the same name
        }                           // => return it's position
    }
    table[last_pos].set_name(buf);  // we haven't found identificator with matching name
    last_pos++;                     // => we create a new one
    return last_pos - 1;
}

void TableIdent::pop() { size--; }

int TableIdent::get_size() { return size; }
