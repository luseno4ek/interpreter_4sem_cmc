#include <iostream>
#include "Scanner.hpp"
#include "Parser.hpp"


int main() {
    Parser test_parser("/Users/lesenka/Documents/Универ/2курс/Прога/c++/210_Indychko_Interpreter/210_Indychko_Interpreter/test1.txt");
    test_parser.analysis();
    test_parser.poliz.print();
    return 0;
}
