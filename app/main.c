#include "../inc/queue.h"
#include <errno.h>
#include <reader.h>
#include <cpu.h>
#include <stdlib.h>
#include <unistd.h>

static long get_cpu_num(void);

#define QUEUE_SIZE 100

static long cpus_num;

static Queue* cpu_info_queue;
static Queue* cpu_usage_queue;

static Reader_arguments* reader_args;

static int initialize_resources(void);
static int initialize_queues(void);
static int initialize_reader_args(void);

static void delete_resources(void);

int main(void) {

    if (initialize_resources())
        return EXIT_FAILURE;

    delete_resources();

    return EXIT_SUCCESS;
}

static int initialize_resources(void) {

    if ((cpus_num = get_cpu_num()) != -1) {
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
    size_t queue_elem_size = sizeof(CPU_Array) + sizeof(CPU) * ((size_t)cpus_num + 1);

    if ((cpu_info_queue = queue_new(QUEUE_SIZE, queue_elem_size)) == NULL) {
        return -1;
    }

    /* TO BE CONTINUED */

    return 0;
}

/* Możliwe że lepiej przenieść do reader.c */
static int initialize_reader_args(void) {

    reader_args = malloc(sizeof(Reader_arguments));

    if (reader_args == NULL) {
        perror("Reader_args malloc failed");
        return -1;
    }

    if (cpus_num == 0) {
        perror("Didn't find any cpu");
        return -1;
    }

    if (cpu_info_queue == NULL) {
        perror("CPU info queue didn't initialized");
        return -1;
    }

    *reader_args = (Reader_arguments){
        .cpu_num = cpus_num,
        .cpu_info_queue = cpu_info_queue,
    };

    return 0;
}

static void delete_resources(void) {

    if (cpu_info_queue != NULL)
        free(cpu_info_queue);

    if (reader_args != NULL)
        free(reader_args);

    /* TO BE CONTINUED */
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
