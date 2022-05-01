#include "userInterface.h"
#include "shell.h"

void myMain(void){    
    setWallClockHook(displayWallClock);
    initShell();
    startShell();
}
