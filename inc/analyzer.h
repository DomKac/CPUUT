#ifndef ANALYZER_H
#define ANALYZER_H

#include <pcp_sentry.h>
#include <queue.h>
#include <watchdog_unit.h>
#include <signal.h>

typedef struct Analyzer_arguments {
    
    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue* cpu_stats_queue;
    Queue* cpu_usage_queue;
    PCP_Sentry* cpu_stats_queue_sentry;
    PCP_Sentry* cpu_usage_queue_sentry;
    Watchdog_unit* wdog_analyzer;
    volatile sig_atomic_t* signal_received;

} Analyzer_arguments;

void* analyzer_func(void* analyzer_args);

#endif
