#include "i386/tick.h"
#include "dev/wallClock.h"
#include "kernel/scheduling/scheduler.h"
unsigned system_ticks;

void tick(void){
    ++system_ticks;
    oneTickUpdateWallClock();
    if (system_ticks % 100 == 0) check_arrive();
    if (system_scheduler.tick_func) system_scheduler.tick_func();
}

