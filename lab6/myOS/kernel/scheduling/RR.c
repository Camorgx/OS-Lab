#include "kernel/scheduling/RR.h"

taskQueueFIFO queueRR;

void init_RR(void) {
    qInit(&queueRR);
}

void enqueue_RR(TCB x) {
    qPush(&queueRR, x);
}

void dequeue_RR() {
    qPop(&queueRR);
}

TCB* next_tsk_RR(void) {
    if (qEmpty(&queueRR)) return &idle;
    else return qFront(&queueRR);
}

extern unsigned system_ticks;
void tick_func_RR(void) {
    if (qEmpty(&queueRR)) return;
    if (system_ticks % 50 == 0) {
        if (current_tsk_stack) {
            TCB* cur = qFront(&queueRR);
            qPush(&queueRR, *cur);
            qPop(&queueRR);
            schedule();
        }
    }
}

scheduler RR_scheduler = (scheduler) {.type = RR, .init = init_RR, .enqueue = enqueue_RR, .dequeue = dequeue_RR,
                                      .next_tsk = next_tsk_RR, .tick_func = tick_func_RR};