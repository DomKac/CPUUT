#ifndef PRINTER_H
#define PRINTER_H

#include <queue.h>

typedef struct Printer_arguments {

    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue* cpu_usage_queue;

} Printer_arguments;

void* printer_func(void* printer_args);

#endif
