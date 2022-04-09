#include "wallClock.h"

extern void startShell(void);

void myMain(void){    
    setWallClockHook(setWallClock);
    startShell();
}
