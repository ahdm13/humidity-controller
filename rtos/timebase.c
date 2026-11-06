#include <cmsis/stm32f1xx.h>
#include <stdint.h>

#define CTRL_ENABLE	((uint32_t)1 << 0)
#define CTRL_TICKINT	((uint32_t)1 << 1)
#define CTRL_CLKSRC	((uint32_t)1 << 2)
#define CTRL_COUNTFLAG	((uint32_t)1 << 16)

void timebase_init(uint32_t load)
{
	/* Disable interrupts: */
	__disable_irq();

	/* Load timer with the reload value for the given quanta */
	SysTick->LOAD = load;

	/* Configure SysTick using control register and enable it */
	SysTick->CTRL = (CTRL_CLKSRC | CTRL_TICKINT | CTRL_ENABLE);

	/* Enable global interrupts */
	NVIC_SetPriority(SysTick_IRQn, 15);
}
