#include "kernel/scheduling/taskQueueFIFO.h"
#include "kernel/scheduling/scheduler.h"
#include "kernel/scheduling/task.h"

#include "kernel/mem/mem.h"

typedef enum scheduler_type {
    FCFS,
    RR,
    PRIORITY
} scheduler_type;

const char* schedule_names[3] = {"FCFS", "RR", "PRIORITY"};

typedef struct scheduler {
    scheduler_type type;
    TCB (*next_tsk)(void);
    void (*enqueue)(TCB tsk);
    void (*dequeue)(void);
    void (*init)(void);
} scheduler;

unsigned long BspContextBase[0x100];
unsigned long *BspContext;
void startMultitask(void) {
    BspContext = BspContextBase + 0x100 - 1;
    TCB firstTsk = qFront(&taskQueue);
    context_switch(&BspContext, firstTsk.stack, firstTsk.tid);
}

void idleTsk(void) {
    while (qEmpty(&taskQueue)) schedule();
}

unsigned long* idle_stack;
unsigned idle_id;

extern void main(void);

void init_tsk_manager(void) {
    task_list_head = (task_list*) kmalloc(sizeof(task_list));
    qInit(&taskQueue);
    idle_id = createTsk(idleTsk);
    idle_stack = task_list_head->next->data.stack;
    unsigned init = createTsk(main);
    tskStart(init);
    startMultitask();
}

void fifo_schedule(void) {
    TCB next = qFront(&taskQueue);
    context_switch(&current_tsk_stack, next.stack, next.tid);
}

void schedule(void) {
    if (qEmpty(&taskQueue))
        context_switch(&current_tsk_stack, idle_stack, idle_id);
    fifo_schedule();
}
