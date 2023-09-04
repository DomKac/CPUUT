#include <printer.h>
#include <stdio.h>

static int print_cpu_usage(register const size_t cpu_num, const double cpu_usage_arr[const cpu_num]);

void* printer_func(void* printer_args) {
    /* INITIALIZATIONS & CONTRACTS */
    if (printer_args == NULL) {
        perror("Printer: received NULL argument");
        return NULL;
    }

    register size_t cpu_num = 0;
    Queue *cpu_usage_queue = NULL;

    {
        Printer_arguments *temp_arg = printer_args;

        cpu_num = temp_arg->cpu_num;
        if (cpu_num == 0) {
            perror("Printer: variable assign failed (cpu_num)");
            return NULL;
        }

        cpu_usage_queue = temp_arg->cpu_usage_queue;
        if (cpu_usage_queue == NULL) {
            perror("Printer: variable assign failed (cpu_usage_queue)");
            return NULL;
        }
    }

    double cpu_usage_arr[cpu_num];

    /* THREAD "MAIN WORK" LOOP */
    size_t poped = 0;
    while (poped < 10) {
        if (!queue_is_empty(cpu_usage_queue)) {
            queue_pop(cpu_usage_queue, cpu_usage_arr);
            print_cpu_usage(cpu_num, cpu_usage_arr);
            poped++;
        }
        sleep(1);
    }
}

static int print_cpu_usage(register const size_t cpu_num, const double cpu_usage_arr[const cpu_num]) {
    
    if (cpu_num == 0) {
        return -1;
    }
    if (cpu_usage_arr == NULL) {
        return -1;
    }    

    for (size_t i = 0; i < cpu_num; i++) {
        // printf("%s %.2f%%\n", , cpu_usage_arr[i]);        
        printf("cpu%zu %.2f%%\n", i, cpu_usage_arr[i]);
    }

    return 0;
}
