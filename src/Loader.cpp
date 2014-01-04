#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>

#include "Instruction.h"
#include "Context.h"
#include "ParseUtils.h"
#include "CoreLogger.h"

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions);
int RunProgram(std::ifstream& infile);

int main(int argc, char** argv)
{
    using namespace std;

    if (argc != 2)
    {
        cout << "Usage: VeeEm <filename>" << endl;
        return 1;
    }

    string filename(argv[1]);
    ifstream infile;
    infile.open(filename.c_str());
    if (!infile.good())
    {
        cout << "Unable to load " << filename << endl;
        return 1;
    }

    return RunProgram(infile);
}

int RunProgram(std::ifstream& infile)
{
    using namespace VeeEm::Core;
    using namespace VeeEm::Core::Logger;

    Log::Initialize(LogLevel::DEBUG);

    std::vector<Instruction> instructions;
    if (!LoadInstructions(infile, instructions))
    {
        return 1;
    }

    Log::Instance(LogLevel::DEBUG) << "Loaded " << instructions.size() << " instructions" << End();
    
    Context context;
    while (context.InstrPtr < instructions.size())
    {
        Instruction current = instructions[context.InstrPtr];
        current.Execute(context);
    }
    
    Log::Teardown();
    return 0;
}

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions)
{
    using namespace std;
    using namespace VeeEm::Core;
    using namespace VeeEm::Core::Logger;
    using namespace VeeEm::Core::Utils;

    InitOpcodeParseMap();

    bool successfulParse = true; // Optimism!
    for (int linenumber = 1; infile.good(); linenumber++)
    {
        string raw_instruction;
        getline(infile, raw_instruction);

        string::size_type nameEnd = raw_instruction.find_first_of(' ');
        if (nameEnd == string::npos)
        {
            nameEnd = raw_instruction.size();
        }
        string name = raw_instruction.substr(0, nameEnd);

        if (raw_instruction.empty() || name.empty())
        {
            continue;
        }

        auto lookup = opcodeParses.find(name);
        if (lookup == opcodeParses.end())
        {
            Log::Instance(LogLevel::ERROR) << "Unknown instuction found during parse: " << name << " (line " << linenumber << ")" << End();
            successfulParse = false;
            continue;
        }

        list<string> parameterList;
        if (name.size() + 1 < raw_instruction.size())
        {
            string parameters = raw_instruction.substr(name.size() + 1);
            while (!parameters.empty())
            {
                string parameter = "";
                string::size_type splitAt = parameters.find_first_of(',');
                if (splitAt == string::npos)
                {
                    splitAt = parameters.size();
                    parameter = Trim(parameters.substr(0, parameters.size()));
                    parameters.clear();
                }
                else
                {
                    parameter = Trim(parameters.substr(0, splitAt));
                    parameters = parameters.substr(splitAt + 1);
                }

                Log::Instance(LogLevel::DEBUG) << "Got parameter [" << parameter << "] from front of list [" << parameters << "]" << End();
                parameterList.push_back(parameter);
            }
        }

        if (parameterList.size() != lookup->second.second)
        {
            Log::Instance(LogLevel::ERROR) << "Unexpected number of parameters at line " << linenumber << " for instruction '" << name
                << "' (should be " << lookup->second.second << " but read " << parameterList.size() << ")" << End();
            successfulParse = false;
            continue;
        }

        Instruction inst(lookup->second.first, lookup->second.second);
        int index = 0;
        for (const string& s : parameterList)
        {
            inst.SetParameter(index++, s);
        }
        instructions.push_back(inst);
    }
    infile.close();

    return successfulParse;
}
