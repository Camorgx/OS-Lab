#ifndef __I386_IO_H__
#define __I386_IO_H__

extern unsigned char inb(unsigned short int port_from);

extern void outb(unsigned short int port_to, unsigned char value);

#endif