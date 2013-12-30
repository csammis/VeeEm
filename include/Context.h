#ifndef __CONTEXT_H__
#define __CONTEXT_H__

class Instruction;

class Context
{
public:
    Context();
    ~Context();

    unsigned int Registers[32];
    Instruction* InstrPtr;
};

#endif // __CONTEXT_H__
