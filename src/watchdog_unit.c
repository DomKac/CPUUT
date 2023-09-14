#include <watchdog_unit.h>
#include <memory.h>

int watchdog_unit_init(Watchdog_unit* wdog_unit) {

        if (wdog_unit == NULL)
            return -1;

        if (pthread_mutex_init(&wdog_unit->state_mutex, NULL) != 0)
            return -1;

        wdog_unit->state = WU_GREEN_STATE;

        return 0;
}

// Watchdog_unit* watchdog_unit_init(void) {

//     Watchdog_unit *const wdog_unit = malloc(sizeof(Watchdog_unit));

//     if (wdog_unit == NULL)
//         return NULL;
    
//     if (pthread_mutex_init(&wdog_unit->state_mutex, NULL) != 0)
//         return NULL;

//     wdog_unit->state = WU_RED;

//     return wdog_unit;
// }

void watchdog_unit_destroy(Watchdog_unit *wdog_unit) {

    if (wdog_unit == NULL)
        return;

    pthread_mutex_destroy(&wdog_unit->state_mutex);
}

// void watchdog_unit_destroy(Watchdog_unit* wdog_unit) {

//     if (wdog_unit == NULL)
//         return;

//     pthread_mutex_destroy(&wdog_unit->state_mutex);

//     free(wdog_unit);
// }



int watchdog_unit_change_state(Watchdog_unit* wdog_unit, WU_state state) {
    
    if (wdog_unit == NULL) {
        return -1;
    }

    if (!(state == WU_GREEN_STATE || state == WU_RED_STATE)) {
        return -1;
    }

    // pthread_mutex_lock(&wdog_unit->state_mutex);
    wdog_unit->state = state;
    // pthread_mutex_unlock(&wdog_unit->state_mutex);

    return 0;
}

/* pytanie czy używać mutexów do czytania w tym przypadku */
WU_state watchdog_unit_get_state(Watchdog_unit* wdog_unit) {

    if (wdog_unit == NULL) {
        return WU_ERROR;
    }

    // pthread_mutex_lock(&wdog_unit->state_mutex);
    register const WU_state state = wdog_unit->state;
    // pthread_mutex_unlock(&wdog_unit->state_mutex);

    return state;
}

void watchdog_unit_mutex_lock(Watchdog_unit* wdog_unit) {
    if (wdog_unit == NULL)
        return;
    pthread_mutex_lock(&wdog_unit->state_mutex);
}

void watchdog_unit_mutex_unlock(Watchdog_unit* wdog_unit) {
    if (wdog_unit == NULL)
        return;
    pthread_mutex_unlock(&wdog_unit->state_mutex);
}
