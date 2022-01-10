#include "Interpreter.hpp"

Interpreter::Interpreter(const char* program)
: parser(program) {};

void Interpreter::interpretation() {
    parser.analysis();
    std::cout << ">>> PROGRAM IS STARTED <<<" << std::endl;
    executor.execute(parser.poliz, parser);
}
