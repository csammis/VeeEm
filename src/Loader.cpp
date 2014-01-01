#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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
    using namespace std;
    using namespace VeeEm::Core;
    using namespace VeeEm::Core::Utils;

    CoreLogger::Initialize(LogLevel::LOG_DEBUG);

    vector<Instruction> instructions;
    if (!LoadInstructions(infile, instructions))
    {
        return 1;
    }

    stringstream message;
    message << "Loaded " << instructions.size() << " instructions";
    CoreLogger::Write(LogLevel::LOG_DEBUG, message.str());

    CoreLogger::Teardown();
    return 0;
}

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions)
{
    using namespace std;
    using namespace VeeEm::Core;
    using namespace VeeEm::Core::Utils;

    InitOpcodeParseMap();
    int linenumber = 1;

    while (infile.good())
    {
        string raw_instruction;
        getline(infile, raw_instruction);

        string name = raw_instruction.substr(0, raw_instruction.find_first_of(' '));

        if (raw_instruction.empty() || name.empty())
        {
            continue;
        }

        auto lookup = opcodeParses.find(name);
        if (lookup == opcodeParses.end())
        {
            stringstream message;
            message << "Unknown instruction found during parse: " << name << " (line " << linenumber << ")";
            CoreLogger::Write(LogLevel::LOG_ERROR, message.str());
            return false;
        }

        Instruction inst(lookup->second.first);
        string parameters = raw_instruction.substr(name.size() + 1);
        for (int i = 0; i < lookup->second.second; i++)
        {
            string::size_type splitAt = parameters.find_first_of(',');
            if (splitAt == string::npos)
            {
                splitAt = parameters.size();
            }
            string parameter = Trim(parameters.substr(0, splitAt));
            inst.AddParameter(parameter);
            parameters = parameters.substr(splitAt);
        }
        instructions.push_back(inst);

        linenumber++;
    }
    infile.close();

    return true;
}
