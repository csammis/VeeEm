#include <string>

#include "Parser.h"
#include "Instruction.h"
#include "CoreLogger.h"
#include "VeeEmProgram.h"

int LoadProgram(const std::string& filename, VeeEmProgram& program)
{
    using namespace VeeEm::Core::Logger;

    VeeEm::Core::Parser::Parser parser(filename);
    if (!parser.Parse())
    {
        return 1;
    }

    Log::Instance(LogLevel::DEBUG) << "Loaded " << parser.Instructions().size() << " instructions" << End();

    program.SetInstructions(parser.Instructions());
    program.SetLabels(parser.Labels());
    program.SetSections(parser.Sections());

    return 0;
}

int main(int argc, char** argv)
{
    using namespace std;
    using namespace VeeEm::Core::Logger;

    Log::Initialize(LogLevel::DEBUG);

    if (argc != 2)
    {
        Log::Instance(LogLevel::ERROR) << "Usage: VeeEm <filename>" << End();
        return 1;
    }

    string filename(argv[1]);
    
    VeeEmProgram program;
    int retval = LoadProgram(filename, program);
    program.Execute();

    Log::Teardown();

    return retval;
}

