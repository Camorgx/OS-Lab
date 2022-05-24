#ifndef LAB5_TASK_H
#define LAB5_TASK_H

extern void CTX_SW(void);
extern void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);

#endif //LAB5_TASK_H
