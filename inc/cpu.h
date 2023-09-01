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

#endif
