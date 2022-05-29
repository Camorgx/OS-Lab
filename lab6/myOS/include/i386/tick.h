#ifndef __TICK_H__
#define __TICK_H__
extern int system_ticks;
extern int HH,MM,SS;

// From asm file.
extern void enable_interrupt(void);
extern void disable_interrupt(void);

extern void tick(void);

#endif
