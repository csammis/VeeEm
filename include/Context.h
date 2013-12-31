#ifndef __CONTEXT_H__
#define __CONTEXT_H__

class Context
{
public:
    Context();
    ~Context();

    unsigned int Registers[32];
    unsigned int InstrPtr;
    unsigned int Flags;
};

#endif // __CONTEXT_H__
