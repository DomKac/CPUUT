#ifndef PCP_SENTRY_H
#define PCP_SENTRY_H

#include <pthread.h>
// #define PCP_SENTRY_INITIALIZER { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER }

typedef struct PCP_Sentry {

    pthread_mutex_t mutex;
    pthread_cond_t can_produce;
    pthread_cond_t can_consume;
} PCP_Sentry;

int pcp_sentry_init(PCP_Sentry* pcp_sentry);
int pcp_sentry_destroy(PCP_Sentry* pcp_sentry);
int pcp_sentry_lock(PCP_Sentry* pcp_sentry);
int pcp_sentry_unlock(PCP_Sentry* pcp_sentry);
int pcp_sentry_call_producer(PCP_Sentry* pcp_sentry);
int pcp_sentry_call_consumer(PCP_Sentry* pcp_sentry);
int pcp_sentry_wait_for_producer(PCP_Sentry* pcp_sentry);
int pcp_sentry_wait_for_consumer(PCP_Sentry* pcp_sentry);

#endif
