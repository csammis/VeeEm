#ifndef __REGISTERH__
#define __REGISTERH__

// Describes an execution error encountered in a Context
enum class ContextError 
{
    NONE,
    SYSCALL_OUT_OF_RANGE,
    LOCATION_FORMAT_UNSUPPORTED,
    LOCATION_REGISTER_OUT_OF_RANGE,
    CONDITIONAL_JUMP_WITHOUT_COMPARE
};

#endif //__REGISTERH__
