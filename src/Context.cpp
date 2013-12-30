#include "Context.h"

#include <string.h>

Context::Context()
{
    memset(&Registers, 0, 32 * sizeof(unsigned int));
}

Context::~Context()
{
}

