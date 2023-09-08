#include <pcp_sentry.h>
#include <pthread.h>

typedef enum PCP_Status {
    PCP_SUCCES,
    EPCP_NULL_ARG,
    EPCP_MUTEX_FAILURE,
    EPCP_CONS_FAILURE,
    EPCP_PROD_FAILURE,
} PCP_Status;

// struct PCP_Sentry {

//     pthread_mutex_t mutex;
//     pthread_cond_t can_produce;
//     pthread_cond_t can_consume;
// };

int pcp_sentry_init(PCP_Sentry* const pcp_sentry) {

    if (pcp_sentry == NULL) {
        return EPCP_NULL_ARG;
    }

    if (pthread_mutex_init(&pcp_sentry->mutex, NULL) != 0) {
        return EPCP_MUTEX_FAILURE;
    }

    if (pthread_cond_init(&pcp_sentry->can_consume, NULL) != 0) {
        pthread_mutex_destroy(&pcp_sentry->mutex);
        return EPCP_CONS_FAILURE;
    }

    if (pthread_cond_init(&pcp_sentry->can_produce, NULL) != 0) {
        pthread_mutex_destroy(&pcp_sentry->mutex);
        pthread_cond_destroy(&pcp_sentry->can_consume);
        return EPCP_PROD_FAILURE;
    }

    // *pcp_sentry = (PCP_Sentry){
    //     .mutex = PTHREAD_MUTEX_INITIALIZER,
    //     .can_consume = PTHREAD_COND_INITIALIZER,
    //     .can_produce = PTHREAD_COND_INITIALIZER,
    // };

    return PCP_SUCCES;
}

int pcp_sentry_destroy(PCP_Sentry* const pcp_sentry) {

    if (pcp_sentry == NULL) {
        return EPCP_NULL_ARG;
    }

    if (pthread_mutex_destroy(&pcp_sentry->mutex) != 0) {
        return EPCP_MUTEX_FAILURE;
    }

    if (pthread_cond_destroy(&pcp_sentry->can_consume) != 0) {
        return EPCP_CONS_FAILURE;
    }

    if (pthread_cond_destroy(&pcp_sentry->can_produce) != 0) {
        return EPCP_PROD_FAILURE;
    }

    return PCP_SUCCES;
}

int pcp_sentry_lock(PCP_Sentry* const pcp_sentry) {
    return pthread_mutex_lock(&pcp_sentry->mutex);
}

int pcp_sentry_unlock(PCP_Sentry* const pcp_sentry) {
    return pthread_mutex_unlock(&pcp_sentry->mutex);
}

int pcp_sentry_call_producer(PCP_Sentry* const pcp_sentry) {
    return pthread_cond_signal(&pcp_sentry->can_produce);
}

int pcp_sentry_call_consumer(PCP_Sentry* const pcp_sentry) {
    return pthread_cond_signal(&pcp_sentry->can_consume);
}

int pcp_sentry_wait_for_producer(PCP_Sentry* const pcp_sentry) {
    return pthread_cond_wait(&pcp_sentry->can_consume, &pcp_sentry->mutex);
}

int pcp_sentry_wait_for_consumer(PCP_Sentry* const pcp_sentry) {
    return pthread_cond_wait(&pcp_sentry->can_produce, &pcp_sentry->mutex);
}

// int pcp_sentry_timedwait_for_consumer(PCP_Sentry *const pcp_sentry) {
//     return pthread_cond_timedwait(&pcp_sentry->can_consume, &pcp_sentry->mutex);
// }
