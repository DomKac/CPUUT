#ifndef PCP_SENTRY_H
#define PCP_SENTRY_H
#include <pthread.h>

#define PCP_SENTRY_INIT \
    { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER }

typedef struct PCP_Sentry PCP_Sentry;

static PCP_Sentry* pcp_sentry_create(void);
static void pcp_sentry_destroy(PCP_Sentry* pcp_sentry);
static void pcp_sentry_lock(PCP_Sentry* pcp_sentry);
static void pcp_sentry_unlock(PCP_Sentry* pcp_sentry);
static void pcp_sentry_call_producer(PCP_Sentry* pcp_sentry);
static void pcp_sentry_call_consumer(PCP_Sentry* pcp_sentry);
static void pcp_sentry_wait_for_producer(PCP_Sentry* pcp_sentry);
static void pcp_sentry_wait_for_consumer(PCP_Sentry *pcp_sentry);

#endif
