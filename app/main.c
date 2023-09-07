#include <cpu.h>
#include <queue.h>
#include <reader.h>
#include <analyzer.h>
#include <printer.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_SIZE 100

/* Queues to Producer-Consumer-Problem */
static Queue* cpu_stats_queue;
static Queue* cpu_usage_queue;

/* Threads arguments */
static Reader_arguments reader_args;
static Analyzer_arguments analyzer_args;
static Printer_arguments printer_args;

/* Threads */
static pthread_t reader;
static pthread_t analyzer;
static pthread_t printer;

/* Initialization functions */
static int initialize_threads(void);
static int initialize_resources(void);
static int initialize_queues(void);
static int initialize_reader_args(void);
static int initialize_analyzer_args(void);
static int initialize_printer_args(void);

/* Clear functions */
static void delete_resources(void);
static void join_threads(void);

/* Other */
static long cpus_num;
static long get_cpu_num(void);

int main(void) {

    if (initialize_resources()) {
        delete_resources();
        return EXIT_FAILURE;
    }

    if (initialize_threads()) {
        delete_resources();
        return EXIT_FAILURE;
    }

    join_threads();

    delete_resources();

    return EXIT_SUCCESS;
}

static int initialize_resources(void) {
    
    if ((cpus_num = get_cpu_num()) < 1) {
        return -1;
    }

    if (initialize_queues() != 0) {
        return -1;
    }

    if (initialize_reader_args() != 0) {
        perror("Reader arguments initialization failed");
        return -1;
    }

    if (initialize_analyzer_args() != 0) {
        perror("Analyzer arguments initialization failed");
        return -1;
    }

    if (initialize_printer_args() != 0) {
        perror("Printer arguments initialization failed");
        return -1;
    }
    
    /* TO BE CONTINUED */

    return 0;
}

static int initialize_queues(void) {

    size_t cpu_stats_elem_size = sizeof(CPU[cpus_num]);

    if ((cpu_stats_queue = queue_new(QUEUE_SIZE, cpu_stats_elem_size)) == NULL) {
        return -1;
    }

    size_t cpu_usage_elem_size = sizeof(double[cpus_num]);

    if ((cpu_usage_queue = queue_new(QUEUE_SIZE, cpu_usage_elem_size)) == NULL) {
        return -1;
    }

    /* TO BE CONTINUED */

    return 0;
}

static int initialize_reader_args(void) {

    if (cpus_num < 1) {
        perror("Can't find any cpu");
        return -1;
    }

    if (cpu_stats_queue == NULL) {
        perror("CPU stats queue doesn't initialized");
        return -1;
    }

    /* może lepiej do reader.c ? */
    FILE *proc_stat_file = fopen("/proc/stat", "r");
    if (proc_stat_file == NULL) {
        perror("Can't open /proc/stat file");
        return -1;
    }

    reader_args = (Reader_arguments) {
        .cpu_num = (size_t)cpus_num,
        .cpu_stats_queue = cpu_stats_queue,
        .proc_stat_file = proc_stat_file,
    };

    return 0;
}

static int initialize_analyzer_args(void) {

    if (cpus_num < 1) {
        perror("Can't find any cpu");
        return -1;
    }

    if (cpu_stats_queue == NULL) {
        perror("CPU stats queue doesn't initialized");
        return -1;
    }

    if (cpu_usage_queue == NULL) {
        perror("CPU usage queue doesn't initialized");
        return -1;
    }

    analyzer_args = (Analyzer_arguments) {
        .cpu_num = (size_t)cpus_num, 
        .cpu_stats_queue = cpu_stats_queue,
        .cpu_usage_queue = cpu_usage_queue,
    };

    return 0;
}

static int initialize_printer_args(void) {

    if (cpus_num < 1) {
        perror("Can't find any cpu");
        return -1;
    }

    if (cpu_usage_queue == NULL) {
        perror("CPU usage queue doesn't initialized");
        return -1;
    }

    printer_args = (Printer_arguments){
        .cpu_num = (size_t)cpus_num,
        .cpu_usage_queue = cpu_usage_queue,
    };

    return 0;
}

static int initialize_threads(void) {

    if (pthread_create(&reader, NULL, reader_func, &reader_args) != 0) {
        perror("Reader creation failed");
        return -1;
    }

    if (pthread_create(&analyzer, NULL, analyzer_func, &analyzer_args) != 0) {
        perror("Analyzer creation failed");
        return -1;
    }

    if (pthread_create(&printer, NULL, printer_func, &printer_args) != 0) {
        perror("Printer creation failed");
        return -1;
    }

    /* TO BE CONTINUED */

    return 0;
}

static void delete_resources(void) {

    if (cpu_stats_queue != NULL)
        queue_delete(cpu_stats_queue);

    if (cpu_usage_queue != NULL)
        queue_delete(cpu_usage_queue);

    /* TO BE CONTINUED */
}

static void join_threads(void) {

    if (pthread_join(reader, NULL) != 0) {
        perror("Failed to join Reader");
    }
    if (pthread_join(analyzer, NULL) != 0) {
        perror("Failed to join Analyzer");
    }
    if (pthread_join(printer, NULL) != 0) {
        perror("Failed to join Printer");
    }
}

static long get_cpu_num(void) {

    errno = 0;

    #ifndef _SC_NPROCESSORS_ONLN
        perror("Reading number of CPUs failed");
        return -1;
    #endif

    register const long ret = sysconf(_SC_NPROCESSORS_ONLN);
    if (ret == -1 && errno != 0) {
        perror("Reading number of CPUs didn't succeeded");
        return -1;
    }

    /* /proc/stat has 1 additional general cpu stats with sum of real cpus stats*/
    return ret + 1; 
}
