#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <string>
#include "RegisterFlags.h"

class Context
{
public:
    Context();
    ~Context();

    unsigned int Registers[32];
    unsigned int InstrPtr;
    RegisterFlags Flags;

    unsigned int* ResolveLocationReference(const std::string& arg);
};

#endif // __CONTEXT_H__
