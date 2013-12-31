#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "Opcode.h"

class Context;

class Instruction
{
public:
    Instruction(Opcode opcode);
    ~Instruction();

    Opcode Opcode() const;

    unsigned int Parameters[3];

    bool Execute(Context& context);

private:
    Opcode m_Opcode;
};

#endif // __INSTRUCTION_H__
