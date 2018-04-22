#ifndef _queuestruct_
#define _queuestruct_

    #ifndef NULL
        #define NULL ((void*) 0)
    #endif

    #include <errno.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>

    typedef struct q_node {
        int* object;
        struct q_node *prev;
        struct q_node *next;
        int count;
    } q_node;

    typedef struct queue_struct {
        q_node *head;
        int size;
    } queue;

    int q_init(queue *);
    int q_push(queue *, void *, int);
    int q_update(queue *, void *);
    void* q_pop(queue *);
    int q_remove(queue *, void *);
    void* q_find(queue *, void *);

#endif