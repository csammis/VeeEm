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
#include "VeeEmProgram.h"

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions, std::map<std::string, int>& labels);
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
    std::map<std::string, int> labels;
    if (!LoadInstructions(infile, instructions, labels))
    {
        return 1;
    }

    Log::Instance(LogLevel::DEBUG) << "Loaded " << instructions.size() << " instructions" << End();
    Log::Instance(LogLevel::DEBUG) << "Beginning execution." << End();
    
    VeeEmProgram program;
    program.SetInstructions(instructions);
    program.SetLabels(labels);
    program.Execute();
    
    Log::Instance(LogLevel::DEBUG) << "Halting." << End();

    Log::Teardown();
    return 0;
}

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions, std::map<std::string, int>& labels)
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

        raw_instruction = Trim(raw_instruction);
        if (raw_instruction.empty())
        {
            continue;
        }

        string::size_type nameEnd = raw_instruction.find_first_of(" \t");
        if (nameEnd == string::npos)
        {
            nameEnd = raw_instruction.size();
        }
        
        string name = raw_instruction.substr(0, nameEnd);
        if (name.empty())
        {
            continue;
        }

        // Add a new label to the label-to-location map. Labels refer to the next non-label instruction.
        if (name.back() == ':')
        {
            string label = name.substr(0, name.size() - 1);
            if (label.empty())
            {
                Log::Instance(LogLevel::ERROR) << "Blank label found during parse at line " << linenumber << End();
                successfulParse = false;
                continue;
            }
            labels[label] = instructions.size();
            continue;
        }

        auto lookup = opcodeParses.find(name);
        if (lookup == opcodeParses.end())
        {
            Log::Instance(LogLevel::ERROR) << "Unknown instuction found during parse: " << name << " (line " << linenumber << ")" << End();
            successfulParse = false;
            continue;
        }

        Log::Instance(LogLevel::DEBUG) << "Parsing line with instruction named " << name << End();

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

