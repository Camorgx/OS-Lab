#include "wallClock.h"
#include "shell.h"

void myMain(void){    
    setWallClockHook(displayWallClock);
    startShell();
}
