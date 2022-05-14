#ifndef __WALLCLOCK_H__
#define __WALLCLOCK_H__

extern void setWallClock(int HH,int MM,int SS);
extern void setWallClockHook(void (*func)(int, int, int));
extern void oneTickUpdateWallClock(int HH, int MM, int SS);
extern void getWallClock(int *HH,int *MM,int *SS);

#endif
