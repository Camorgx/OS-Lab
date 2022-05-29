#include "kernel/scheduling/scheduler.h"

#include "kernel/mem/mem.h"

#include "kernel/scheduling/FCFS.h"
#include "kernel/scheduling/RR.h"
#include "kernel/scheduling/Priority.h"

const char* schedule_names[3] = {"FCFS", "RR", "PRIORITY"};

scheduler system_scheduler;

unsigned long BspContextBase[0x100];
unsigned long *BspContext;
void startMultitask(void) {
    BspContext = BspContextBase + 0x100 - 1;
    TCB* firstTsk = system_scheduler.next_tsk();
    context_switch(&BspContext, firstTsk->stack, firstTsk->tid);
}

TCB init, idle;

void idleTsk(void) {
    while (system_scheduler.next_tsk()->tid == idle.tid) schedule();
}

extern void main(void);

void init_tsk_manager(scheduler_type type) {
    set_schedule_method(type);
    task_list_head = (task_list*) kmalloc(sizeof(task_list));
    createTsk(idleTsk);
    idle = task_list_head->next->data;
    createTsk(main);
    init = task_list_head->next->data;
    system_scheduler.init();
    tskStart(init.tid);
    startMultitask();
}

void schedule(void) {
    TCB next = *(system_scheduler.next_tsk());
    context_switch(&current_tsk_stack, next.stack, next.tid);
}

void set_schedule_method(scheduler_type type) {
    switch (type) {
        case FCFS: system_scheduler = FCFS_scheduler; break;
        case RR: system_scheduler = RR_scheduler; break;
        case PRIORITY: system_scheduler = Priority_scheduler; break;
        default: system_scheduler = FCFS_scheduler;
    }
}
