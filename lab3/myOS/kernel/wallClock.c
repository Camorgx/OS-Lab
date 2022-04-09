#include "wallClock.h"
#include "vga.h"

const int hour_offset = VGA_SCREEN_HEIGHT * VGA_SCREEN_WIDTH + VGA_SCREEN_WIDTH - 8;

void (*wallClock_hook)(int, int, int) = (void*)0;

void oneTickUpdateWallClock(int HH, int MM, int SS) {
	if(wallClock_hook) wallClock_hook(HH,MM,SS);
}

void setWallClockHook(void (*func)(int, int, int)) {
	wallClock_hook = func;
}

void setWallClock(int HH,int MM,int SS) {
    put_char2pos(HH / 10 + '0', 0x2, hour_offset);
    put_char2pos(HH % 10 + '0', 0x2, hour_offset + 1);
    put_char2pos(':', 0x2, hour_offset + 2);
    put_char2pos(MM / 10 + '0', 0x2, hour_offset + 3);
    put_char2pos(MM % 10 + '0', 0x2, hour_offset + 4);
    put_char2pos(':', 0x2, hour_offset + 5);
    put_char2pos(SS / 10 + '0', 0x2, hour_offset + 6);
    put_char2pos(SS % 10 + '0', 0x2, hour_offset + 7);
}

void getWallClock(int *HH,int *MM,int *SS) {
    short* dest = VGA_BASE + hour_offset;
    *HH = (*dest - '0') * 10 + (*(dest + 1) - '0');
    *MM = (*(dest + 3) - '0') * 10 + (*(dest + 4) - '0');
    *SS = (*(dest + 6) - '0') * 10 + (*(dest + 7) - '0');
}
