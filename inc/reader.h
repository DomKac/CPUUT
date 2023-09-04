#ifndef READER_H
#define READER_H

#include <queue.h>
#include <stdio.h>

typedef struct Reader_arguments {

    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue* cpu_stats_queue;
    FILE* proc_stat_file;
    /* Watchhouse */

} Reader_arguments;

void* reader_func(void* reader_args);


#endif
