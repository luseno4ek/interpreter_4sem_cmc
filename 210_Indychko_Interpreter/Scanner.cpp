#include "Scanner.hpp"
#include <cstring>

const char* Scanner::ServiceWords[] = {
    ""
    "program",
    "string",
    "int",
    "boolean",
    "struct",
    "if",
    "then",
    "else",
    "for",
    "while",
    "break",
    "goto",
    "false",
    "true",
    "not",
    "or",
    "and",
    "read",
    "write",
    NULL
};

/*/////////////////////////////////////////*/

const char* Scanner::LimSymbols[] = {
    ""
    "@",
    "{",
    "}",
    "*",
    "/",
    "+",
    "-",
    "<",
    ">",
    "<=",
    ">=",
    "==",
    "!=",
    "=",
    ";",
    ",",
    ".",
    ":",
    "(",
    ")",
    NULL
};

/*/////////////////////////////////////////*/

TableIdent Scanner::Identifiers(100);
TableIdent Scanner::StringData(100);

/*/////////////////////////////////////////*/

TypeOfLex Scanner::words[] = {
    LEX_NULL,
    LEX_PROGRAM,
    LEX_STRING, LEX_INT, LEX_BOOL, LEX_STRUCT,
    LEX_IF, LEX_THEN, LEX_ELSE,
    LEX_FOR, LEX_WHILE,
    LEX_BREAK, LEX_GOTO,
    LEX_FALSE, LEX_TRUE,
    LEX_NOT, LEX_OR, LEX_AND,
    LEX_READ, LEX_WRITE,
    LEX_NULL
};

/*/////////////////////////////////////////*/

TypeOfLex Scanner::symbols[] = {
    LEX_NULL,
    LEX_FIN, LEX_BEGIN, LEX_END,
    LEX_MULTIPLY, LEX_DIVISION, LEX_PLUS,
    LEX_MINUS, LEX_LESS, LEX_GR, LEX_LEQ,
    LEX_GEQ, LEX_EQ, LEX_NEQ, LEX_ASSIGN,
    LEX_SEMICOLON, LEX_COMMA, LEX_DOT, LEX_COLON,
    LEX_LPAREN, LEX_RPAREN,
    LEX_NULL
};

/*/////////////////////////////////////////*/

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
        if(strcmp(buf, list[i]) == 0) { return ++i; }
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
                } else if(c == '/') {
                    gc();
                    if(c == '*') {
                        curr_state = COMMENTARY;
                    } else {
                        throw "Incorrect commentary use!";
                    }
                } else if(c == ' ' || c == '\n') {
                    gc();
                    break;
                } else {
                    curr_state = SYMBOL;
                    break;
                }
                gc();
                break;
            case ID:
                if(isalpha(c) || isdigit(c) || c == '.') {
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
                } else if(c == '\n' || c == ' ') {
                    gc();
                    clear();
                    curr_state = INIT;
                }
                break;
            case STRING:
                while(c != '"') {
                    add();
                    gc();
                }
                gc();
                return Lex(LEX_STRING_DATA, (long long)strdup(buf));
                break;
            case COMMENTARY:
                while(c != '*') {
                    gc();
                }
                gc();
                if(c != '/') {
                    curr_state = COMMENTARY;
                    break;
                }
                curr_state = INIT;
                break;
        }
    } while(true);
}




void Scanner::Print_Result() {
    Lex curr_lex;
    curr_lex = get_lex();
    while(curr_lex.get_type() != LEX_FIN) {
        std::cout << curr_lex;
        if(curr_lex.get_type() == LEX_SEMICOLON || curr_lex.get_type() == LEX_BEGIN) {
            std::cout << std::endl;
        }
        curr_lex = get_lex();
    }
}


