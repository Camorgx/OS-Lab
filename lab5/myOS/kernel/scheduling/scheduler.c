#include "kernel/scheduling/taskQueueFIFO.h"
#include "kernel/scheduling/scheduler.h"
#include "kernel/scheduling/task.h"

#include "kernel/mem/mem.h"

extern unsigned long* current_ysk_stack;
void fifo_schedule(void) {
    context_switch(&current_ysk_stack, qfront(&taskQueue).stack);
}

extern task_list* task_list_head;
extern const unsigned long initial_size;
extern taskQueueFIFO taskQueue;
void init_tsk_manager(void) {
    task_list_head = (task_list*) kmalloc(sizeof(task_list));
    taskQueue.data = (TCB*) kmalloc(initial_size * sizeof(TCB));
    taskQueue.size = initial_size;
}