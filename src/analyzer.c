#include <analyzer.h>
#include <cpu.h>
#include <stdio.h>
#include <memory.h>

static int calulate_cpus_usage(const register size_t cpu_num, const CPU cur_cpu_arr[const cpu_num], const CPU prev_cpu_arr[const cpu_num], double cpu_usage_arr[cpu_num]);
static void print_cpu_info(size_t cpu_num, const CPU cpu_arr[cpu_num]);
static double cpu_usage(register const CPU cur_cpu, register const CPU prev_cpu);

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
        Analyzer_arguments* temp_arg = analyzer_args;
        
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

    CPU cur_cpu_arr[cpu_num];
    CPU prev_cpu_arr[cpu_num];
    double cpu_usage_arr[cpu_num];

    memset(prev_cpu_arr, 0, sizeof(CPU[cpu_num]));

    /* THREAD "MAIN WORK" LOOP */
    size_t poped = 0;
    while (poped < 10) {
        if (!queue_is_empty(cpu_stats_queue)) {
            queue_pop(cpu_stats_queue, cur_cpu_arr);
            if (calulate_cpus_usage(cpu_num, cur_cpu_arr, prev_cpu_arr, cpu_usage_arr) != 0) {
                printf("error in calculate_cpus_usage!\n");
                break;
            }
            memcpy(prev_cpu_arr, cur_cpu_arr, sizeof(CPU[cpu_num]));
            queue_insert(cpu_usage_queue, cpu_usage_arr);
            poped++;
        }
        sleep(1);
    }

    // for (;;) {
    //     if (!queue_is_empty(cpu_stats_queue)) {
    //         queue_pop(cpu_stats_queue, cur_cpu_arr);
    //         if (calulate_cpus_usage(cpu_num, cur_cpu_arr, prev_cpu_arr, cpu_usage_arr) != 0 ) {
    //             printf("error in calculate_cpus_usage!\n");
    //         }
    //         memcpy(prev_cpu_arr, cur_cpu_arr, sizeof(CPU[cpu_num]));
    //         queue_insert(cpu_usage_queue, cpu_usage_arr);
    //     }
    // }
}

static void print_cpu_info(size_t cpu_num, const CPU cpu_arr[cpu_num]) {
    for (size_t i = 0; i < cpu_num; i++) {
        printf("%s ", cpu_arr[i].name);
        printf("%zu ", cpu_arr[i].user);
        printf("%zu ", cpu_arr[i].nice);
        printf("%zu ", cpu_arr[i].system);
        printf("%zu ", cpu_arr[i].idle);
        printf("%zu ", cpu_arr[i].iowait);
        printf("%zu ", cpu_arr[i].irq);
        printf("%zu ", cpu_arr[i].softirq);
        printf("%zu ", cpu_arr[i].steal);
        printf("%zu ", cpu_arr[i].guest);
        printf("%zu\n", cpu_arr[i].guest_nice);
    }
    puts("");
}

static int calulate_cpus_usage(const register size_t cpu_num, const CPU cur_cpu_arr[const cpu_num], const CPU prev_cpu_arr[const cpu_num], double cpu_usage_arr[cpu_num]) {
    
    if (cpu_num == 0) {
        perror("Number of cpus equal to 0");
        return -1;
    }
    if (cur_cpu_arr == NULL || prev_cpu_arr == NULL || cpu_usage_arr == NULL) {
        perror("One of pointers is NULL");
        return -1;
    }

    for (size_t i = 0; i < cpu_num; i++) {
        cpu_usage_arr[i] = cpu_usage(cur_cpu_arr[i], prev_cpu_arr[i]);
    }

    return 0;
}

static double cpu_usage(register const CPU cur_cpu, register const CPU prev_cpu) {
    size_t PrevIdle = prev_cpu.idle + prev_cpu.iowait;
    size_t Idle = cur_cpu.idle + cur_cpu.iowait;

    size_t PrevNonIdle = prev_cpu.user + prev_cpu.nice + prev_cpu.system + prev_cpu.irq + prev_cpu.softirq + prev_cpu.steal;
    size_t NonIdle = cur_cpu.user + cur_cpu.nice + cur_cpu.system + cur_cpu.irq + cur_cpu.softirq + cur_cpu.steal;

    size_t PrevTotal = PrevIdle + PrevNonIdle;
    size_t Total = Idle + NonIdle;

    // differentiate: actual value minus the previous one
    double totald = (double)(Total - PrevTotal);
    double idled = (double)(Idle - PrevIdle);

    return (totald - idled) / (double)totald * 100.0;
}
