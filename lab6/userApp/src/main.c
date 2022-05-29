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

    unsigned shell = createTsk(startShell, 1, 1000);

#ifdef FIFO_TEST
    createTsk(task2, 6, 200);
    createTsk(task1, 2, 200);
    createTsk(task4, 5, 200); 
    createTsk(task3, 3, 200);
    createTsk(task5, 4, 300);
#endif // FIFO_TEST
}
