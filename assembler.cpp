#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assembler.h"

const char* reg_spu[] = {"AX", "BX", "CX", "DX", "EX"};

void assemble(const char* file_name) {

    table_labels my_labels = {};
    get_labels_array(file_name, &my_labels);

    FILE* cmd = fopen(file_name, "rb");
    FILE* machine_cmd = fopen("machine_cmd", "w");

    int num_cmd = sizeof(options)/sizeof(options[0]);

    char new_cmd[MAX_SIZE_CMD] = {};
    int flag_scan = fscanf(cmd, "%s", new_cmd);

    while (flag_scan == 1) {
        bool flag_cmd = 0;
        for (int i = 0; i < num_cmd; ++i) {

            if (strcmp(new_cmd, options[i].cmd) == 0) {
                flag_cmd = 1;
                fprintf(machine_cmd, "%d ", options[i].id);

                if (options[i].id == PUSH) {
                    assemble_push(cmd, machine_cmd);
                }

                if (options[i].id == POP) {
                    if(assemble_pop(cmd, machine_cmd)) {
                        continue;
                    }
                }

                if (JA <= options[i].id && options[i].id <= CALL) {
                    assemble_jmp(cmd, machine_cmd, &my_labels);                    
                }

                flag_scan = fscanf(cmd, "%s", new_cmd);
                fprintf(machine_cmd, "\n");
                break;
            }
        }
        if (flag_cmd == 0) {
        for (int i = 0; i < my_labels.count; ++i) {
            if (compare_labels(my_labels.array[i].name, new_cmd) == 0) {
                flag_scan == fscanf(cmd, "%s", new_cmd);
                break;
            }
        }
        }
    }

    free(my_labels.array);
    fclose(cmd);
    fclose(machine_cmd);
}

int compare_labels(const char* str1, const char* str2) {

    int res = 0;
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        res = str1[i] - str2[i];
        if (res != 0 && (str1[i] == ':' || str2[i] == ':')) {
            return 0;
        }
        else if (res != 0) {
            return 1;
        }
        i++;
    }
    return 0;
}

void get_labels_array(const char* file_name, table_labels* my_labels) {

    FILE* cmd = fopen(file_name, "rb");

    char new_cmd[MAX_SIZE_CMD] = {};
    int num_cmd = sizeof(options)/sizeof(options[0]);
    
    my_labels->array = (labels*)calloc(NUM_LABELS, sizeof(labels));
    my_labels->count = 0;
    int counter = 0;

    while (fscanf(cmd, "%s", new_cmd) == 1) {

        int new_count = 1;
        for (int i = 0; i < num_cmd; ++i) {
            if (strcmp(new_cmd, options[i].cmd) == 0) {
                new_count = options[i].arg;
                break;
            }
        }

        counter += new_count;

        if (strchr(new_cmd, ':')) {

            counter--;
            bool flag = 0;
            for (int i = 0; i < my_labels->count; ++i) {
                if (compare_labels(my_labels->array[i].name, new_cmd) == 0) {
                    flag = 1;
                    my_labels->array[i].adress = counter;
                    break;
                }
            }
            if (flag == 0) {
                my_labels->array[my_labels->count].adress = counter;
                memcpy(my_labels->array[my_labels->count].name, new_cmd, MAX_SIZE_CMD); 
                my_labels->count++;
            }
        }

        if (strcmp(new_cmd, "JMP") == 0 || (strcmp(new_cmd, "JB") == 0) || (strcmp(new_cmd, "JA") == 0) || strcmp(new_cmd, "CALL") == 0) {

            fscanf(cmd, "%s", new_cmd);
            counter++;
            bool flag = 0;
            for (int i = 0; i < my_labels->count; ++i) {
                if (compare_labels(my_labels->array[i].name, new_cmd) == 0) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                memcpy(my_labels->array[my_labels->count].name, new_cmd, MAX_SIZE_CMD);
                my_labels->count++;
            }
        }
    }
    fclose(cmd);
}

void assemble_push(FILE* cmd, FILE* machine_cmd) {

    char new_cmd[MAX_SIZE_CMD] = {};
    fscanf(cmd, "%s", new_cmd); 

    bool flag = 0;
    for (int i = 0; i < NUM_REGISTERS; ++i) {
        if (strcmp(new_cmd, reg_spu[i]) == 0) {
            fprintf(machine_cmd, "1 %d ", i);
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        fprintf(machine_cmd, "0 %s", new_cmd);
    }
}

int assemble_pop(FILE* cmd, FILE* machine_cmd) {

    char new_cmd[MAX_SIZE_CMD] = {};
    fscanf(cmd, "%s ", new_cmd);
    
    bool flag = 0;
    for (int j = 0; j < NUM_REGISTERS; ++j) {
        if (strcmp(new_cmd, reg_spu[j]) == 0) {
            fprintf(machine_cmd, "1 %d ", j);
            flag = 1;
            return 0;
        }
    }
    if (flag == 0) {
        fprintf(machine_cmd, "0 0\n");
        return 1;
    }   
}

void assemble_jmp(FILE* cmd, FILE* machine_cmd, table_labels* my_labels) {

    char new_cmd[MAX_SIZE_CMD] = {};
    fscanf(cmd, "%s ", new_cmd);

    for (int j = 0; j < my_labels->count; ++j) {
        if (compare_labels(my_labels->array[j].name, new_cmd) == 0) {
            fprintf(machine_cmd, "%d ", my_labels->array[j].adress);
        }
    }
}