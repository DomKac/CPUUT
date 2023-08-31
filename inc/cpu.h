#ifndef CPU_H
#define CPU_H
#include <stddef.h>

typedef struct CPU {
    char name[8];
    size_t user;
    size_t nice;
    size_t system;
    size_t idle;
    size_t iowait;
    size_t irq;
    size_t softirq;
    size_t steal;
    size_t guest;
    size_t guest_nice;
} CPU;

typedef struct CPU_Array {
    size_t cpu_num;
    CPU arr[];
} CPU_Array;

typedef struct CPU_Usage_Array {
    size_t cpu_num;
    double arr[];
} CPU_Usage_Array;

CPU_Array* cpu_array_new(size_t n_elem);
void cpu_array_delete(CPU_Array* cpu_arr);

#endif
