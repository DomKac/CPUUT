#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdlib.h>

#define EQUEUE_EMPTY -1
#define EQUEUE_FULL -2
#define EQUEUE_NULLARG -3

typedef struct Queue Queue;

Queue *queue_new(size_t max_size, size_t elem_size);
void queue_delete(Queue* q);

int queue_insert(Queue* restrict q, const void* const restrict elem);
int queue_pop(Queue* restrict q, void* const restrict elem);

bool queue_is_empty(const Queue* q);
bool queue_is_full(const Queue* q);

#endif
