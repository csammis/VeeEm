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
    JUMP_EQUAL,
    JUMP_NOT_EQUAL,
    JUMP_LESS_THAN,
    JUMP_LESS_THAN_EQUAL_TO,
    JUMP_GREATER_THAN,
    JUMP_GREATER_THAN_EQUAL_TO,
    SYSCALL
};

#endif //__OPCODE_H__
