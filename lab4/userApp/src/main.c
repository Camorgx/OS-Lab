#include "userInterface.h"
#include "shell.h"

#define MEM_TEST
#ifdef MEM_TEST
extern void memTestCaseInit(void);
#endif

void main(void){    
    setWallClockHook(displayWallClock);
    initShell();

#ifdef MEM_TEST
    memTestCaseInit();
#endif

    startShell();
}
