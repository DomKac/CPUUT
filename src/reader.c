#include <cpu.h>
#include <reader.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

/* musimy mieć kolejkę w którą władujemy przeczytane informacje */
void *reader_func(void *reader_args) {

    /* INITIALIZATIONS & CONTRACTS */
    if (reader_args == NULL) {
        perror("Reader: received NULL argument");
        return NULL;
    }

    size_t cpu_num = 0;
    FILE *proc_stat_file = NULL;
    Queue* cpu_stats_queue = NULL;
    PCP_Sentry* cpu_stats_queue_sentry = NULL;

    {
        Reader_arguments* temp_arg = reader_args;

        cpu_num = temp_arg->cpu_num;
        if (cpu_num == 0) {
            perror("Reader: variable assign failed (cpu_num)");
            return NULL;
        }

        proc_stat_file = temp_arg->proc_stat_file;
        if (proc_stat_file == NULL) {
            perror("Reader: variable assign failed (proc_stat_file)");
            return NULL;
        }

        cpu_stats_queue = temp_arg->cpu_stats_queue;
        if (cpu_stats_queue == NULL) {
            perror("Reader: variable assign failed (cpu_stats_queue)");
            return NULL;
        }

        cpu_stats_queue_sentry = temp_arg->cpu_stats_queue_sentry;
        if (cpu_stats_queue_sentry == NULL) {
            perror("Reader: variable assign failed (cpu_stats_queue_sentry)");
            return NULL;
        }
    }

    const struct timespec sleep_time = {.tv_nsec = 0, .tv_sec = 1};
    CPU cpu_arr[cpu_num];

    /* THREAD "MAIN WORK" LOOP */
    while (true) {
        for (size_t i = 0; i < cpu_num; i++) {
            if (!fscanf(proc_stat_file, "%s %zu %zu %zu %zu %zu %zu %zu %zu %zu %zu\n", 
                 cpu_arr[i].name, 
                &cpu_arr[i].user, 
                &cpu_arr[i].nice, 
                &cpu_arr[i].system, 
                &cpu_arr[i].idle, 
                &cpu_arr[i].iowait, 
                &cpu_arr[i].irq, 
                &cpu_arr[i].softirq, 
                &cpu_arr[i].steal, 
                &cpu_arr[i].guest, 
                &cpu_arr[i].guest_nice))
            {
                perror("Reader: reading from file failed");
                fclose(proc_stat_file);
                return NULL;
            }
        }

        pcp_sentry_lock(cpu_stats_queue_sentry);
        if (queue_is_full(cpu_stats_queue)) {
            printf("Reader: Czekam na opróżnienie kolejki przez Analyzer\n");
            pcp_sentry_wait_for_consumer(cpu_stats_queue_sentry);
        }

        queue_insert(cpu_stats_queue, cpu_arr);
        printf("Reader: Wstawiłem stats do kolejki\n");

        pcp_sentry_call_consumer(cpu_stats_queue_sentry);
        pcp_sentry_unlock(cpu_stats_queue_sentry);

        fflush(proc_stat_file);
        rewind(proc_stat_file);

        nanosleep(&sleep_time, NULL);
    }

    fclose(proc_stat_file);
}

