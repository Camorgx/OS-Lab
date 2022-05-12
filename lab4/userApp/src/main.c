#include "userInterface.h"
#include "shell.h"
#include "mem.h"

#define MEM_TEST
#ifdef MEM_TEST
extern void memTestCaseInit(void);
#endif
//#define DEBUG
void main(void) {
#ifndef DEBUG
    setWallClockHook(displayWallClock);
    initShell();

#ifdef MEM_TEST
    memTestCaseInit();
#endif

    startShell();
#endif


#ifdef DEBUG
    unsigned long p1 = dPartitionAlloc(kernelMemHandler, 0x100);
    printf(0x7, "p = 0x%x\n", p1);
    unsigned long p2 = dPartitionAlloc(kernelMemHandler, 0x100);
    printf(0x7, "p = 0x%x\n", p2);
    dPartitionWalkByAddr(kernelMemHandler);
#endif
}
