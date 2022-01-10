#include <iostream>
#include "Interpreter.hpp"


int main() {
    Interpreter interp_test("/Users/lesenka/Documents/Универ/2курс/Прога/c++/210_Indychko_Interpreter/210_Indychko_Interpreter/test1.txt");
    try {
        interp_test.interpretation();
    }
    catch (const char* err) {
        std::cout << err << std::endl;
        return 0;
    }
    catch (Lex& err){
        std::cout << err << std::endl;
        return 0;
    }
    return 0;
}
