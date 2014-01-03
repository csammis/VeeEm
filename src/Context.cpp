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
    using namespace VeeEm::Core::Logger;

    if (arg.empty())
    {
        Log::Instance(LogLevel::WARNING) << "ResolveLocationReference called with empty argument. This is almost certainly wrong." << End();
        return nullptr;
    }

    if (arg[0] == 'r')
    {
        int registerIndex = strtol(arg.substr(1).c_str(), nullptr, 0);
        if (registerIndex < 0 || registerIndex > 31)
        {
            Log::Instance(LogLevel::ERROR) << "ResolveLocationReference asked for out-of-range register value (" << arg << ")" << End();
            Error = ContextError::LOCATION_REGISTER_OUT_OF_RANGE;
            return nullptr;
        }
        return &(Registers[registerIndex]);
    }

    Error = ContextError::LOCATION_FORMAT_UNSUPPORTED;
    return nullptr;
}

bool Context::ResolveValue(const std::string& arg, unsigned int& value)
{
    using namespace VeeEm::Core::Logger;

    if (arg.empty())
    {
        Log::Instance(LogLevel::WARNING) << "ResolveValue called with empty argument." << End();
        return false;
    }

    if (arg[0] == '$')
    {
        value = static_cast<unsigned int>(strtol(arg.substr(1).c_str(), nullptr, 0));
        return true;
    }

    Log::Instance(LogLevel::ERROR) << "Non-constant values are not supported." << End();
    Error = ContextError::LOCATION_FORMAT_UNSUPPORTED;

    return false;
}

