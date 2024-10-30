#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "enum.h"

const int MAX_SIZE_CMD = 15;
const int NUM_LABELS = 10;

struct labels {
    int adress;
    char name[MAX_SIZE_CMD];
};

struct table_labels {
    labels* array;
    size_t count;
};

struct Command {
    const char* cmd;
    int         id;
    int         arg;
};

const Command options[] = {
    {"PUSH", PUSH, 2},
    {"ADD",  ADD,  1},
    {"SUB",  SUB,  1},
    {"DIV",  DIV,  1},
    {"MUL",  MUL,  1},
    {"SQRT", SQRT, 1},
    {"IN",   IN,   1},
    {"OUT",  OUT,  1},
    {"POP",  POP,  2},
    {"JA",   JA,   1},
    {"JB",   JB,   1},
    {"JMP",  JMP,  1},
    {"JE",   JE,   1},
    {"CALL", CALL, 1},
    {"RET",  RET,  1},
    {"HLT",  HLT,  1},
};

void assemble(const char* file_name);

int compare_labels(const char* str1, const char* str2);

void mem_cpy(char* str1, char* str2, size_t size); 

void get_labels_array(const char* file_name, table_labels* my_labels);

void assemble_push(FILE* cmd, FILE* machine_cmd);

int assemble_pop(FILE* cmd, FILE* machine_cmd);

void assemble_jmp(FILE* cmd, FILE* machine_cmd, table_labels* my_labels);

#endif // ASSEMBLER_H
