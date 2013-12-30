#ifndef __SYSTEMCALL_H__
#define __SYSTEMCALL_H__

#include <memory>

enum class Syscall;
class Context;

class SystemCall
{
public:
    SystemCall();
    virtual ~SystemCall();

    virtual void Execute(Context& context) = 0;

    static std::unique_ptr<SystemCall> Create(Syscall type);
};

#endif //__SYSTEMCALL_H__
