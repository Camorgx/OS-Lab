#include "dev/i8253.h"

void init8253(void){
    const unsigned short para = 1193180 / 100;
    outb(0x43, 0x34);
    outb(0x40, para & 0xff); //	Lower bits
    outb(0x40, para >> 8); // Higher bits
    
    // Allow time interrupt
    unsigned char master = inb(0x21);
    unsigned char slave = inb(0xA1);
    master = (master >> 1) << 1;
    slave = (slave >> 1) << 1;
    outb(0x21, master);
    outb(0xA1, slave);
}
