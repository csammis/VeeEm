#ifndef __SYSCALLS_DUMPCONTEXT_H__
#define __SYSCALLS_DUMPCONTEXT_H__

#include "SystemCall.h"
#include "Context.h"

#include <iostream>
#include <iomanip>

class DumpContext : public SystemCall 
{
public:
    DumpContext() { }
    ~DumpContext() { }

    void Execute(Context& context)
    {
        using namespace std;
        cout << setfill('0');

        cout << "REGISTERS" << endl;
        for (int i = 0; i < 32; i++)
        {
            if (i < 10) cout << " ";
            cout << "r" << setbase(10) << i << ": 0x" << setw(8) << setbase(16) << context.Registers[i] << " ";
            if ((i + 1) % 8 == 0)
            {
                cout << endl;
            }
        }
        cout << "FLAGS                          STACK           CALLSTACK" << endl;
        cout << "Err: 0x" << setw(8) << setbase(16) << static_cast<unsigned int>(context.Error);
        cout << " Cmp: 0x" << setw(2) << static_cast<unsigned int>(context.CompareFlags);
        cout << "      Size: 0x" << setw(4) << setbase(16) << static_cast<unsigned int>(context.Stack.size());
        cout << "    Size: 0x" << setw(4) << setbase(16) << static_cast<unsigned int>(context.CallStack.size());
        cout << endl;
    }
};

#endif //__SYSCALLS_DUMPCONTEXT_H__
