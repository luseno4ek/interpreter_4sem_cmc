#ifndef Interpreter_hpp
#define Interpreter_hpp

#include "Executor.hpp"

class Interpreter {
private:
    Parser parser;
    Executor executor;
public:
    explicit Interpreter(const char* program);
    void interpretation();
};

#endif
