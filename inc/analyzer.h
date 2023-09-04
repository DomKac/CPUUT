#ifndef ANALYZER_H
#define ANALYZER_H

#include <queue.h>
typedef struct Analyzer_arguments {
    
    size_t cpu_num; /* Liczba cpu w /proc/stat */
    Queue* cpu_stats_queue;
    Queue* cpu_usage_queue;

} Analyzer_arguments;

void* analyzer_func(void* analyzer_args);

#endif
