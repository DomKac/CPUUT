#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <watchdog_unit.h>
#include <signal.h>

typedef struct Watchdog_arguments {

    volatile sig_atomic_t* signal_received;
    Watchdog_unit* wdog_reader;
    Watchdog_unit* wdog_analyzer;
    Watchdog_unit* wdog_printer;
    // Watchdog_unit* wdog_logger;

} Watchdog_arguments;

void* watchdog_func(void* watchdog_args);

#endif
