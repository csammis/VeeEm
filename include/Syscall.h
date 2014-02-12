#ifndef __SYSCALL_H__
#define __SYSCALL_H__

enum class Syscall
{
    DUMPCONTEXT,
    PUTCHAR
};

#define MIN_SYSCALL 0
#define MAX_SYSCALL 1

#endif //__SYSCALL_H__
