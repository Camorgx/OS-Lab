#ifndef __WALLCLOCK_H__
#define __WALLCLOCK_H__

extern void setWallClock(int HH, int MM, int SS);
extern void setWallClockHook(void (*func)(void));
extern void oneTickUpdateWallClock(void);
extern void displayWallClock(void);
extern void getWallClock(int *HH, int *MM, int *SS);

#endif
