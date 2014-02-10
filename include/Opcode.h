#ifndef __OPCODE_H__
#define __OPCODE_H__

enum class Opcode
{
    LOAD,
    INCREMENT,
    DECREMENT,
    ADD,
    SUBTRACT,
    COMPARE,
    // Don't add instructions between the jumps! (see OPCODE_REBASE_MIN/MAX)
    JUMP_EQUAL,
    JUMP_NOT_EQUAL,
    JUMP_LESS_THAN,
    JUMP_LESS_THAN_EQUAL_TO,
    JUMP_GREATER_THAN,
    JUMP_GREATER_THAN_EQUAL_TO,
    // Don't add instructions between the jumps!
    PUSH,
    POP,
    SYSCALL
};

// Use these constants to define a range of opcodes which support rebasing
#define OPCODE_REBASE_MIN Opcode::JUMP_EQUAL
#define OPCODE_REBASE_MAX Opcode::JUMP_GREATER_THAN_EQUAL_TO

#endif //__OPCODE_H__

