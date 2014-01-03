#include "Context.h"

#include <string.h>
#include <stdlib.h>

#include "CoreLogger.h"

Context::Context()
    : InstrPtr(0), Error(ContextError::NONE)
{
    memset(&Registers, 0, 32 * sizeof(unsigned int));
}

Context::~Context()
{
}

unsigned int* Context::ResolveLocationReference(const std::string& arg)
{
    using namespace VeeEm::Core;

    if (arg.empty())
    {
        CoreLogger::Write(LogLevel::LOG_WARNING, "ResolveLocationReference called with empty argument. This is almost certainly wrong.");
        return nullptr;
    }

    if (arg[0] == 'r')
    {
        int registerIndex = strtol(arg.substr(1).c_str(), nullptr, 0);
        if (registerIndex < 0 || registerIndex > 31)
        {
            CoreLogger::Write(LogLevel::LOG_ERROR, "ResolveLocationReference asked for out-of-range register value (" + arg + ")");
            Error = ContextError::LOCATION_REGISTER_OUT_OF_RANGE;
            return nullptr;
        }
        return &(Registers[registerIndex]);
    }

    Error = ContextError::LOCATION_FORMAT_UNSUPPORTED;
    return nullptr;
}

