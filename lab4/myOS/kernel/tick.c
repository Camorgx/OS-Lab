#include "tick.h"
#include "wallClock.h"
int system_ticks;

void tick(void){
    ++system_ticks;
    oneTickUpdateWallClock();
}

