#include "Instruction.h"

#include <string.h>

Instruction::Instruction(Opcode opcode)
    : m_Opcode(opcode)
{
    memset(&Parameters, 0, 3 * sizeof(unsigned int));
}

Opcode Instruction::Opcode() const
{
    return m_Opcode;
}
