#include "SystemCall.h"

#include "Syscall.h"

#include "syscalls/DumpContext.h"
#include "syscalls/Putchar.h"

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
    case Syscall::PUTCHAR:
        return std::unique_ptr<SystemCall>(new Putchar);
    }
}

