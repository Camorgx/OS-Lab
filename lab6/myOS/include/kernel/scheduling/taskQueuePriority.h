#ifndef TASK_QUEUE_PRIORITY
#define TASK_QUEUE_PRIORITY

#include "kernel/scheduling/task.h"
#include "kernel/scheduling/taskVector.h"

typedef struct taskQueuePriority {
    int (*greater)(TCB* a, TCB* b);
    taskVector dat;
} taskQueuePriority;

extern void pqInit(taskQueuePriority* queue, int (*greater_func)(TCB* a, TCB* b));
extern void pqPush(taskQueuePriority* queue, TCB x);
extern void pqPop(taskQueuePriority* queue);
extern unsigned pqSize(taskQueuePriority* queue);
extern TCB* pqTop(taskQueuePriority* queue);
extern unsigned pqEmpty(taskQueuePriority* queue);

#endif // TASK_QUEUE_PRIORITY