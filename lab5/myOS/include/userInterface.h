#ifndef LAB4_USERINTERFACE_H
#define LAB4_USERINTERFACE_H

#include <stdarg.h>

#include "dev/wallClock.h"
#include "dev/uart.h"
#include "lib/libmem.h"
#include "lib/libio.h"
#include "dev/vga.h"

#ifndef NULL
#define NULL ((void*)0)
#endif // NULL

#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif // UNUSED

extern unsigned createTsk(void (*tskBody)(void));
extern void tskStart(unsigned tskIndex);

#endif // LAB4_USERINTERFACE_H
