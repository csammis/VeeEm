#include "SystemCall.h"

#include "Syscall.h"
#include "syscalls/Print.h"

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
    case Syscall::PRINT:
        return std::unique_ptr<SystemCall>(new Print);
        break;
    }
}

