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

    opcodeParses.insert("load", make_pair(Opcode::LOAD, 2));
    opcodeParses.insert("incr", make_pair(Opcode::INCREMENT, 1));
    opcodeParses.insert("decr", make_pair(Opcode::DECREMENT, 1));
    opcodeParses.insert("call", make_pair(Opcode::SYSCALL, 1));
}

} } }

#endif //__PARSEUTILS_H__
