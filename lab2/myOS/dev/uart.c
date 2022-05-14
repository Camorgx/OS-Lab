#include "io.h"
#include "uart.h"

#define UART_PORT 0x3F8 // 串口端口号

/* 向串口输出一个字符
 * 使用封装好的 outb 函数 */
void uart_put_char(unsigned char ch) {
    outb(UART_PORT, ch);
    if (ch == '\n') outb(UART_PORT, '\r');
}

/* 向串口输出一个字符串
 * 此函数接口禁止修改 */
void uart_put_chars(char *str) {
    for (char* i = str; *i != '\0'; ++i)
        uart_put_char(*i);
}
