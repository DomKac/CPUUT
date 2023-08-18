#include "../inc/queue.h"
#include <memory.h>
#include <stdint.h>

struct Queue {
    size_t n_elem;
    size_t cur_size;
    size_t elem_size;
    size_t head;
    size_t tail;
    uint8_t buf[]; /* FAM */
};

Queue *queue_new(const size_t n_len, const size_t elem_size) {

    if (n_len == 0)
        return NULL;
    if (elem_size == 0)
        return NULL;

    Queue *const q = calloc(1, sizeof(Queue) + elem_size * n_len);

    if (q == NULL)
        return NULL;

    *q = (Queue){
        .n_elem = n_len,
        .elem_size = elem_size};

    return q;
}

void queue_delete(Queue *q) {
    if (q != NULL)
        free(q);
}

int queue_insert(Queue *restrict q, const void *const restrict elem) {

    if (q == NULL)
        return EQUEUE_NULLARG;

    if (elem == NULL)
        return EQUEUE_NULLARG;

    if (queue_is_full(q))
        return EQUEUE_FULL;

    memcpy(q->buf + q->head * q->elem_size, elem, q->elem_size);

    ++q->head;
    /* Doszliśmy do końca buffera */
    if (q->head >= q->n_elem)
        q->head = 0;

    ++q->cur_size;

    return 0;
}

int queue_pop(Queue *restrict q, void *const restrict elem) {
    
    if (q == NULL)
        return EQUEUE_NULLARG;

    if (elem == NULL)
        return EQUEUE_NULLARG;

    if (queue_is_empty(q)) {
        memset(elem, 0, q->elem_size); /* ADDED BUT IS IT GOOD ? */
        return EQUEUE_EMPTY;
    }

    memcpy(elem, q->buf + q->tail * q->elem_size, q->elem_size);
    /* clear memory */
    memset(q->buf + q->tail * q->elem_size, 0, q->elem_size);

    ++q->tail;
    if (q->tail >= q->n_elem)
        q->tail = 0;

    --q->cur_size;

    return 0;
}

bool queue_is_empty(const Queue* q) {
    if (q == NULL)
        return true;
    return q->cur_size == 0;
}

bool queue_is_full(const Queue* q) {
    if (q == NULL)
        return false;
    return q->n_elem == q->cur_size;
}
