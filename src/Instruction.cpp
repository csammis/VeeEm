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
    case Opcode::DECREMENT:
        m_Parameters.resize(3);
        m_Parameters[1] = m_Parameters[0];
        m_Parameters[2] = (Opcode() == Opcode::INCREMENT) ? "$1" : "$-1";
    case Opcode::ADD:
    case Opcode::SUBTRACT:
        {
            unsigned int* pReg = context.ResolveLocationReference(m_Parameters[0]);
            unsigned int a = 0, b = 0;
            if (!context.ResolveValue(m_Parameters[1], a) || !context.ResolveValue(m_Parameters[2], b))
            {
                return false;
            }

            if (Opcode() == Opcode::SUBTRACT)
            {
                *pReg = a - b;
            }
            else
            {
                *pReg = a + b;
            }
        }
        break;
    case Opcode::COMPARE:
        {
            unsigned int a = 0, b = 0;
            if (!context.ResolveValue(m_Parameters[0], a) || !context.ResolveValue(m_Parameters[1], b))
            {
                return false;
            }

            context.CompareFlags = 0;
            if (a == b) context.CompareFlags |= 0x01;
            if (a < b) context.CompareFlags |= 0x02;
            if (a > b) context.CompareFlags |= 0x04;
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

