#include "dev/vga.h"
#include "dev/i8253.h"
#include "dev/i8259A.h"
#include "i386/tick.h"
#include "kernel/mem/mem.h"
#include "kernel/scheduling/scheduler.h"

extern void main(void);

void osStart(void)
{
	init8259A();
	init8253();
	tick();
	enable_interrupt();
    clear_screen();
    pMemInit();
    init_tsk_manager(FCFS);
}
