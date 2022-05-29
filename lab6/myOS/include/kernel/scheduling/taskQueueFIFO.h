#ifndef TASK_QUEUE_FIFO_H
#define TASK_QUEUE_FIFO_H

#include "kernel/scheduling/task.h"

typedef struct taskQueueFIFO {
    TCB* data;
    unsigned head, tail;
    unsigned size;
} taskQueueFIFO;

extern taskQueueFIFO queueFIFO;
extern const unsigned initial_size;

extern void displayQueue(const taskQueueFIFO* queue);

extern unsigned qInit(taskQueueFIFO* queue);
extern unsigned qPush(taskQueueFIFO* queue, TCB tcb);
extern unsigned qPop(taskQueueFIFO* queue);
extern TCB* qFront(const taskQueueFIFO* queue);
extern TCB* qBack(const taskQueueFIFO* queue);
extern unsigned qLength(const taskQueueFIFO* queue);
extern unsigned qFull(const taskQueueFIFO* queue);
extern unsigned qEmpty(const taskQueueFIFO* queue);

#endif // !TASK_QUEUE_FIFO_H