#include "tick.h"
#include "wallClock.h"
int system_ticks;
int HH, MM, SS;

void tick(void){
    if (++system_ticks % 100 == 0) {
        if (++SS >= 60) { SS = 0; ++MM; }
        if (MM >= 60) { MM = 0; ++HH; }
        if (HH >= 24) HH = 0;
    }
    setWallClock(HH,MM,SS);
    return;
}

