#include "dev/i8259A.h"

void init8259A(void){
    // Block all interrupt sources.
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

    // Master Init
    outb(0x20, 0x11);
    outb(0x21, 0x20);
    outb(0x21, 0x04);
    outb(0x21, 0x03);

    // Slave Init
    outb(0xA0, 0x11);
    outb(0xA1, 0x28);
    outb(0xA1, 0x02);
    outb(0xA1, 0x01);
}
