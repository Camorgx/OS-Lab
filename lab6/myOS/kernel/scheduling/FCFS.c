#include "kernel/scheduling/FCFS.h"

taskQueueFIFO queueFIFO;

void init_FCFS() {
    qInit(&queueFIFO);
}

void enqueue_FCFS(TCB x) {
    qPush(&queueFIFO, x);
}

void dequeue_FCFS() {
    qPop(&queueFIFO);
}

TCB* next_tsk_FCFS(void) {
    if (qEmpty(&queueFIFO)) return &idle;
    else return qFront(&queueFIFO);
}

scheduler FCFS_scheduler = (scheduler){.type = FCFS, .init = init_FCFS,
                                       .enqueue = enqueue_FCFS, .dequeue = dequeue_FCFS,
                                       .next_tsk = next_tsk_FCFS, .tick_func = 0};