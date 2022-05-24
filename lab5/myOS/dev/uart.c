#include "i386/io.h"
#include "dev/uart.h"

#define UART_PORT 0x3F8 // 串口端口号

// 从串口输入一个字符
// 来自助教提供的 uart.c 文件
unsigned char uart_get_char(void) {
	while (!(inb(UART_PORT + 5) & 1));
	return inb(UART_PORT);
}

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
