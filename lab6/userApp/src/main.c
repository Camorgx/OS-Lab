#include "userInterface.h"
#include "test_tasks.h"
#include "shell.h"

//#define MEM_TEST
#ifdef MEM_TEST
extern void memTestCaseInit(void);
#endif

#define SCHEDULE_TEST

void main(void) {
    setWallClockHook(displayWallClock);
    initShell();

#ifdef MEM_TEST
    memTestCaseInit();
#endif

    createTsk(startShell, 1, 1000);

#ifdef SCHEDULE_TEST
    createTsk(task2, 6, 100);
    createTsk(task1, 2, 100);
    createTsk(task4, 5, 100);
    createTsk(task3, 3, 100);
    createTsk(task5, 4, 200);
#endif // SCHEDULE_TEST
}
