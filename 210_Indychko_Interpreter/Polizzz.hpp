#ifndef Polizzz_hpp
#define Polizzz_hpp

#include <stdio.h>
#include "iostream"
#include "Lexeme.hpp"

class Poliz {
private:
    Lex *p;
    int size;
    int free;
 public:
    Poliz (int max_size);
    ~Poliz();
    void put_lex(Lex l);
    void update_lex(Lex l, int place);
    Lex& operator[](long long index);
    void blank();
    int get_free();
    void print();
};
#endif /* Polizzz_hpp */
