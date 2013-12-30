#ifndef __SYSCALLS_PRINT_H__
#define __SYSCALLS_PRINT_H__

#include "SystemCall.h"

#include <iostream>

class Print : public SystemCall 
{
public:
    Print();
    ~Print();

    void Execute(Context& context)
    {
        using namespace std;
        cout << "In Print syscall" << endl;
    }
};

#endif //__SYSCALLS_PRINT_H__
