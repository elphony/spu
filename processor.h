#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack/stack.h"
#include "enum.h"

#define OPERATION(stk, sign) {              \
    StackElem_t a = stack_pop(stk);         \
    StackElem_t b = stack_pop(stk);         \
    StackElem_t res = a sign b;             \
    stack_push(stk, res);                   \ 
} 

struct spu {
    Stack_t stk;
    int* code;
    size_t size_code;
    size_t ip;
    int registers[NUM_REGISTERS]; 
    Stack_t stk_ret;
};

void processor(const char* name_file, spu* dream_team);

void input_file(const char* name_file, spu* dream_team);

int switch_cmd(spu* dream_team);

int get_arg(spu* dream_team);

#endif // PROCESSOR_H