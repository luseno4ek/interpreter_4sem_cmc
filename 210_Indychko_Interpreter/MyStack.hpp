#ifndef MyStack_hpp
#define MyStack_hpp

#include <stdio.h>

template<class T, int max_size>
class MyStack {
private:
    T stack[max_size];
    int top;
public:
    MyStack();
    void reset();
    void push(T item);
    T pop();
    bool isempty();
    bool isfull();
};

#endif /* MyStack_hpp */
