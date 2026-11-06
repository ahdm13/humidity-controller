#include "mutex.h"
#include <cmsis/stm32f1xx.h>
#include <stdbool.h>

void rtos_mutex_init(bool *mutex)
{
	__disable_irq();
	*mutex = false;
	__enable_irq();
}

int rtos_mutex_get(bool *mutex)
{
	if (*mutex)
		return -1;

	__disable_irq();
	*mutex = true;
	__enable_irq();

	return 0;
}

void rtos_mutex_rel(bool *mutex)
{
	__disable_irq();
	*mutex = false;
	__enable_irq();
}
