#include "Instruction.h"
#include "SystemCall.h"
#include "Syscall.h"
#include "Context.h"
#include "RegisterFlags.h"

#include <string.h>
#include <stdlib.h>

Instruction::Instruction(enum Opcode opcode)
    : m_Opcode(opcode)
{
}

Instruction::~Instruction()
{
}

Opcode Instruction::Opcode() const
{
    return m_Opcode;
}

void Instruction::AddParameter(const std::string& parameter)
{
    m_Parameters.push_back(parameter);
}

bool Instruction::Execute(Context& context)
{
    switch (Opcode())
    {
    case Opcode::LOAD:
        break;
    case Opcode::INCREMENT:
        break;
    case Opcode::DECREMENT:
        break;
    case Opcode::SYSCALL:
        {
            int arg = atoi(m_Parameters.front().c_str());
            if (arg < MIN_SYSCALL || arg > MAX_SYSCALL)
            {
                context.Flags = static_cast<unsigned int>(RegisterFlags::FLAGS_ERR_SYSCALL_OUT_OF_RANGE);
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

