#ifndef Identifier_hpp
#define Identifier_hpp

#include "Lexeme.hpp"

class Ident {
private:
    char* name;
    bool declare;
    bool assign;
    TypeOfLex type;
    int value;
    int goto_label;
    bool is_goto_label;
public:
    Ident();
    ~Ident();
    char* get_name();
    void set_name(const char* buf);
    TypeOfLex get_type();
    void set_type(TypeOfLex _type);
    int get_value();
    void set_value(int _value);
    bool get_declare();
    void set_declare();
    bool get_assign();
    void set_assign();
    void set_goto_label(int _label);
    int get_goto_label();
    void set_is_goto_label();
    bool get_is_goto_label();
};

/*/////////////////////////////////////////*/

class TableIdent {
private:
    Ident* table;
    int size;
    int last_pos;
public:
    TableIdent(int max_size);
    ~TableIdent();
    Ident& operator[] (int k);
    int add(const char* buf);
};

/*/////////////////////////////////////////*/

class TableString {
private:
    char** table;
    int size;
public:
    TableString();
    ~TableString();
    int add(const char* buf);
};

#endif /* Identificator_hpp */
