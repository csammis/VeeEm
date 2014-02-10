#include "VeeEmProgram.h"

#include <sstream>

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

void VeeEmProgram::SetSections(const std::map<std::string, int>& sections)
{
    m_Sections = sections;
}

void VeeEmProgram::SetInstructions(const std::vector<Instruction>& instructions)
{
    m_Instructions = instructions;
}

void VeeEmProgram::Execute() const
{
    using namespace VeeEm::Core::Logger;

    Context context;

    // Set the programmer-defined entry point if there is one
    auto entry = m_Sections.find("entry");
    if (entry != m_Sections.end())
    {
        context.InstrPtr = entry->second;
    }

    while (context.InstrPtr < m_Instructions.size())
    {
        Instruction current = m_Instructions[context.InstrPtr];
        
        if (!(Rebase(current, context) && current.Execute(context)))
        {
            Log::Instance(LogLevel::ERROR) << "Caught runtime error! Dumping current context:" << End();
            Instruction whoops(Opcode::SYSCALL, 1);
            whoops.SetParameter(0, "0");
            whoops.Execute(context);
            break;
        }
    }
}

bool VeeEmProgram::Rebase(Instruction& instruction, Context& context) const
{
    using namespace std;
    using namespace VeeEm::Core::Logger;

    if (instruction.Opcode() >= OPCODE_REBASE_MIN && instruction.Opcode() <= OPCODE_REBASE_MAX &&
            !instruction.m_Rebased)
    {
        string parameter = instruction.m_Parameters[0];
        if (parameter[0] == ':')
        {
            auto labelLocation = m_Labels.find(parameter.substr(1));
            if (labelLocation == m_Labels.end())
            {
                Log::Instance(LogLevel::ERROR) << "Couldn't find label " << parameter << End();
                context.Error = ContextError::LABEL_NOT_FOUND;
                return false;
            }
            stringstream stream;
            stream << "$" << (labelLocation->second - context.InstrPtr);
            instruction.m_Parameters[0] = stream.str();
        }
        instruction.m_Rebased = true;
    }
    return true;
}

