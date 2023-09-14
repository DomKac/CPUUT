#include "../inc/watchdog.h"
#include <time.h>

void* watchdog_func(void* watchdog_args) {

    /* INITIALIZATIONS & CONTRACTS */
    if (watchdog_args == NULL) {
        perror("Watchdog: received NULL argument");
        return NULL;
    }

    volatile sig_atomic_t* signal_received = NULL;
    Watchdog_unit *wdog_reader = NULL;
    Watchdog_unit *wdog_analyzer = NULL;
    Watchdog_unit *wdog_printer = NULL;
    // Watchdog_unit *wdog_logger = NULL; /* TODO */

    {
        Watchdog_arguments *temp_arg = watchdog_args;

        signal_received = temp_arg->signal_received;
        if (signal_received == NULL) {
            perror("Watchdog: variable assign failed (signal_received)");
            return NULL;
        }

        wdog_reader = temp_arg->wdog_reader;
        if (wdog_reader == NULL) {
            perror("Watchdog: variable assign failed (wdog_reader)");
            return NULL;
        }

        wdog_analyzer = temp_arg->wdog_analyzer;
        if (wdog_analyzer == NULL) {
            perror("Watchdog: variable assign failed (wdog_analyzer)");
            return NULL;
        }

        wdog_printer = temp_arg->wdog_printer;
        if (wdog_printer == NULL) {
            perror("Watchdog: variable assign failed (wdog_printer)");
            return NULL;
        }

        // if (wdog_logger == NULL) {
        //     perror("Watchdog: variable assign failed (wdog_logger)");
        //     return NULL;
        // }
    }

    const struct timespec sleep_time = {.tv_nsec = 0, .tv_sec = 2};

    /* THREAD "MAIN WORK" LOOP */
    while (!*signal_received) {

        /* Reader */
        watchdog_unit_mutex_lock(wdog_reader);
        if (watchdog_unit_get_state(wdog_reader) == WU_GREEN_STATE) {
            watchdog_unit_change_state(wdog_reader, WU_RED_STATE);
            watchdog_unit_mutex_unlock(wdog_reader);
            printf("Watchdog: READER working good!\n");
        } else {
            watchdog_unit_mutex_unlock(wdog_reader);
            printf("Watchdog: READER something went wrong!\n");
            raise(SIGINT);
            break;
        }

        /* Analyzer */
        watchdog_unit_mutex_lock(wdog_analyzer);
        if (watchdog_unit_get_state(wdog_analyzer) == WU_GREEN_STATE) {
            watchdog_unit_change_state(wdog_analyzer, WU_RED_STATE);
            watchdog_unit_mutex_unlock(wdog_analyzer);
            printf("Watchdog: ANALYZER working good!\n");
        } 
        else {
            watchdog_unit_mutex_unlock(wdog_analyzer);
            printf("Watchdog: ANALYZER something went wrong!\n");
            raise(SIGINT);
            break;
        }

        /* Printer */
        watchdog_unit_mutex_lock(wdog_printer);
        if (watchdog_unit_get_state(wdog_printer) == WU_GREEN_STATE) {
            watchdog_unit_change_state(wdog_printer, WU_RED_STATE);
            watchdog_unit_mutex_unlock(wdog_printer);
            printf("Watchdog: PRINTER working good!\n");
        }
        else {
            watchdog_unit_mutex_unlock(wdog_printer);
            printf("Watchdog: PRINTER something went wrong!\n");
            raise(SIGINT);
            break;
        }

        nanosleep(&sleep_time, NULL);
    }

    /* Clear section */

    return NULL;

}
