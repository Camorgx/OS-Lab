#include "../../include/vga.h"
#include "../../include/uart.h"
#include "../../include/libio.h"

char kBuf[400];
int printk(int color, const char *format, ...) {
    for (int i = 0; i < 400; ++i)
        kBuf[i] = '\0';
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(kBuf, format, args);
    va_end(args);
    
    // VGA 输出
    append2screen(kBuf, color);

    // 串口输出
    uart_put_chars(kBuf);
    
    return cnt;
}

char uBuf[400];
int printf(int color, const char *format, ...) {
    for (int i = 0; i < 400; ++i)
        kBuf[i] = '\0';
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(kBuf, format, args);
    va_end(args);
    
    // VGA 输出
    append2screen(kBuf, color);

    // 串口输出
    uart_put_chars(kBuf);
    
    return cnt;
}
