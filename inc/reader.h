#ifndef READER_H
#define READER_H

#include <queue.h>
#include <stdio.h>

#define PROC_STAT_FILE "/proc/stat"

typedef struct Reader_arguments{

    /* FILE* proc_stat_file; ? */
    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue *cpu_stats_queue;
    /* Watchhouse */

} Reader_arguments;

void* reader_func(void* reader_args);


#endif
