#include <cpu.h>
#include <reader.h>
#include <unistd.h>
#include <errno.h>

// static long get_cpu_num(void);

/* musimy mieć kolejkę w którą władujemy przeczytane informacje */
void* reader_func(void* reader_args) {

    if (reader_args == NULL) {
        perror("Reader: received NULL argument");
        return NULL;
    }

    Reader_arguments* reader = reader_args;
    
    /* ? */
    if (reader == NULL) {
        perror("Reader: wrong transition from reader_args");
        return NULL;
    }

    FILE *proc_stat_file = fopen("proc/stat", "r");
    /* ? */
    if (proc_stat_file == NULL) {
        perror("Reader: IO error");
        return NULL;
    }

    CPU_Array *cpu_arr = cpu_array_new(reader->cpu_num + 1);

    if (cpu_arr == NULL)
        return NULL;

    for (;;) {
        
    }
}

// static long get_cpu_num(void) {

//     errno = 0;

//     #ifndef _SC_NPROCESSORS_ONLN
//         perror("Reading number of CPUs failed");
//         return -1;
//     #endif

//     register const long ret = sysconf(_SC_NPROCESSORS_ONLN);
//     if (ret == -1 && errno != 0) {
//         perror("Reading number of CPUs didn't succeeded");
//         return -1;
//     }

//     return ret;
// }
