#include "libio.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "mem.h"
#include "libmem.h"

extern void myMain(void);

void osStart(void)
{
	init8259A();
	init8253();
	tick();
	enable_interrupt();
	
    clear_screen();

    /*pMemInit();  //after this, we can use kmalloc/kfree and malloc/free

	{
		unsigned long tmp = dPartitionAlloc(pMemHandler,100);
		dPartitionWalkByAddr(pMemHandler);
		dPartitionFree(pMemHandler,tmp);
		dPartitionWalkByAddr(pMemHandler);
	}*/

    myPrintk(0x2, "START RUNNING......\n");
    myMain();
    myPrintk(0x2, "STOP RUNNING......ShutDown\n");
    while (1);
}
