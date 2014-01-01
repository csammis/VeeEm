#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <list>
#include <string>

#include "Opcode.h"

class Context;

class Instruction
{
public:
    Instruction(enum Opcode opcode);
    ~Instruction();

    Opcode Opcode() const;

    void AddParameter(const std::string& parameter);

    bool Execute(Context& context);

private:
    std::list<std::string> m_Parameters;
    enum Opcode m_Opcode;
};

#endif // __INSTRUCTION_H__
