#ifndef __PARSEUTILS_H__
#define __PARSEUTILS_H__

#include <string>
#include <map>

#include "Opcode.h"

namespace VeeEm { namespace Core { namespace Utils {

typedef std::map<std::string, std::pair<Opcode, int> > OpcodeParseMap;

static OpcodeParseMap opcodeParses;

void InitOpcodeParseMap()
{
    if (opcodeParses.size() > 0)
    {
        return;
    }

    opcodeParses["load"] = std::make_pair(Opcode::LOAD, 2);
    opcodeParses["incr"] = std::make_pair(Opcode::INCREMENT, 1);
    opcodeParses["decr"] = std::make_pair(Opcode::DECREMENT, 1);
    opcodeParses["add"] = std::make_pair(Opcode::ADD, 3);
    opcodeParses["sub"] = std::make_pair(Opcode::SUBTRACT, 3);
    opcodeParses["comp"] = std::make_pair(Opcode::COMPARE, 2);
    opcodeParses["je"] = std::make_pair(Opcode::JUMP_EQUAL, 1);
    opcodeParses["jne"] = std::make_pair(Opcode::JUMP_NOT_EQUAL, 1);
    opcodeParses["jlt"] = std::make_pair(Opcode::JUMP_LESS_THAN, 1);
    opcodeParses["jlte"] = std::make_pair(Opcode::JUMP_LESS_THAN_EQUAL_TO, 1);
    opcodeParses["jgt"] = std::make_pair(Opcode::JUMP_GREATER_THAN, 1);
    opcodeParses["jgte"] = std::make_pair(Opcode::JUMP_GREATER_THAN_EQUAL_TO, 1);
    opcodeParses["syscall"] = std::make_pair(Opcode::SYSCALL, 1);
}

std::string Trim(const std::string& input)
{
    std::string retval(input);
    if (!retval.empty())
    {
        int front = 0;
        int back = retval.size() - 1;
        while (retval[front] == ' ')
        {
            front++;
        }
        while (retval[back] == ' ')
        {
            back--;
        }
        retval = retval.substr(front, back - front + 1);
    }
    return retval;
}

} } }

#endif //__PARSEUTILS_H__
