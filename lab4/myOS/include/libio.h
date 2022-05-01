#ifndef __LIBIO_H__
#define __LIBIO_H__

#include <stdarg.h>
extern int vsprintf(char *buf, const char *fmt, va_list args);
extern int sprintf(char *buf, const char *fmt, ...);

extern int myPrintk(int color, const char* format, ...);
extern int myPrintf(int color, const char* format, ...);

#endif // __LIBIO_H__