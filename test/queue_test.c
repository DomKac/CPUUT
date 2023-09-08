#include "../inc/queue.h"
#include <assert.h>
#include <memory.h>

void queue_main_test(void);

static void queue_new_test(void);
static void queue_delete_test(void);

static void queue_is_full_on_empty_test(void);
static void queue_is_empty_on_empty_test(void);

static void queue_insert_single_test(void);
static void queue_insert_multiple_test(void);
static void queue_insert_on_full_test(void);

static void queue_pop_single_test(void);
static void queue_pop_multiple_test(void);
static void queue_pop_on_empty_test(void);

static void queue_array_test(void);

static void queue_size_test(void);

static void queue_new_test(void) {
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);
        assert(q != NULL);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 0;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);
        assert(q == NULL);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 0;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);
        assert(q == NULL);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 10;
        register const size_t elem_size = 0;

        Queue* q = queue_new(n_elem, elem_size);
        assert(q == NULL);

        queue_delete(q);
    }

    {
        register const size_t n_elem = 0;
        register const size_t elem_size = 0;

        Queue* q = queue_new(n_elem, elem_size);
        assert(q == NULL);

        queue_delete(q);
    }
}

static void queue_delete_test(void) {

    register const size_t n_elem = 0;
    register const size_t elem_size = 0;

    Queue* q = queue_new(n_elem, elem_size);
    queue_delete(q);
    assert(q == NULL);
}

static void queue_is_empty_on_empty_test(void) {
    {
        register const size_t n_elem = 0;
        register const size_t elem_size = 0;

        Queue* q = queue_new(n_elem, elem_size);

        assert(queue_is_empty(q));

        queue_delete(q);
    }
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = 0;

        Queue* q = queue_new(n_elem, elem_size);

        assert(queue_is_empty(q));

        queue_delete(q);
    }
    {
        register const size_t n_elem = 0;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);

        assert(queue_is_empty(q));

        queue_delete(q);
    }
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);

        assert(queue_is_empty(q));

        queue_delete(q);
    }
}

static void queue_is_full_on_empty_test(void) {
    {
        register const size_t n_elem = 0;
        register const size_t elem_size = 0;

        Queue* q = queue_new(n_elem, elem_size);

        assert(!queue_is_full(q));

        queue_delete(q);
    }
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = 0;

        Queue* q = queue_new(n_elem, elem_size);

        assert(!queue_is_full(q));

        queue_delete(q);
    }
    {
        register const size_t n_elem = 0;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);

        assert(!queue_is_full(q));

        queue_delete(q);
    }
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = sizeof(int);

        Queue* q = queue_new(n_elem, elem_size);

        assert(!queue_is_full(q));

        queue_delete(q);
    }
}

static void queue_insert_single_test(void) {

    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(double);

    Queue* q = queue_new(n_elem, elem_size);

    assert(sizeof(double) == 8);
    assert(queue_is_empty(q));

    queue_insert(q, &(double){7.49});

    assert(!queue_is_empty(q));

    queue_delete(q);
}

static void queue_insert_multiple_test(void) {

    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(size_t);

    Queue* q = queue_new(n_elem, elem_size);

    for (size_t i = 0; i < n_elem; i++) {
        queue_insert(q, &i);
        assert(!queue_is_empty(q));
        assert(queue_is_full(q) == (i == n_elem - 1));
        assert(!queue_is_full(q) == (i < n_elem - 1));
    }

    queue_delete(q);
}

static void queue_insert_on_full_test(void) {
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(unsigned long long);

    Queue* q = queue_new(n_elem, elem_size);

    for (size_t i = 0; i < n_elem; i++) {
        queue_insert(q, &i);
        assert(!queue_is_empty(q));
        assert(queue_is_full(q) == (i == n_elem - 1));
        assert(!queue_is_full(q) == (i < n_elem - 1));
    }

    assert(queue_insert(q, &(unsigned long long){123}) == EQUEUE_FULL);

    queue_delete(q);
}

static void queue_pop_single_test(void) {
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);
    const int ins_val = 7;
    int ret_val = 0;

    Queue* q = queue_new(n_elem, elem_size);

    queue_insert(q, &ins_val);
    queue_pop(q, &ret_val);

    assert(ins_val == ret_val);

    queue_delete(q);
}

static void queue_pop_multiple_test(void) {

    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int);
    int val = 0;

    Queue* q = queue_new(n_elem, elem_size);

    for (size_t i = 0; i < n_elem; i++) {
        queue_insert(q, &(int){(int)i});
        assert(!queue_is_empty(q));
        assert(queue_is_full(q) == (i == n_elem - 1));
        assert(!queue_is_full(q) == (i < n_elem - 1));
    }

    for (size_t i = 0; i < n_elem; i++) {
        queue_pop(q, &val);
        assert(val == (int)i);
        assert(queue_is_empty(q) == (i == n_elem - 1));
    }

    /* Fill the queue second time */
    for (size_t i = 0; i < n_elem; i++) {
        queue_insert(q, &(int){(int)i});
        assert(!queue_is_empty(q));
        assert(queue_is_full(q) == (i == n_elem - 1));
        assert(!queue_is_full(q) == (i < n_elem - 1));
    }

    queue_delete(q);
}

static void queue_pop_on_empty_test(void) {
    {
        register const size_t n_elem = 10;
        register const size_t elem_size = sizeof(int);
        int val = 0;

        Queue* q = queue_new(n_elem, elem_size);

        for (size_t i = 0; i < n_elem; i++) {
            queue_insert(q, &(int){(int)i});
            assert(!queue_is_empty(q));
            assert(queue_is_full(q) == (i == n_elem - 1));
            assert(!queue_is_full(q) == (i < n_elem - 1));
        }

        for (size_t i = 0; i < n_elem; i++) {
            queue_pop(q, &val);
            assert(val == (int)i);
            assert(queue_is_empty(q) == (i == n_elem - 1));
        }

        /* Pop on empty queue */
        for (size_t i = 0; i < n_elem; i++) {
            assert(queue_pop(q, &val) == EQUEUE_EMPTY);
            assert(val == 0);
        }

        queue_delete(q);
    }

    {
        register const size_t n_elem = 10;
        register const size_t elem_size = sizeof(double);
        const double ins_val = 1.34;
        double ret_val = 0.0;

        Queue* q = queue_new(n_elem, elem_size);

        queue_insert(q, &ins_val);
        queue_pop(q, &ret_val);

        /* pop on empty queue */
        register const int err = queue_pop(q, &ret_val);
        assert(err == EQUEUE_EMPTY);

        queue_delete(q);
    }
}

static void queue_array_test(void) {
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(int[3]);
    int input[3] = {2, 3, 5};
    int output[3];

    Queue *q = queue_new(n_elem, elem_size);

    assert(q != NULL);

    queue_insert(q, input);
    assert(!queue_is_empty(q));
    queue_pop(q, output);

    for (size_t i = 0; i < 3; i++) {
        assert(input[i] == output[i]);
    }

    queue_delete(q);
}

static void queue_size_test(void) {
    register const size_t n_elem = 10;
    register const size_t elem_size = sizeof(unsigned long long);

    Queue *q = queue_new(n_elem, elem_size);

    for (size_t i = 0; i < n_elem; i++) {
        queue_insert(q, &i);
        assert(queue_size(q) == i + 1);
    }

    queue_delete(q);
}

void queue_main_test(void) {
    queue_new_test();
    queue_delete_test();

    queue_is_empty_on_empty_test();
    queue_is_full_on_empty_test();

    queue_insert_single_test();
    queue_insert_multiple_test();
    queue_insert_on_full_test();

    queue_pop_single_test();
    queue_pop_multiple_test();
    queue_pop_on_empty_test();

    queue_array_test();

    queue_size_test();
}
