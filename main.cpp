#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"
#include "processor.h"

int main() {
    assemble("square_solver.txt");

    open_log_file("LOG.log", "w");
    spu my_spu = {};

    processor("machine_cmd", &my_spu);
    close_log_file();
}