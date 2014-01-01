#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Instruction.h"
#include "ParseUtils.h"

using namespace std;

bool LoadInstructions(const string& filename, vector<Instruction>& instructions);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "Usage: VeeEm <filename>" << endl;
        return 1;
    }

    string filename(argv[1]);
    vector<Instruction> instructions;
    if (!LoadInstructions(filename, instructions))
    {
        cout << "Unable to load " << filename << endl;
        return 1;
    }

    cout << "DEBUG: Loaded " << instructions.size() << " instructions" << endl;

    return 0;
}

bool LoadInstructions(const string& filename, vector<Instruction>& instructions)
{
    using namespace VeeEm::Core::Utils;

    InitOpcodeParseMap();
    int linenumber = 1;

    ifstream infile;
    infile.open(filename.c_str());
    while (infile.good())
    {
        string raw_instruction;
        getline(infile, raw_instruction);

        string name = raw_instruction.substr(0, raw_instruction.find_first_of(' '));

        if (raw_instruction.empty() || name.empty())
        {
            continue;
        }

        cout << "DEBUG: Looking up parse information for instruction {" << name << "}" << endl;
        auto lookup = opcodeParses.find(name);
        if (lookup == opcodeParses.end())
        {
            cout << "** Unknown instruction found during parse: " << name << " (line " << linenumber << ")" << endl;
            return false;
        }

        Instruction inst(lookup->second.first);
        string parameters = raw_instruction.substr(name.size() + 1);
        cout << "DEBUG: Parsing {" << parameters << "} as parameters for instruction" << endl;
        for (int i = 0; i < lookup->second.second; i++)
        {
            string::size_type splitAt = parameters.find_first_of(',');
            if (splitAt == string::npos)
            {
                splitAt = parameters.size();
            }
            string parameter = Trim(parameters.substr(0, splitAt));
            inst.AddParameter(parameter);
            cout << "DEBUG: Added parameter {" << parameter << "} to instruction" << endl;

            parameters = parameters.substr(splitAt);
        }
        instructions.push_back(inst);

        linenumber++;
    }

    return true;
}
