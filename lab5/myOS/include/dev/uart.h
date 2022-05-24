#ifndef __UART_H__
#define __UART_H__

extern void uart_put_char(unsigned char ch);
extern void uart_put_chars(char *str);

extern unsigned char uart_get_char(void);

#endif