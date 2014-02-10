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
    // Don't add instructions between the jumps unless they support label as parameters! (see OPCODE_REBASE_MIN/MAX)
    JUMP_EQUAL,
    JUMP_NOT_EQUAL,
    JUMP_LESS_THAN,
    JUMP_LESS_THAN_EQUAL_TO,
    JUMP_GREATER_THAN,
    JUMP_GREATER_THAN_EQUAL_TO,
    CALL,
    // Don't add instructions between the jumps!
    PUSH,
    POP,
    RETURN,
    SYSCALL
};

// Use these constants to define a range of opcodes which support rebasing
#define OPCODE_REBASE_MIN Opcode::JUMP_EQUAL
#define OPCODE_REBASE_MAX Opcode::CALL

#endif //__OPCODE_H__

