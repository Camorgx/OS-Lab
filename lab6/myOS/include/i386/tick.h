#ifndef __TICK_H__
#define __TICK_H__
extern unsigned system_ticks;
extern int HH,MM,SS;

// From asm file.
extern void enable_interrupt(void);
extern void disable_interrupt(void);

extern void tick(void);

#endif
