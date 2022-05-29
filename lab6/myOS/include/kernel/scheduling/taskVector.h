#ifndef TASK_VECTOR
#define TASK_VECTOR

#include "kernel/scheduling/task.h"

typedef struct taskVector {
    TCB* dat;
    unsigned actual_size, array_size;
} taskVector;

extern void vInit(taskVector* vector);
extern unsigned vSize(taskVector* vector);
extern unsigned vEmpty(taskVector* vector);
extern TCB* vAt(taskVector* vector, unsigned id);
extern void vClear(taskVector* vector);
extern void vPushBack(taskVector* vector, TCB x);
extern void vPopBack(taskVector* vector);

#endif // TASK_VECTOR