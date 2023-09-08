#ifndef READER_H
#define READER_H

#include <queue.h>
#include <pcp_sentry.h>
#include <stdio.h>

typedef struct Reader_arguments {

    size_t cpu_num; /* Liczba cpu w /proc/stat */
    FILE* proc_stat_file;
    Queue* cpu_stats_queue;
    PCP_Sentry* cpu_stats_queue_sentry;

} Reader_arguments;

void* reader_func(void* reader_args);


#endif
