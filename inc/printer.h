#ifndef PRINTER_H
#define PRINTER_H

#include <queue.h>
#include <pcp_sentry.h>

typedef struct Printer_arguments {

    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue* cpu_usage_queue;
    PCP_Sentry* cpu_usage_queue_sentry;

} Printer_arguments;

void* printer_func(void* printer_args);

#endif
