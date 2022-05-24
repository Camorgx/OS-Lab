#include "lib/libio.h"
#include "dev/vga.h"
#include "dev/i8253.h"
#include "dev/i8259A.h"
#include "i386/tick.h"
#include "kernel/mem/mem.h"

extern void main(void);

#define MEM_TEST
void osStart(void)
{
	init8259A();
	init8253();
	tick();
	enable_interrupt();
    clear_screen();

#ifdef MEM_TEST
    pMemInit();  //after this, we can use kmalloc/kfree and malloc/free
#endif //MEM_TEST

    printk(0x2, "START RUNNING......\n");
    main();
    printk(0x2, "STOP RUNNING......ShutDown\n");
}
