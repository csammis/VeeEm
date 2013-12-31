#include <iostream>
#include <vector>
#include <string>

#include "Instruction.h"
#include "ParseUtils.h"

using namespace std;

bool LoadInstructions(const string& filename, vector<Instruction> instructions);

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

    return 0;
}

bool LoadInstructions(const string& filename, vector<Instruction> instructions)
{
    using namespace VeeEm::Core::Utils;

    InitOpcodeParseMap();

    ifstream infile;
    infile.open(filename.c_str());
    while (infile.good())
    {
        string raw_instruction;
        getline(infile, raw_instruction);

        string name = raw_instruction.substr(0, raw_instruction.find_first_of(' '));
        auto lookup = opcodeParses.find(name);
        if (lookup == opcodeParses.end())
        {
            cout << "Unknown instruction found during parse: " << name << endl;
            return false;
        }

        Instruction inst(lookup->first);
        string parameters = raw_instruction.substr(name.size() + 1);
        for (int i = 0; i < lookup->second; i++)
        {
            string parameter = parameters.substr(0, parameters.find_first_of(','));

            parameters = parameters.substr(parameter.size() + 1);
        }
    }
}
