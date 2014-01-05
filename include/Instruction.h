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

    friend class VeeEmProgram;

private:
    std::vector<std::string> m_Parameters;
    enum Opcode m_Opcode;
    bool m_Rebased;

    bool DoJump(Context& context);
};

#endif // __INSTRUCTION_H__
