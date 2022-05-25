#ifndef LAB5_TASK_H
#define LAB5_TASK_H

extern void CTX_SW(void);
extern void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);

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
    tskPara params;
} TCB;

#define NULL_TCB ((TCB){.tid = 0, .state = READY, .stack = 0, \
    .params = (tskPara){0, 0, 0}})

#endif //LAB5_TASK_H
