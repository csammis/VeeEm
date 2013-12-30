#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "Opcode.h"

class Instruction
{
public:
    Instruction(Opcode opcode);
    ~Instruction();

    Opcode Opcode() const;

    unsigned int Parameters[3];

private:
    Opcode m_Opcode;
};

#endif // __INSTRUCTION_H__
