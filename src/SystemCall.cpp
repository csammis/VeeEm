#include "SystemCall.h"

#include "Syscall.h"
#include "syscalls/DumpContext.h"

SystemCall::SystemCall()
{
}

SystemCall::~SystemCall()
{
}

std::unique_ptr<SystemCall> SystemCall::Create(Syscall type)
{
    switch (type)
    {
    case Syscall::DUMPCONTEXT:
        return std::unique_ptr<SystemCall>(new DumpContext);
        break;
    }
}

