#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <vector>
#include <string>

#include "Opcode.h"

class Context;

class Instruction
{
public:
    Instruction(enum Opcode opcode, int parameterCount);
    ~Instruction();

    Opcode Opcode() const;

    void SetParameter(int index, const std::string& parameter);

    bool Execute(Context& context);

private:
    std::vector<std::string> m_Parameters;
    enum Opcode m_Opcode;

    bool DoJump(Context& context);
};

#endif // __INSTRUCTION_H__
