#ifndef LAB5_TASK_H
#define LAB5_TASK_H

extern void CTX_SW(void);
extern void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk,
                           unsigned next_tsk_id);

typedef enum State {
    READY,
    WAITING
} State;

typedef struct tskPara {
    unsigned priority;
    unsigned arrTime;
    unsigned exeTime;
} tskPara;

typedef struct TCB {
    unsigned tid; // 从 1 开始编号，0 被空 TCB 独占
    State state;
    unsigned long* stack;
    unsigned long malloced_pos;
    tskPara params;
} TCB;

typedef struct task_list {
    TCB data;
    struct task_list* next;
} task_list;

extern task_list* task_list_head;

#define NULL_TCB ((TCB){.tid = 0, .state = READY, .stack = 0, \
    .params = (tskPara){0, 0, 0}})

extern unsigned createTsk(void (*tskBody)(void));
extern void destroyTsk(unsigned tskIndex);

extern void stack_init(unsigned long **stk, void (*task)(void));

extern void tskStart(unsigned tskIndex);
extern void tskEnd();

extern unsigned current_tsk_index;
extern unsigned long* current_tsk_stack;

#endif //LAB5_TASK_H
