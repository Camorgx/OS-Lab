#include "kernel/scheduling/taskQueuePriority.h"
#include "kernel/scheduling/Priority.h"

taskQueuePriority queuePriority;

int cmp_priority(TCB* a, TCB* b) {
    return (a->params.priority > b->params.priority);
}

void init_Priority() {
    pqInit(&queuePriority, cmp_priority);
}

void enqueue_Priority(TCB x) {
    pqPush(&queuePriority, x);
}

void dequeue_Priority() {
    pqPop(&queuePriority);
}

TCB* next_tsk_Priority(void) {
    if (pqEmpty(&queuePriority)) return &idle;
    else return pqTop(&queuePriority);
}

scheduler Priority_scheduler = (scheduler) {.next_tsk = next_tsk_Priority, .dequeue = dequeue_Priority,
                                            .enqueue = enqueue_Priority, .init = init_Priority,
                                            .type = PRIORITY, .tick_func = 0};
