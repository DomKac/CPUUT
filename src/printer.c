#include <printer.h>
#include <watchdog_unit.h>
#include <stdio.h>

static int print_cpu_usage(register const size_t cpu_num, const double cpu_usage_arr[const cpu_num]);

void* printer_func(void* printer_args) {
    /* INITIALIZATIONS & CONTRACTS */
    if (printer_args == NULL) {
        perror("Printer: received NULL argument");
        return NULL;
    }

    register size_t cpu_num = 0;
    Queue* cpu_usage_queue = NULL;
    PCP_Sentry* cpu_usage_queue_sentry = NULL;
    Watchdog_unit* wdog_printer = NULL;
    volatile sig_atomic_t *signal_received = NULL;

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

        cpu_usage_queue_sentry = temp_arg->cpu_usage_queue_sentry;
        if (cpu_usage_queue_sentry == NULL) {
            perror("Printer: variable assign failed (cpu_usage_queue_sentry)");
            return NULL;
        }

        wdog_printer = temp_arg->wdog_printer;
        if (wdog_printer == NULL) {
            perror("Printer: variable assign failed (wdog_printer)");
            return NULL;
        }

        signal_received = temp_arg->signal_received;
        if (signal_received == NULL) {
            perror("Printer: variable assign failed (signal_received)");
            return NULL;
        }
    }

    double cpu_usage_arr[cpu_num];

    /* THREAD "MAIN WORK" LOOP */
    while (!*signal_received) {
        pcp_sentry_lock(cpu_usage_queue_sentry);
        if (queue_is_empty(cpu_usage_queue)) {
            printf("Printer: Czekam na uzupełnienie kolejki przez Analyzer\n");
            pcp_sentry_wait_for_producer(cpu_usage_queue_sentry);
        }

        if (*signal_received) {
            pcp_sentry_unlock(cpu_usage_queue_sentry);
            puts("Dostałem sygnał, kończę!");
            break;
        }

        queue_pop(cpu_usage_queue, cpu_usage_arr);

        pcp_sentry_call_producer(cpu_usage_queue_sentry);
        pcp_sentry_unlock(cpu_usage_queue_sentry);
        printf("Printer: signal received = %d\n", *signal_received);

        watchdog_unit_mutex_lock(wdog_printer);
        watchdog_unit_change_state(wdog_printer, WU_GREEN_STATE);
        watchdog_unit_mutex_unlock(wdog_printer);
    }

    printf("Printer after loop: signal received = %d\n", *signal_received);

    puts("Printer: Zrobiłem co miałem zrobić!");
    return NULL;
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
