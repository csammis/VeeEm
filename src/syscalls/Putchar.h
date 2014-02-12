#ifndef __SYSCALLS_PUTCHAR_H__
#define __SYSCALLS_PUTCHAR_H__

#include "SystemCall.h"
#include "Context.h"
#include <iostream>

class Putchar : public SystemCall
{
public:
    Putchar() { }
    ~Putchar() { }

    void Execute(Context& context)
    {
        unsigned int value = context.Stack.top();
        context.Stack.pop();
        
        char* pChar = reinterpret_cast<char*>(&value);
        std::cout << pChar;
    }
};

#endif // __SYSCALLS_PUTCHAR_H__
