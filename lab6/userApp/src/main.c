#include "userInterface.h"
#include "test_tasks.h"
#include "shell.h"

//#define MEM_TEST
#ifdef MEM_TEST
extern void memTestCaseInit(void);
#endif

#define FIFO_TEST

void main(void) {
    setWallClockHook(displayWallClock);
    initShell();

#ifdef MEM_TEST
    memTestCaseInit();
#endif

    unsigned shell = createTsk(startShell);

#ifdef FIFO_TEST
    unsigned t1 = createTsk(task1);
    unsigned t2 = createTsk(task2);
    unsigned t3 = createTsk(task3);
    unsigned t4 = createTsk(task4);
    unsigned t5 = createTsk(task5);
    tskStart(t3);
    tskStart(t5);
    tskStart(t4);
    tskStart(t2);
    tskStart(t1);
#endif // FIFO_TEST

    tskStart(shell);
}
