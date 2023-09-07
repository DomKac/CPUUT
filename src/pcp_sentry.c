#include <pcp_sentry.h>

struct PCP_Sentry {

    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
};

static PCP_Sentry* pcp_sentry_create(void) {

    PCP_Sentry *pcp_sentry = malloc(sizeof(PCP_Sentry));
    if (pcp_sentry == NULL) {
        return NULL;
    }

    *pcp_sentry = (PCP_Sentry){
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .can_consume = PTHREAD_COND_INITIALIZER,
        .can_produce = PTHREAD_COND_INITIALIZER,
    };

    return pcp_sentry;
}

static void pcp_sentry_destroy(PCP_Sentry *pcp_sentry) {

    pthread_cond_destroy(&pcp_sentry->can_consume);
    pthread_cond_destroy(&pcp_sentry->can_produce);
    pthread_mutex_destroy(&pcp_sentry->mutex);

    free(pcp_sentry);
}

static void pcp_sentry_lock(PCP_Sentry *pcp_sentry) {
    pthread_mutex_lock(&pcp_sentry->mutex);
}

static void pcp_sentry_unlock(PCP_Sentry *pcp_sentry) {
    pthread_mutex_unlock(&pcp_sentry->mutex);
}

static void pcp_sentry_call_producer(PCP_Sentry *pcp_sentry) {
    pthread_cond_signal(&pcp_sentry->can_produce);
}

static void pcp_sentry_call_consumer(PCP_Sentry *pcp_sentry) {
    pthread_cond_signal(&pcp_sentry->can_consume);
}

static void pcp_sentry_wait_for_producer(PCP_Sentry *pcp_sentry) {
    pthread_cond_wait(&pcp_sentry->can_produce, &pcp_sentry->mutex);
}

static void pcp_sentry_wait_for_consumer(PCP_Sentry *pcp_sentry) {
    pthread_cond_wait(&pcp_sentry->can_produce, &pcp_sentry->mutex);
}
