#ifndef __REGISTERH__
#define __REGISTERH__

// Describes an execution error encountered in a Context
enum class ContextError 
{
    NONE,
    SYSCALL_OUT_OF_RANGE,
    LOCATION_FORMAT_UNSUPPORTED,
    LOCATION_REGISTER_OUT_OF_RANGE,
};

#endif //__REGISTERH__
