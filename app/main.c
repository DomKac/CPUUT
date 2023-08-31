#include <cpu.h>
#include <queue.h>
#include <reader.h>
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

/* Threads */
static pthread_t reader;

/* Initialization functions */
static int initialize_threads(void);
static int initialize_resources(void);
static int initialize_queues(void);
static int initialize_reader_args(void);

/* Clear functions */
static void delete_resources(void);
static void finalize_threads(void);

/* Other */
static long cpus_num;
static long get_cpu_num(void);

int main(void) {

    puts("test 1");
    if (initialize_resources())
        return EXIT_FAILURE;

    puts("test 2");

    if (initialize_threads())
        return EXIT_FAILURE;

    puts("test 3");

    finalize_threads();

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

    /* TO BE CONTINUED */

    return 0;
}

static int initialize_queues(void) {
    size_t cpu_stats_queue_elem_size = sizeof(CPU_Array) + sizeof(CPU) * ((size_t)cpus_num + 1);

    if ((cpu_stats_queue = queue_new(QUEUE_SIZE, cpu_stats_queue_elem_size)) == NULL) {
        return -1;
    }

    // if ((cpu_usage_queue = queue_new(QUEUE_SIZE, ...)) == NULL) {
    //     return -1;
    // }

    /* TO BE CONTINUED */

    return 0;
}

/* Możliwe że lepiej przenieść do reader.c */
static int initialize_reader_args(void) {

    // reader_args = malloc(sizeof(Reader_arguments));

    // if (reader_args == NULL) {
    //     perror("Reader_args malloc failed");
    //     return -1;
    // }

    if (cpus_num == 0) {
        perror("Didn't find any cpu");
        return -1;
    }

    if (cpu_stats_queue == NULL) {
        perror("CPU info queue didn't initialized");
        return -1;
    }

    reader_args = (Reader_arguments){
        .cpu_num = (size_t)cpus_num + 1, /* /proc/stat has 1 additional general cpu stats with sum of real cpus stats*/
        .cpu_stats_queue = cpu_stats_queue,
    };

    return 0;
}

static int initialize_threads(void) {
    if (pthread_create(&reader, NULL, reader_func, &reader_args)) {
        perror("Reader creation failed");
        return -1;
    }
    return 0;
}

static void delete_resources(void) {

    if (cpu_stats_queue != NULL)
        free(cpu_stats_queue);

    // if (reader_args != NULL)
    //     free(reader_args);

    /* TO BE CONTINUED */
}

static void finalize_threads(void) {
    pthread_join(reader, NULL);
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

    return ret;
}
