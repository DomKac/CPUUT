#ifndef PRINTER_H
#define PRINTER_H

#include <queue.h>
#include <pcp_sentry.h>
#include <watchdog_unit.h>
#include <signal.h>

typedef struct Printer_arguments {

    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue* cpu_usage_queue;
    PCP_Sentry* cpu_usage_queue_sentry;
    Watchdog_unit* wdog_printer;
    volatile sig_atomic_t* signal_received;

} Printer_arguments;

void* printer_func(void* printer_args);

#endif
