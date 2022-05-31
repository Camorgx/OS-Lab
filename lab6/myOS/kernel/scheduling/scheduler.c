#include "kernel/scheduling/scheduler.h"

#include "kernel/mem/mem.h"

#include "kernel/scheduling/FCFS.h"
#include "kernel/scheduling/RR.h"
#include "kernel/scheduling/Priority.h"

#include "lib/libio.h"

#include "i386/tick.h"

const char* schedule_names[3] = {"FCFS", "RR", "PRIORITY"};

scheduler system_scheduler;

unsigned long BspContextBase[0x100];
unsigned long *BspContext;
void startMultitask(void) {
    BspContext = BspContextBase + 0x100 - 1;
    TCB* firstTsk = system_scheduler.next_tsk();
    context_switch(&BspContext, firstTsk->stack, firstTsk->tid);
}

TCB init;
TCB idle;

void idleTsk(void) {
    while (system_scheduler.next_tsk()->tid == idle.tid);
    schedule();
}

extern void main(void);

unsigned long* initial_idle_stack;

void init_tsk_manager(scheduler_type type) {
    set_schedule_method(type);
    task_list_head = (task_list*) kmalloc(sizeof(task_list));
    *task_list_head = (task_list) {.data = NULL_TCB, .next = 0};
    system_scheduler.init();
    createTsk(idleTsk, 0, 0);
    createTsk(main, 0, 0);
    tskStart(init.tid);
    startMultitask();
}

void schedule(void) {
    TCB* next = system_scheduler.next_tsk();
    if (next->tid == idle.tid) {
        *(next->stack + 9) = (unsigned long)idleTsk;
        context_switch(&idle.stack, next->stack, next->tid);
    }
    else context_switch(&current_tsk_stack, next->stack, next->tid);
}

void set_schedule_method(scheduler_type type) {
    switch (type) {
        case FCFS: system_scheduler = FCFS_scheduler; break;
        case RR: system_scheduler = RR_scheduler; break;
        case PRIORITY: system_scheduler = Priority_scheduler; break;
        default: system_scheduler = FCFS_scheduler;
    }
    printk(0x7, "Schedule type set to %s.\n", schedule_names[type]);
}

unsigned meet_arrival = 0;
void check_arrive() {
    meet_arrival = 0;
    for (task_list* p = task_list_head->next; p; p = p->next) {
        if (p->data.params.arrTime == system_ticks) {
            tskStart(p->data.tid);
            meet_arrival = 1;
        }
        else if (p->data.params.arrTime > system_ticks) break;
    }
}
