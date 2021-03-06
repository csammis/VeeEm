#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <string>
#include <stack>
#include "RegisterFlags.h"

class Context
{
public:
    Context();
    ~Context();

    unsigned int Registers[32];
    unsigned int InstrPtr;
    ContextError Error;
    unsigned char CompareFlags;

    unsigned int* ResolveLocationReference(const std::string& arg);
    bool ResolveValue(const std::string& arg, unsigned int& value);
    bool ResolveJumpOffset(const std::string& arg, unsigned int& offset);

    std::stack<unsigned int> Stack;
    std::stack<unsigned int> CallStack;

private:
    bool ValidateConstant(const std::string& arg, unsigned int& value);
    bool ValidateRegister(const std::string& arg, int& registerIndex);
};

#endif // __CONTEXT_H__
