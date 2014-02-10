#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>

#include "Instruction.h"
#include "Context.h"
#include "Parser.h"
#include "CoreLogger.h"
#include "VeeEmProgram.h"

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions,
        VeeEm::Core::Parser::LabelInstructionMap& labels, VeeEm::Core::Parser::LabelInstructionMap& sections);
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
    using namespace VeeEm::Core::Parser;

    Log::Initialize(LogLevel::DEBUG);

    std::vector<Instruction> instructions;
    LabelInstructionMap labels;
    LabelInstructionMap sections;
    if (!LoadInstructions(infile, instructions, labels, sections))
    {
        return 1;
    }

    Log::Instance(LogLevel::DEBUG) << "Loaded " << instructions.size() << " instructions" << End();
    Log::Instance(LogLevel::DEBUG) << "Beginning execution." << End();
    
    VeeEmProgram program;
    program.SetInstructions(instructions);
    program.SetLabels(labels);
    program.SetSections(sections);
    program.Execute();
    
    Log::Instance(LogLevel::DEBUG) << "Halting." << End();

    Log::Teardown();
    return 0;
}

bool LoadInstructions(std::ifstream& infile, std::vector<Instruction>& instructions,
        VeeEm::Core::Parser::LabelInstructionMap& labels, VeeEm::Core::Parser::LabelInstructionMap& sections)
{
    using namespace std;
    using namespace VeeEm::Core;
    using namespace VeeEm::Core::Logger;

    bool successfulParse = true; // Optimism!
    VeeEm::Core::Parser::Parser parser;
    for (int linenumber = 1; infile.good(); linenumber++)
    {
        string raw_instruction;
        getline(infile, raw_instruction);

        VeeEm::Core::Parser::Line line(raw_instruction, linenumber);
        if (!parser.ParseLine(line, instructions, labels, sections))
        {
            successfulParse = false;
            continue;
        }
    }
    infile.close();

    return successfulParse;
}

