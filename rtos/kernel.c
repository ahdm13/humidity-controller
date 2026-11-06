#include "internal.h"
#include "rtos.h"
#include "timebase.h"
#include <stdint.h>

#define MS_PRESCALER	TIMEBASE_FREQ/1000

void rtos_kernel_launch(uint32_t quanta_ms)
{
	rtos_thread_last->next = rtos_thread_current;
	timebase_init((MS_PRESCALER * quanta_ms) - 1);

	/* Launch scheduler */
	rtos_scheduler_launch();
}
