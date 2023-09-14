#ifndef READER_H
#define READER_H

#include <queue.h>
#include <pcp_sentry.h>
#include <watchdog_unit.h>
#include <stdio.h>
#include <signal.h>

typedef struct Reader_arguments {

    size_t cpu_num; /* Liczba cpu w /proc/stat */
    FILE* proc_stat_file;
    Queue* cpu_stats_queue;
    PCP_Sentry* cpu_stats_queue_sentry;
    Watchdog_unit* wdog_reader;
    volatile sig_atomic_t *signal_received;

} Reader_arguments;

void* reader_func(void* reader_args);


#endif
