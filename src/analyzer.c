#include <analyzer.h>
#include <cpu.h>
#include <stdio.h>

static double calulate_cpu_usage(const CPU* const cpu);
static void print_cpu_info(const CPU_Array *const cpu_arr, size_t cpu_num);

void* analyzer_func(void* analyzer_args) {
    
    /* INITIALIZATIONS & CONTRACTS */
    if (analyzer_args == NULL) {
        perror("Analyzer: received NULL argument");
        return NULL;
    }

    register size_t cpu_num = 0;
    Queue* cpu_stats_queue = NULL;
    Queue* cpu_usage_queue = NULL;

    {
        Analyzer_arguments *temp_arg = analyzer_args;
        
        cpu_num = temp_arg->cpu_num;
        if (cpu_num == 0) {
            perror("Analyzer: variable assign failed (cpu_num)");
            return NULL;
        }

        cpu_stats_queue = temp_arg->cpu_stats_queue;
        if (cpu_stats_queue == NULL) {
            perror("Analyzer: variable assign failed (cpu_stats_queue)");
            return NULL;
        }

        cpu_usage_queue = temp_arg->cpu_usage_queue;
        if (cpu_usage_queue == NULL) {
            perror("Analyzer: variable assign failed (cpu_usage_queue)");
            return NULL;
        }
    }

    CPU_Array* cpu_arr = cpu_array_new(cpu_num);
    if (cpu_arr == NULL) {
        return NULL;
    }

    size_t cpu_usage = 0;

    /* THREAD "MAIN WORK" LOOP */
    for (;;) {
        if (!queue_is_empty(cpu_stats_queue)) {
            queue_pop(cpu_stats_queue, cpu_arr);
            
            print_cpu_info(cpu_arr, cpu_num);
        }
    }

    /* FREE RESOURCES */
    cpu_array_delete(cpu_arr);
}

static void print_cpu_info(const CPU_Array *const cpu_arr, size_t cpu_num) {
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
