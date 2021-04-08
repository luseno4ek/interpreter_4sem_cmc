#ifndef Identificator_hpp
#define Identificator_hpp

#include "Lexeme.hpp"

class Ident {
private:
    char* name;
    bool declare;
    bool assign;
    TypeOfLex type;
    int value;
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
    ///function returns a position of identificator with name buf from the table
    int add(const char* buf);
};

#endif /* Identificator_hpp */
