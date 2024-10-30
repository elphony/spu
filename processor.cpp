#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <assert.h>
#include <math.h>

#include "processor.h"

void processor(const char* name_file, spu* dream_team) {
    input_file(name_file, dream_team);

    stack_ctor(&dream_team->stk, 1);
    stack_ctor(&dream_team->stk_ret, 1);

    while (dream_team->ip < dream_team->size_code) {
        if(switch_cmd(dream_team)) {
            stack_dtor(&dream_team->stk);
            stack_dtor(&dream_team->stk_ret);
            free(dream_team->code);
            return;
        }
    }

    stack_dtor(&dream_team->stk);
    stack_dtor(&dream_team->stk_ret);
    free(dream_team->code);
}

void input_file(const char* name_file, spu* dream_team) {
    FILE* f = fopen(name_file, "rb");
    assert(f != nullptr && "Error opening the file");

    struct stat file_stat = {};
    
    fstat(fileno(f), &file_stat);
    assert(errno == 0 && "Information reading error");

    dream_team->code = (int*)calloc(file_stat.st_size, sizeof(int));
    assert(dream_team->code != nullptr && "Memory allocation error");

    int new_cmd = 0;
    dream_team->size_code = 0;

    while (fscanf(f, "%d", &new_cmd) == 1) {
        dream_team->code[dream_team->size_code] = new_cmd;
        dream_team->size_code++;
    }

    fclose(f);
}

int switch_cmd(spu* dream_team) {

    StackElem_t new_elem = 0;

    switch (dream_team->code[dream_team->ip]) {
        case PUSH:
            stack_push(&dream_team->stk, get_arg(dream_team));
            dream_team->ip += 3;
            return 0;

        case ADD:
            OPERATION(&dream_team->stk, +);
            dream_team->ip++;
            return 0;
            
        case SUB:
            OPERATION(&dream_team->stk, -);
            dream_team->ip++;
            return 0;

        case DIV:
        {
            StackElem_t a = stack_pop(&dream_team->stk);
            StackElem_t b = stack_pop(&dream_team->stk);
            if (b == 0) {
                fprintf(stderr, "ERROR: division by zero\n");
                return 1;
            }
            StackElem_t res = a / b;
            stack_push(&dream_team->stk, res);
            dream_team->ip++;
            return 0;
        }
        case MUL:
            OPERATION(&dream_team->stk, *);
            dream_team->ip++;
            return 0;
            
        case SQRT:
            new_elem = (int)sqrt(stack_pop(&dream_team->stk));
            stack_push(&dream_team->stk, new_elem);
            dream_team->ip++;
            return 0;
            
        case IN:
            new_elem = 0;
            scanf("%d", &new_elem);
            stack_push(&dream_team->stk, new_elem);
            dream_team->ip++;
            return 0;

        case OUT:
            fprintf(stdout, "%d\n", dream_team->stk.data[dream_team->stk.size-1]);
            dream_team->ip++;
            return 0;

        case POP:
            {
            switch (dream_team->code[dream_team->ip + 1]) {
                case 0:
                    stack_pop(&dream_team->stk);
                    break;
                case 1:
                    dream_team->registers[dream_team->code[dream_team->ip + 2]] = stack_pop(&dream_team->stk);
                    break;
            }
            dream_team->ip += 3;
            return 0;
            }
        case JA:
            {
                int a = stack_pop(&dream_team->stk);
                int b = stack_pop(&dream_team->stk);
                if (a > b) {
                    dream_team->ip = dream_team->code[dream_team->ip + 1];
                }
                else {
                    dream_team->ip += 2;
                }
                return 0;
            }
        case JB:
            {
                int a = stack_pop(&dream_team->stk);
                int b = stack_pop(&dream_team->stk);
                if (a < b) {
                    dream_team->ip = dream_team->code[dream_team->ip + 1];
                }
                else {
                    dream_team->ip += 2;
                }
                return 0;
            }
        case JE:
            {
                int a = stack_pop(&dream_team->stk);
                int b = stack_pop(&dream_team->stk);
                if (a == b) {
                    dream_team->ip = dream_team->code[dream_team->ip + 1];
                }
                else {
                    dream_team->ip += 2;
                }
                return 0;
            }

        case JMP:
            dream_team->ip = dream_team->code[dream_team->ip + 1];
            return 0;

        case CALL:
            stack_push(&dream_team->stk_ret, dream_team->ip + 2);
            dream_team->ip = dream_team->code[dream_team->ip + 1];

            while (dream_team->code[dream_team->ip] != RET) {
                switch_cmd(dream_team);
            }

            dream_team->ip = stack_pop(&dream_team->stk_ret);
            return 0;

        case HLT:
            fprintf(stdout, "Победа все закончилось\n");
            dream_team->ip++;
            return 1;

        default:
            fprintf(stderr, "Мама что творится\n");
    }
}

int get_arg(spu* dream_team) {
    switch (dream_team->code[dream_team->ip + 1]) {
        case 0:
            return dream_team->code[dream_team->ip + 2];
        case 1:
            return dream_team->registers[dream_team->code[dream_team->ip + 2]];
        default:
            fprintf(stderr, "Недопустимый аргумент\n");
    }
}