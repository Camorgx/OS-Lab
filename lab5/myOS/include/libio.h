#ifndef __LIBIO_H__
#define __LIBIO_H__

#include <stdarg.h>
extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int sprintf(char *buf, const char *fmt, ...);

extern int printk(int color, const char* format, ...);
extern int printf(int color, const char* format, ...);

#endif // __LIBIO_H__