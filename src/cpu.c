#include "cpu.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

CPU_Array* cpu_array_new(register const size_t cpu_num) {
    
    if (cpu_num == 0) {
        perror("Can't create array of size 0");
        return NULL;
    }

    CPU_Array *const cpu_arr = calloc(1, sizeof(CPU_Array) + cpu_num * sizeof(CPU));

    if (cpu_arr == NULL) {
        perror("Memory allocation for CPU_Array didn't succeded");
        return NULL;
    }

    cpu_arr->cpu_num = cpu_num;

    return cpu_arr;
}

void cpu_array_delete(CPU_Array* const cpu_arr) {
    if (cpu_arr == NULL)
        return;
    free(cpu_arr);
}
