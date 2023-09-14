#ifndef WATCHDOG_UNIT_H
#define WATCHDOG_UNIT_H

#include <pthread.h>

typedef enum WU_state {
    WU_GREEN_STATE, /* thread send signal that everything works fine */
    WU_RED_STATE,
    WU_ERROR,
} WU_state;

typedef struct Watchdog_unit {

    pthread_mutex_t state_mutex;
    WU_state state;
    char pad[4];

} Watchdog_unit;

int watchdog_unit_init(Watchdog_unit* wdog_unit);
// Watchdog_unit* watchdog_unit_init(void);
void watchdog_unit_destroy(Watchdog_unit* wdog_unit);
int watchdog_unit_change_state(Watchdog_unit* wdog_unit, WU_state state);
WU_state watchdog_unit_get_state(Watchdog_unit* wdog_unit);
void watchdog_unit_mutex_lock(Watchdog_unit* wdog_unit);
void watchdog_unit_mutex_unlock(Watchdog_unit* wdog_unit);

#endif
