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
    void set_name(char* buf);
    TypeOfLex get_type();
    void set_type(TypeOfLex _type);
    int get_value();
    void set_value(int _value);
    bool get_declare();
    void set_declare();
    bool get_assign();
    void set_assign();
};

class TableIdent {
    
};

#endif /* Identificator_hpp */
