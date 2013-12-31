#include "Context.h"

#include <string.h>

Context::Context()
    : InstrPtr(0), Flags(0)
{
    memset(&Registers, 0, 32 * sizeof(unsigned int));
}

Context::~Context()
{
}

