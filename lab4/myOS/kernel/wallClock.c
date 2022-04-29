#include "wallClock.h"
#include "vga.h"

const int hour_offset = VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH + VGA_SCREEN_WIDTH - 8;
extern int system_ticks;
int HH, MM, SS;

void (*wallClock_hook)(void) = (void*)0;


void oneTickUpdateWallClock(void) {
	if (system_ticks % 100 == 0) {
        if (++SS >= 60) { SS = 0; ++MM; }
        if (MM >= 60) { MM = 0; ++HH; }
        if (HH >= 24) HH = 0;
    }
    if (wallClock_hook) wallClock_hook();
}

void setWallClockHook(void (*func)(void)) {
	wallClock_hook = func;
}

void displayWallClock(void) {
    int h, m, s;
    getWallClock(h, m, s);
    put_char2pos(h / 10 + '0', 0x2, hour_offset);
    put_char2pos(h % 10 + '0', 0x2, hour_offset + 1);
    put_char2pos(':', 0x2, hour_offset + 2);
    put_char2pos(m / 10 + '0', 0x2, hour_offset + 3);
    put_char2pos(m % 10 + '0', 0x2, hour_offset + 4);
    put_char2pos(':', 0x2, hour_offset + 5);
    put_char2pos(s / 10 + '0', 0x2, hour_offset + 6);
    put_char2pos(s % 10 + '0', 0x2, hour_offset + 7);
}

void setWallClock(int h,int m,int s) {
    HH = (h >= 0 && h <= 24) ? h : 0;
    MM = (m >= 0 && m <= 59) ? m : 0;
    SS = (s >= 0 && s <= 59) ? s : 0;
}

void getWallClock(int *h,int *m,int *s) {
    *h = HH; *m = MM; *s = SS;
}
