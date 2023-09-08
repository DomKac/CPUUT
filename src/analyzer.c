#include <analyzer.h>
#include <cpu.h>
#include <stdio.h>
#include <memory.h>

static int calulate_cpus_usage(const register size_t cpu_num, const CPU cur_cpu_arr[const cpu_num], const CPU prev_cpu_arr[const cpu_num], double cpu_usage_arr[cpu_num]);
static void print_cpu_info(size_t cpu_num, const CPU cpu_arr[cpu_num]);
static double cpu_usage(register const CPU cur_cpu, register const CPU prev_cpu);
static int print_cpu_usage(register const size_t cpu_num, const double cpu_usage_arr[const cpu_num]);

void *analyzer_func(void *analyzer_args) {

    /* INITIALIZATIONS & CONTRACTS */
    if (analyzer_args == NULL) {
        perror("Analyzer: received NULL argument");
        return NULL;
    }

    register size_t cpu_num = 0;
    Queue* cpu_stats_queue = NULL;
    Queue* cpu_usage_queue = NULL;
    PCP_Sentry* cpu_stats_queue_sentry = NULL;
    PCP_Sentry* cpu_usage_queue_sentry = NULL;

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

        cpu_stats_queue_sentry = temp_arg->cpu_stats_queue_sentry;
        if (cpu_stats_queue_sentry == NULL) {
            perror("Analyzer: variable assign failed (cpu_stats_queue_sentry)");
            return NULL;
        }

        cpu_usage_queue_sentry = temp_arg->cpu_usage_queue_sentry;
        if (cpu_usage_queue_sentry == NULL) {
            perror("Analyzer: variable assign failed (cpu_usage_queue_sentry)");
            return NULL;
        }
    }

    CPU cur_cpu_arr[cpu_num];
    CPU prev_cpu_arr[cpu_num];
    double cpu_usage_arr[cpu_num];

    memset(prev_cpu_arr, 0, sizeof(CPU[cpu_num]));

    /* THREAD "MAIN WORK" LOOP */
    while (true) {

        /* Pop from stats queue */
        pcp_sentry_lock(cpu_stats_queue_sentry);
        if (queue_is_empty(cpu_stats_queue)) {
            printf("Analyzer: Czekam na uzupełnienie kolejki przez Reader\n");
            pcp_sentry_wait_for_producer(cpu_stats_queue_sentry);
            // tutaj trzeba watchdoga
        }

        queue_pop(cpu_stats_queue, cur_cpu_arr);
        printf("Analyzer: Pobrałem stats z kolejki\n");

        pcp_sentry_call_producer(cpu_stats_queue_sentry);
        pcp_sentry_unlock(cpu_stats_queue_sentry);

        /* Calculate cpu usage */
        if (calulate_cpus_usage(cpu_num, cur_cpu_arr, prev_cpu_arr, cpu_usage_arr) != 0) {
            printf("error in calculate_cpus_usage!\n");
            break;
        }
        memcpy(prev_cpu_arr, cur_cpu_arr, sizeof(CPU[cpu_num]));

        /* Insert into usage queue */
        pcp_sentry_lock(cpu_usage_queue_sentry);
        if (queue_is_full(cpu_usage_queue)) {
            printf("Analyzer: Czekam na opróżnienie kolejki przez Printer\n");
            pcp_sentry_wait_for_consumer(cpu_usage_queue_sentry);
        }

        if(queue_insert(cpu_usage_queue, cpu_usage_arr));
        printf("Analyzer: Wstawiam usage z kolejki\n");

        puts("Printer cpu usage:");
        print_cpu_usage(cpu_num, cpu_usage_arr);
        printf("cpu_usage_queue size = %zu\n\n", queue_size(cpu_usage_queue));

        pcp_sentry_call_consumer(cpu_usage_queue_sentry);
        pcp_sentry_unlock(cpu_usage_queue_sentry);
    }
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
