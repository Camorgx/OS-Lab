#include "i386/tick.h"
#include "dev/wallClock.h"
int system_ticks;

void tick(void){
    ++system_ticks;
    oneTickUpdateWallClock();
}

