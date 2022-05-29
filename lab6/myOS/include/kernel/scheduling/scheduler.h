#ifndef LAB5_SCHEDULER_H
#define LAB5_SCHEDULER_H

#include "task.h"

typedef enum scheduler_type {
    FCFS,
    RR,
    PRIORITY
} scheduler_type;

extern const char* schedule_names[3];

typedef struct scheduler {
    scheduler_type type;
    TCB* (*next_tsk)(void);
    void (*enqueue)(TCB);
    void (*dequeue)(void);
    void (*init)(void);
    void (*tick_func)(void);
} scheduler;

extern scheduler system_scheduler;

extern void set_schedule_method(scheduler_type type);
extern void schedule(void);
extern void init_tsk_manager(scheduler_type type);

extern void check_arrive();

#endif //LAB5_SCHEDULER_H
