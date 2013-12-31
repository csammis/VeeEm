#ifndef __REGISTERFLAGS_H__
#define __REGISTERFLAGS_H__

enum class RegisterFlags : unsigned int
{
    FLAGS_NONE                     = 0x00000000,
    FLAGS_ERR_SYSCALL_OUT_OF_RANGE = 0x00000001
}

#endif //__REGISTERFLAGS_H__
