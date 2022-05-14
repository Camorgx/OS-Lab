#include "libio.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "mem.h"

extern void main(void);

#define MEM_TEST
void osStart(void)
{
	init8259A();
	init8253();
	tick();
	enable_interrupt();
    clear_screen();

    pMemInit();  //after this, we can use kmalloc/kfree and malloc/free
#ifndef MEM_TEST
    clear_screen();
#endif //MEM_TEST

    printk(0x2, "START RUNNING......\n");
    main();
    printk(0x2, "STOP RUNNING......ShutDown\n");
}
