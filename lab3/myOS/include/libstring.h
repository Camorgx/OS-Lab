#ifndef __LIBSTRING_H__
#define __LIBSTRING_H__

// These functions are defined in myOS/lib/io/vsprintf.c

extern void * memset(void *dst, int val, unsigned long ulcount);
extern void * memcpy(void *dst, const void *src, unsigned long ulcount);

extern char * strchr(const char *str, int ch);
extern int strlen(const char * str);
extern int strncmp(const char *first,const char *last,int count);
extern unsigned long strnlen(const char *s, int count);
extern int strcmp(const char *source,const char *dest);
extern int atoi(char *str);
extern char * itoa(int n, char * chBuffer);
extern char * ftoa(double dValue, char * chBuffer);

#endif // __LIBSTRING_H__