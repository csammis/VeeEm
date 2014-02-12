#include "Instruction.h"
#include "SystemCall.h"
#include "Syscall.h"
#include "Context.h"
#include "RegisterFlags.h"

#include <stdlib.h>

Instruction::Instruction(enum Opcode opcode, int parameterCount)
    : m_Parameters(parameterCount), m_Opcode(opcode), m_Rebased(false)
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

            context.CompareFlags = 0x01;
            if (a == b) context.CompareFlags |= 0x02;
            if (a < b) context.CompareFlags |= 0x04;
            if (a > b) context.CompareFlags |= 0x08;
        }
        break;
    case Opcode::JUMP_EQUAL:
    case Opcode::JUMP_NOT_EQUAL:
    case Opcode::JUMP_LESS_THAN:
    case Opcode::JUMP_GREATER_THAN:
    case Opcode::JUMP_LESS_THAN_EQUAL_TO:
    case Opcode::JUMP_GREATER_THAN_EQUAL_TO:
        if (!DoJump(context))
        {
            return false;
        }
        break;
    case Opcode::PUSH:
        {
            unsigned int* pReg = context.ResolveLocationReference(m_Parameters[0]);
            if (pReg == nullptr)
            {
                return false;
            }
            context.Stack.push({ Context::StackSource::PUSH, *pReg });
        }
        break;
    case Opcode::POP:
        {
            unsigned int* pReg = context.ResolveLocationReference(m_Parameters[0]);
            if (pReg == nullptr)
            {
                return false;
            }

            if (context.Stack.empty())
            {
                context.Error = ContextError::POP_UNBALANCED_WITH_PUSH;
                return false;
            }

            Context::StackContext entry = context.Stack.top();
            if (entry.source != Context::StackSource::PUSH)
            {
                context.Error = ContextError::POP_UNBALANCED_WITH_PUSH;
                return false;
            }
            *pReg = entry.value;
            context.Stack.pop();
        }
        break;
    case Opcode::CALL:
        {
            unsigned int offset = 0;
            if (!context.ResolveJumpOffset(m_Parameters[0], offset))
            {
                return false;
            }
            context.Stack.push( {Context::StackSource::CALL, context.InstrPtr} );
            context.InstrPtr += offset - 1;
        }
        break;
    case Opcode::RETURN:
        {
            if (context.Stack.empty())
            {
                context.Error = ContextError::RETURN_UNBALANCED_WITH_CALL;
                return false;
            }

            Context::StackContext entry = context.Stack.top();
            if (entry.source != Context::StackSource::CALL)
            {
                context.Error = ContextError::RETURN_UNBALANCED_WITH_CALL;
                return false;
            }
            context.InstrPtr = entry.value;
            context.Stack.pop();
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
            std::unique_ptr<SystemCall> call = SystemCall::Create(static_cast<Syscall>(arg));
            call->Execute(context);
        }
        break;
    }
    context.InstrPtr++;
    return true;
}

bool Instruction::DoJump(Context& context)
{
    if (!(context.CompareFlags & 0x01))
    {
        context.Error = ContextError::CONDITIONAL_JUMP_WITHOUT_COMPARE;
        return false;
    }

    if ((m_Opcode == Opcode::JUMP_NOT_EQUAL && !(context.CompareFlags & 0x02)) ||
        (m_Opcode == Opcode::JUMP_EQUAL && (context.CompareFlags & 0x02)) ||
        (m_Opcode == Opcode::JUMP_LESS_THAN && (context.CompareFlags & 0x04)) ||
        (m_Opcode == Opcode::JUMP_GREATER_THAN && (context.CompareFlags & 0x08)) ||
        (m_Opcode == Opcode::JUMP_LESS_THAN_EQUAL_TO && (context.CompareFlags & 0x06)) ||
        (m_Opcode == Opcode::JUMP_GREATER_THAN_EQUAL_TO && (context.CompareFlags & 0x0A))
       )
    {
        unsigned int offset = 0;
        if (!context.ResolveJumpOffset(m_Parameters[0], offset))
        {
            return false;
        }
        context.InstrPtr += offset - 1; // Because the Execute is going to increment IP anyway
    }

    context.CompareFlags = 0x00;
    return true;
}


