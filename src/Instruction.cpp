#include "Instruction.h"
#include "SystemCall.h"
#include "Syscall.h"
#include "Context.h"
#include "RegisterFlags.h"

#include <stdlib.h>

Instruction::Instruction(enum Opcode opcode, int parameterCount)
    : m_Opcode(opcode), m_Parameters(parameterCount)
{
}

Instruction::~Instruction()
{
}

Opcode Instruction::Opcode() const
{
    return m_Opcode;
}

void Instruction::SetParameter(int index, const std::string& parameter)
{
    m_Parameters[index] = parameter;
}

bool Instruction::Execute(Context& context)
{
    using namespace std;

    switch (Opcode())
    {
    case Opcode::LOAD:
        {
            unsigned int* pReg = context.ResolveLocationReference(m_Parameters[0]);
            unsigned int value = 0;
            if (!context.ResolveValue(m_Parameters[1], value))
            {
                return false;
            }
            *pReg = value;
        }
        break;
    case Opcode::INCREMENT:
        {
            unsigned int* pLoc = context.ResolveLocationReference(m_Parameters[0]);
            *pLoc = (*pLoc) + 1;
        }
        break;
    case Opcode::DECREMENT:
        {
            unsigned int* pLoc = context.ResolveLocationReference(m_Parameters[0]);
            *pLoc = (*pLoc) - 1;
        }
        break;
    case Opcode::ADD:
        {
            unsigned int* pReg = context.ResolveLocationReference(m_Parameters[0]);
            unsigned int a = 0, b = 0;
            if (!context.ResolveValue(m_Parameters[1], a) || !context.ResolveValue(m_Parameters[2], b))
            {
                return false;
            }
            *pReg = a + b;
        }
        break;
    case Opcode::SYSCALL:
        {
            int arg = strtol(m_Parameters[0].c_str(), nullptr, 0);
            if (arg < MIN_SYSCALL || arg > MAX_SYSCALL)
            {
                context.Error = ContextError::SYSCALL_OUT_OF_RANGE;
                return false;
            }
            unique_ptr<SystemCall> call = SystemCall::Create(static_cast<Syscall>(arg));
            call->Execute(context);
        }
        break;
    }
    context.InstrPtr++;
    return true;
}

