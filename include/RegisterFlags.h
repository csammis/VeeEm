#ifndef __REGISTERH__
#define __REGISTERH__

// Describes an execution error encountered in a Context
enum class ContextError 
{
    NONE,
    SYSCALL_OUT_OF_RANGE,
    LOCATION_FORMAT_UNSUPPORTED,
    LOCATION_REGISTER_OUT_OF_RANGE,
    CONDITIONAL_JUMP_WITHOUT_COMPARE,
    LABEL_NOT_FOUND,
    POP_UNBALANCED_WITH_PUSH,
    RETURN_UNBALANCED_WITH_CALL
};

#endif //__REGISTERH__
