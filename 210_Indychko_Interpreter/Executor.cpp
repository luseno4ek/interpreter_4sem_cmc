#include "Executor.hpp"

template<class T, int max_size>
MyStack<T, max_size>::MyStack()
: top(0) {}

template<class T, int max_size>
void MyStack<T, max_size>::reset() { top = 0; }

template<class T, int max_size>
bool MyStack<T, max_size>::isempty() { return top == 0; }

template<class T, int max_size>
bool MyStack<T, max_size>::isfull() { return top == max_size; }

template<class T, int max_size>
void MyStack<T, max_size>::push(T item) {
    if(isfull()) {
        throw "Stack is full!";
    }
    stack[top] = item;
    top++;
}

template<class T, int max_size>
T MyStack<T, max_size>::pop() {
    if(isempty()) {
        throw "Stack is empty!";
    }
    top--;
    return stack[top];
}

/*/////////////////////////////////////////*/

void Executor::execute(Poliz& prog, const Parser& parser) {
    MyStack<Lex, 100> args;
    long long i, j, index = 0, size = prog.get_free();
    Lex stack_lex_1, stack_lex_2;
    while(index < size) {
        curr_element = prog[index];
        switch(curr_element.get_type()) {
        case LEX_STRING_DATA:
        case LEX_TRUE:
        case LEX_FALSE:
        case LEX_NUM:
        case POLIZ_ADDRESS:
        case POLIZ_LABEL:
            args.push(curr_element);
            break;
        case LEX_SEMICOLON:
            if(!args.isempty()) {
                args.pop();
            }
            break;
        case LEX_ID:
            i = curr_element.get_value();
            if(Scanner::Identifiers[i].get_type() == LEX_STRUCT) {
                args.push(Lex(LEX_STRUCT, i));
                break;
            } else if(Scanner::Identifiers[i].get_assign()) {
                if (Scanner::Identifiers[i].get_type() == LEX_STRING) {
                    args.push(Lex(LEX_STRING_DATA,
                        (long long)strdup((char*)Scanner::Identifiers[i].get_value())));
                } else {
                    args.push(Lex(Scanner::Identifiers[i].get_type(), Scanner::Identifiers[i].get_value()));
                }
                break;
            } else {
                throw "POLIZ: indefinite identifier";
            }
        case LEX_NOT:
            args.push(Lex(LEX_BOOL, !args.pop().get_value()));
            break;
        case LEX_UNARY_MINUS:
            args.push(Lex(LEX_INT, -args.pop().get_value()));
            break;
        case LEX_OR:
            i = args.pop().get_value();
            args.push(Lex(LEX_BOOL, args.pop().get_value() || i));
            break;
        case LEX_AND:
            i = args.pop().get_value();
            args.push(Lex(LEX_BOOL, args.pop().get_value() && i));
            break;
        case POLIZ_GO:
            index = args.pop().get_value() - 1;
            break;
        case POLIZ_FGO:
            i = args.pop().get_value();
            if(!args.pop().get_value())
                index = i - 1;
            break;
        case LEX_WRITE:
            stack_lex_1 = args.pop();
            if (stack_lex_1.get_type() == LEX_STRING_DATA) {
                std::cout << (char*)stack_lex_1.get_value() << std::endl;
            } else {
                std::cout << stack_lex_1.get_value() << std::endl;
            }
            break;
        case LEX_READ:
            long long k;
            i = args.pop().get_value();
            if(Scanner::Identifiers[i].get_type() == LEX_INT) {
                std::cout << "Input int value for ";
                std::cout << Scanner::Identifiers[i].get_name() << " :" << std::endl;
                std::cin >> k;
            } else if(Scanner::Identifiers[i].get_type() == LEX_BOOL) {
                char buff[20];
            L:
                std::cout << "Input boolean value ";
                std::cout << "(true or false) for ";
                std::cout << Scanner::Identifiers[i].get_name() << " :" << std::endl;
                std::cin >> buff;
                if(!strcmp(buff, "true")) {
                    k = 1;
                } else if(!strcmp(buff, "false")) {
                    k = 0;
                } else {
                    std::cout << "Error in input: true/false!" << std::endl;
                    std::cout << std::endl;
                    goto L;
                }
            } else {
                char buff[80];
                std::cout << "Input string for ";
                std::cout << Scanner::Identifiers[i].get_name() << " :" << std::endl;
                std::cin >> buff;
                k = (long long)strdup(buff);
            }
            Scanner::Identifiers[i].set_value(k);
            Scanner::Identifiers[i].set_assign();
            break;
        case LEX_PLUS:
            stack_lex_1 = args.pop();
            if (stack_lex_1.get_type() == LEX_STRING_DATA) {
                char* curr = strdup((char*)args.pop().get_value());
                args.push(Lex(LEX_STRING_DATA,
                    (long long)strcat(curr, (char*)stack_lex_1.get_value())));
            }
            else {
                args.push(Lex(stack_lex_1.get_type(),
                    args.pop().get_value() + stack_lex_1.get_value()));
            }
            break;
            case LEX_MULTIPLY:
            args.push(Lex(LEX_INT, args.pop().get_value() * args.pop().get_value()));
            break;
        case LEX_MINUS:
            i = args.pop().get_value();
            args.push(Lex(LEX_INT, args.pop().get_value() - i));
            break;
        case LEX_DIVISION:
            i = args.pop().get_value();
            if (i) {
                args.push(Lex(LEX_INT, args.pop().get_value() / i));
                break;
            }
            else throw "POLIZ:divide by zero";
        case LEX_EQ:
            stack_lex_1 = args.pop();
            if (stack_lex_1.get_type() == LEX_STRING_DATA) {
                args.push(Lex(LEX_BOOL,
                    strcmp((char*)args.pop().get_value(), (char*)stack_lex_1.get_value()) == 0));
            } else {
                args.push(Lex(stack_lex_1.get_type(),
                    args.pop().get_value() == stack_lex_1.get_value()));
            }
            break;
            case LEX_LESS:
            stack_lex_1 = args.pop();
            if (stack_lex_1.get_type() == LEX_STRING_DATA) {
                args.push(Lex(LEX_BOOL,
                    strcmp((char*)args.pop().get_value(), (char*)stack_lex_1.get_value()) < 0));
            } else {
                args.push(Lex(stack_lex_1.get_type(),
                    args.pop().get_value() < stack_lex_1.get_value()));
            }
            break;
            case LEX_GR:
            stack_lex_1 = args.pop();
            if (stack_lex_1.get_type() == LEX_STRING_DATA) {
                args.push(Lex(LEX_BOOL,
                    strcmp((char*)args.pop().get_value(), (char*)stack_lex_1.get_value()) > 0));
            }
            else {
                args.push(Lex(stack_lex_1.get_type(),
                    args.pop().get_value() > stack_lex_1.get_value()));
            }
            break;
        case LEX_LEQ:
            i = args.pop().get_value();
            args.push(Lex(LEX_BOOL, args.pop().get_value() <= i));
            break;
        case LEX_GEQ:
            i = args.pop().get_value();
            args.push(Lex(LEX_BOOL, args.pop().get_value() >= i));
            break;
        case LEX_NEQ:
            stack_lex_1 = args.pop();
            if (stack_lex_1.get_type() == LEX_STRING_DATA) {
                args.push(Lex(LEX_BOOL,
                    strcmp((char*)args.pop().get_value(), (char*)stack_lex_1.get_value()) != 0));
            } else {
                args.push(Lex(stack_lex_1.get_type(),
                    args.pop().get_value() != stack_lex_1.get_value()));
            }
            break;
        case LEX_ASSIGN:
            stack_lex_2 = args.pop();
            j = args.pop().get_value();
            if(stack_lex_2.get_type() == LEX_STRUCT) {
                int len = parser.length_of_struct(Scanner::Identifiers[j].get_value());
                for(int y = 0; y < len; y++) {
                    Ident& r_val = Scanner::Identifiers[stack_lex_2.get_value() + 1 + y];
                    Ident& l_val = Scanner::Identifiers[j + 1 + y];
                    if (r_val.get_type() == LEX_STRING) {
                        l_val.set_value((long long)strdup((char*)r_val.get_value()));
                    } else {
                        l_val.set_value(r_val.get_value());
                    }
                    if (r_val.get_assign()) {l_val.set_assign();}
                }
            } else if (stack_lex_2.get_type() == LEX_STRING_DATA) {
                long long str_curr = (long long)strdup((char*)stack_lex_2.get_value());
                Scanner::Identifiers[j].set_value(str_curr);
            } else {
                Scanner::Identifiers[j].set_value(stack_lex_2.get_value());
            }
            args.push(stack_lex_2);
            Scanner::Identifiers[j].set_assign();
            break;
        default:
            throw "POLIZ: unexpected elem";
        }
        index++;
    };
    std::cout << ">>>PROGRAM WAS FINISHED CORRECTLY!<<<" << std::endl;
}
