#include "Scanner.hpp"
#include <cstring>

Scanner::Scanner(const char* program) {
    fp = fopen(program, "r"); // file "program" is opened for reading
    curr_state = INIT;
    clear();
    gc();
}

void Scanner::clear() {
    buf_top = 0;
    for(int i = 0; i < 80; i++) { buf[i] = '\0'; }
}

void Scanner::add() { buf[buf_top++] = c; }

int Scanner::look(const char *_buf, char **list) {
    int i = 0;
    while(list[i] != NULL) {
        if(strcmp(buf, list[i]) == 0) { return i; }
        i++;
    }
    return 0;
}

void Scanner::gc() { c = fgetc(fp); }


