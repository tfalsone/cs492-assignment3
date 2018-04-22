#include "queue_struct.h"

int q_init(queue *q) {
    q->head = NULL;
    q->size = 0;

    return 0;
}

int q_push(queue *q, void *obj, int priority) {
    q_node* node; // new node to be added to queue
    q_node* current; // iterates through queue
    q_node* tail; // marks end of queue

    if (q == NULL) { return -1; }
    if (obj == NULL) { return -2; }

    node = (q_node*)malloc(sizeof(q_node));
    node->object = obj;
    node->count = priority;
    current = q->head;
    if (q->head != NULL) {
        tail = current->prev;
    }

    if (current == NULL) {
        // empty queue
        node->prev = node;
        node->next = node;
        q->head = node;
    } else if (tail == current) {
        // queue has one item
        if (node->count < current->count) {
            // if new node has lower priority, new node goes to front of queue
            q->head = node;
        }
        node->next = current;
        node->prev = current;
        current->next = node;
        current->prev = node;
    } else {
        // queue has multiple items
        while (node->count >= current->count && tail != current) {
            current = current->next;
        }

        if (node->count < current->count) {
            // node has lower priority that current, goes before current
            node->next = current;
            node->prev = current->prev;
            node->prev->next = node;
            node->next->prev = node;
        } else {
            // node goes after current
            node->next = current->next;
            node->prev = current;
            node->prev->next = node;
            node->next->prev = node;
        }
    }
    q->size++;
    return 0;
}

void* q_pop(queue *q) {
    void* object;
    q_node* node;

    if (q == NULL || q->head == NULL) {
        return NULL;
    }

    node = q->head;
    object = node->object;

    if (q->size-- == 1) { //should this be 0?
        q->head = NULL;
    } else {
        q->head = node->next;
        q->head->prev = node->prev;
        node->prev->next = q->head;
    }
    return object;
}

int q_remove(queue *q, void *obj) {
    q_node *node;
    q_node *current;

    if (q == NULL) { return -1; }
    if (obj == NULL) { return -2; }

    current = q->head;
    node = q->head;

    if (q->size-- == 1) {
        q->head = NULL;
    } else {
        if (q->head->object = obj) {
            // q pop
            q_pop(q);
        } else {
            while (node->object != obj) {
                node = node->next;
            }
            current = node->next;
            current->prev = node->prev;
            current->prev->next = current;
        }
    }
    return 0;
}

int q_update(queue *q, void *obj) {
    q_node *node;
    node = q->head;

    if (q == NULL) { return -1; }
    if (obj == NULL) { return -2; }

    while (node->object != obj) {
        node = node->next;
    }
    node->count++;
    q_remove(q, obj);
    q_push(q, obj, node->count);
    return node->count;
}

void* q_find(queue *q, void *obj) {
    q_node *current;
    current = q->head;
    int i = q->size;

    if (q == NULL) { return -1; }
    if (obj == NULL) { return -2; }

    while (current->object != obj && i-- > 0) {
        current = current->next;
    }

    if (current->object == obj)
        return current;
    else
        return NULL;
}