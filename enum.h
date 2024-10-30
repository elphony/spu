#ifndef ENUM_H
#define ENUM_H

const int NUM_REGISTERS = 5;

enum FlagCommand {
    PUSH = 0,
    ADD  = 1,
    SUB  = 2,
    DIV  = 3,
    MUL  = 4,
    SQRT = 5,
    IN   = 6,
    OUT  = 7,
    POP  = 8,
    JA   = 9,
    JB   = 10,
    JE   = 11,
    JMP  = 12,
    CALL = 13,
    RET  = 14,
    HLT  = -666
};

#endif
