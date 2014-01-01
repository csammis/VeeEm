#ifndef __SYSCALLS_PRINT_H__
#define __SYSCALLS_PRINT_H__

#include "SystemCall.h"
#include "Context.h"

#include <iostream>

class Print : public SystemCall 
{
public:
    Print() { }
    ~Print() { }

    void Execute(Context& context)
    {
        using namespace std;
        cout << "~~ printing current context ~~" << endl;
        for (int i = 0; i < 32; i++)
        {
            cout << "r" << i << ": " << context.Registers[i] << " ";
            if (i % 3 == 0)
            {
                cout << endl;
            }
        }
    }
};

#endif //__SYSCALLS_PRINT_H__
