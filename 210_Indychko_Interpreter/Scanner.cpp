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

int Scanner::look(const char *_buf, const char **list) {
    int i = 0;
    while(list[i] != NULL) {
        if(strcmp(buf, list[i]) == 0) { return i; }
        i++;
    }
    return 0;
}

void Scanner::gc() { c = fgetc(fp); }

/*/////////////////////////////////////////*/

bool Scanner::isalpha(char c) { return (c >= 'a') && (c <= 'z'); }

bool Scanner::isdigit(char c) { return (c >= '0') && (c <= '9'); }

/*/////////////////////////////////////////*/


Lex Scanner::get_lex() {
    int d = 0, j;
    curr_state = INIT;
    do {
        switch (curr_state){
            case INIT:
                clear();
                if(isalpha(c)) {
                    add();
                    curr_state = ID;
                } else if(isdigit(c)) {
                    d = c - '0';
                    curr_state = NUMBER;
                } else if((c == '=') || (c == '<') || (c == '>') || (c == '!')) {
                    add();
                    curr_state = DOUBLESYMBOL;
                } else if(c == '@') {
                    return Lex(LEX_FIN);
                } else if(c == '"') {
                    curr_state = STRING;
                } else {
                    curr_state = SYMBOL;
                    break;
                }
                gc();
                break;
            case ID:
                if(isalpha(c) || isdigit(c)) {
                    add();
                    gc();
                } else {
                    if((j = look(buf, ServiceWords)) != 0) {
                        return Lex(words[j], j);
                    } else {
                        j = Identifiers.add(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMBER:
                if(isdigit(c)) {
                    d = d * 10 + (c - '0');
                    gc();
                } else {
                    return Lex(LEX_NUM, d);
                }
                break;
            case DOUBLESYMBOL:
                if(c == '=') {
                    add();
                    gc();
                } else if(buf[buf_top - 2] == '!') {
                        throw '!';
                }
                j = look(buf, LimSymbols);
                return Lex(symbols[j], j);
                break;
            case SYMBOL:
                add();
                if((j = look(buf, LimSymbols)) != 0) {
                    gc();
                    return Lex(symbols[j], j);
                } else {
                    throw c;
                }
                break;
            case STRING:
                while(c != '"') {
                    add();
                    gc();
                }
                j = StringsData.add(buf);
                return Lex(LEX_STRING_DATA, j);
                break;
        }
    } while(true);
}
