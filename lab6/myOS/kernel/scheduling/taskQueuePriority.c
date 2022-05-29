#include "kernel/scheduling/taskQueuePriority.h"

void swap(TCB* a, TCB* b) {
    TCB t = *a;
    *a = *b;
    *b = t;
}

void swim(taskQueuePriority* queue, unsigned pos) {
    while (pos > 1 && queue->greater(vAt(&queue->dat, pos), vAt(&queue->dat, pos >> 1))) {
        swap(vAt(&queue->dat, pos), vAt(&queue->dat, pos >> 1));
        pos >>= 1;
    }
}

void sink(taskQueuePriority* queue, unsigned pos) {
    while (2 * pos <= pqSize(queue)) {
        unsigned target = (2 * pos < pqSize(queue) &&
                queue->greater(vAt(&queue->dat, 2 * pos + 1), vAt(&queue->dat, 2 * pos)))
                        ? 2 * pos + 1 : 2 * pos;
        if (queue->greater(vAt(&queue->dat, target), vAt(&queue->dat, pos))) {
            swap(vAt(&queue->dat, pos), vAt(&queue->dat, target));
            pos = target;
        }
        else break;
    }
}

void pqPop(taskQueuePriority* queue) {
    swap(vAt(&queue->dat, 1), vAt(&queue->dat, vSize(&queue->dat) - 1));
    vPopBack(&queue->dat);
    sink(queue, 1);
}

void pqPush(taskQueuePriority* queue, TCB x) {
    vPushBack(&queue->dat, x);
    swim(queue, vSize(&queue->dat) - 1);
}

unsigned pqSize(taskQueuePriority* queue) {
    return vSize(&queue->dat) - 1;
}

unsigned pqEmpty(taskQueuePriority* queue) {
    return (vSize(&queue->dat) == 1);
}

TCB* pqTop(taskQueuePriority* queue) {
    return vAt(&queue->dat, 1);
}

void pqInit(taskQueuePriority* queue, int (*greater_func)(TCB* a, TCB* b)) {
    queue->greater = greater_func;
    vInit(&queue->dat);
}