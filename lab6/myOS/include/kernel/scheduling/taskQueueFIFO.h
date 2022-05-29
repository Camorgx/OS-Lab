#ifndef TASKQUEUEFIFO_H
#define TASKQUEUEFIFO_H

#include "kernel/scheduling/task.h"

typedef struct taskQueueFIFO {
    TCB* data;
    unsigned head, tail;
    unsigned size;
} taskQueueFIFO;

extern taskQueueFIFO taskQueue;
extern const unsigned initial_size;

extern void display_queue(const taskQueueFIFO* queue);

extern unsigned qpush(taskQueueFIFO* queue, TCB tcb);
extern unsigned qpop(taskQueueFIFO* queue);
extern TCB qfront(const taskQueueFIFO* queue);
extern TCB qback(const taskQueueFIFO* queue);
extern unsigned qlength(const taskQueueFIFO* queue);
extern unsigned qfull(const taskQueueFIFO* queue);
extern unsigned qempty(const taskQueueFIFO* queue);

#endif // !TASKQUEUEFIFO_H