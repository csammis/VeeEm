#include "VeeEmProgram.h"

#include "Instruction.h"
#include "Context.h"
#include "CoreLogger.h"

VeeEmProgram::VeeEmProgram()
{
}

VeeEmProgram::~VeeEmProgram()
{
}

void VeeEmProgram::SetLabels(const std::map<std::string, int>& labels)
{
    m_Labels = labels;
}

void VeeEmProgram::SetInstructions(const std::vector<Instruction>& instructions)
{
    m_Instructions = instructions;
}

void VeeEmProgram::Execute() const
{
    using namespace VeeEm::Core::Logger;

    Context context;
    while (context.InstrPtr < m_Instructions.size())
    {
        Instruction current = m_Instructions[context.InstrPtr];
        if (!current.Execute(context))
        {
            Log::Instance(LogLevel::ERROR) << "Caught runtime error! Dumping current context:" << End();
            Instruction whoops(Opcode::SYSCALL, 1);
            whoops.SetParameter(0, "0");
            whoops.Execute(context);
            break;
        }
    }
}

