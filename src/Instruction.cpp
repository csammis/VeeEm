#include "Instruction.h"

#include <string.h>
#include <stdlib.h>

Instruction::Instruction(Opcode opcode)
    : m_Opcode(opcode)
{
}

Opcode Instruction::Opcode() const
{
    return m_Opcode;
}

bool Instruction::Execute(Context& context)
{
    switch (OpCode())
    {
    case Opcode.LOAD:
        break;
    case Opcode.INCREMENT:
        break;
    case Opcode.DECREMENT:
        break;
    case Opcode.SYSCALL:
        {
            int arg = atoi(m_Parameters.front().c_str());
            if (arg < MIN_SYSCALL || arg > MAX_SYSCALL)
            {
                context.Flags = RegisterFlags::FLAGS_ERR_SYSCALL_OUT_OF_RANGE;
                return false;
            }
            std::unique_ptr<SystemCall> call = SystemCall::Create(static_cast<Syscall>(arg));
            call->Execute(context);
        }
        break;
    }
    context.InstrPtr++;
    return true;
}

