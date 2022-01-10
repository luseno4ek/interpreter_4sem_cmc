#ifndef Executor_hpp
#define Executor_hpp

#include "Parser.hpp"

class Executor {
private:
    Lex curr_element;
public:
    void execute(Poliz& prog,  const Parser& parser);
};

#endif
