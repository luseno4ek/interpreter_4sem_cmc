#include "MyStack.hpp"

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
