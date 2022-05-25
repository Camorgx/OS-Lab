#include "kernel/scheduling/task.h"
#include "kernel/mem/mem.h"

unsigned long **prevTSK_StackPtrAddr;
unsigned long *nextTSK_StackPtr;

void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk) {
    prevTSK_StackPtrAddr = prevTskStkAddr;
    nextTSK_StackPtr = nextTskStk;
    CTX_SW();
}

void stack_init(unsigned long **stk, void (*task)(void)) {
    *(*stk)-- = (unsigned long)0x08; // CS 
    *(*stk)-- = (unsigned long)tskEnd;
    *(*stk)-- = (unsigned long)task; // eip
    // pushf
    *(*stk)-- = (unsigned long)0x0202; // flag registers
    // pusha
    *(*stk)-- = (unsigned long)0xAAAAAAAA; // eax
    *(*stk)-- = (unsigned long)0xCCCCCCCC; // ecx
    *(*stk)-- = (unsigned long)0xDDDDDDDD; // edx
    *(*stk)-- = (unsigned long)0xBBBBBBBB; // ebx
    *(*stk)-- = (unsigned long)0x44444444; // esp
    *(*stk)-- = (unsigned long)0x55555555; // ebp
    *(*stk)-- = (unsigned long)0x66666666; // esi
    **stk = (unsigned long)0x77777777; // edi
}

typedef struct task_list {
    TCB data;
    struct task_list* next;
} task_list;

task_list* head;

const unsigned long stack_size = 0x100000; // 1 M

unsigned tid_cnt = 1;
int createTsk(void (*tskBody)(void)) {
    TCB tcb;
    tcb.tid = tid_cnt++;
    tcb.stack = (unsigned long*) malloc(stack_size) + stack_size - 1;
    tcb.state = WAITING;
    tcb.params = (tskPara) {.priority = 0, .arrTime = 0, .exeTime = 0};
    stack_init(&tcb.stack, tskBody);
    task_list* tmp = (task_list*) kmalloc(sizeof(task_list));
    tmp->data = tcb;
    tmp->next = head->next;
    head->next = tmp;
    return tcb.tid;
}

void destroyTsk(int tskIndex) {
    task_list* tmp = head->next, * prev = head;
    for (; tmp; tmp = tmp->next) {
        if (tmp->data.tid == tskIndex) {
            free(tmp->data.stack + 1 - stack_size);
            prev->next = tmp->next;
            kfree(tmp);
        }
        prev = tmp;
    }
}

void tskStart(TCB *tsk) {

}

void tskEnd() {

}
