#include <cpu.h>
#include <reader.h>
#include <unistd.h>
#include <errno.h>

// static long get_cpu_num(void);
static void print_cpu_info(const CPU_Array *const cpu_arr, size_t cpu_num);

/* musimy mieć kolejkę w którą władujemy przeczytane informacje */
void *reader_func(void *reader_args) {

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

    FILE *proc_stat_file = fopen(PROC_STAT_FILE, "r");
    /* ? */
    if (proc_stat_file == NULL) {
        perror("Reader: IO error");
        return NULL;
    }

    CPU_Array *cpu_arr = cpu_array_new(reader->cpu_num + 1);

    if (cpu_arr == NULL)
        return NULL;

    for (size_t j = 0; j < 10; j++) {
        for (size_t i = 0; i < reader->cpu_num; i++) {
            if (!fscanf(proc_stat_file, "%s %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu\n", 
                cpu_arr->arr[i].name, 
                &cpu_arr->arr[i].user, 
                &cpu_arr->arr[i].nice, 
                &cpu_arr->arr[i].system, 
                &cpu_arr->arr[i].idle, 
                &cpu_arr->arr[i].iowait, 
                &cpu_arr->arr[i].irq, 
                &cpu_arr->arr[i].softirq, 
                &cpu_arr->arr[i].steal, 
                &cpu_arr->arr[i].guest, 
                &cpu_arr->arr[i].guest_nice))
            {
                perror("fscanf failed");
                free(cpu_arr);
                fclose(proc_stat_file);
                return NULL;
            }
        }

        fflush(proc_stat_file);
        rewind(proc_stat_file);

        print_cpu_info(cpu_arr, reader->cpu_num);

        sleep(2);
    }

    free(cpu_arr);
    fclose(proc_stat_file);
}

static void print_cpu_info(const CPU_Array* const cpu_arr, size_t cpu_num) {
    for (size_t i = 0; i < cpu_num; i++) {
        printf("%s ", cpu_arr->arr[i].name);
        printf("%zu ", cpu_arr->arr[i].user);
        printf("%zu ", cpu_arr->arr[i].nice);
        printf("%zu ", cpu_arr->arr[i].system);
        printf("%zu ", cpu_arr->arr[i].idle);
        printf("%zu ", cpu_arr->arr[i].iowait);
        printf("%zu ", cpu_arr->arr[i].irq);
        printf("%zu ", cpu_arr->arr[i].softirq);
        printf("%zu ", cpu_arr->arr[i].steal);
        printf("%zu ", cpu_arr->arr[i].guest);
        printf("%zu\n", cpu_arr->arr[i].guest_nice);
    }
    puts("");
}
