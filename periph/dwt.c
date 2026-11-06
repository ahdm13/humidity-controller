#include "dwt.h"
#include "system.h"
#include <cmsis/stm32f1xx.h>
#include <stdint.h>

void dwt_init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable TRC
	DWT->CYCCNT = 0;                              // Reset cycle counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;          // Enable cycle counter
}

void dwt_delay_us(volatile uint32_t us) {
	uint32_t clk_cycles_per_us = SYSFREQ / 1000000;
	us *= clk_cycles_per_us;
	uint32_t start_tick = DWT->CYCCNT;
	while ((DWT->CYCCNT - start_tick) < us);
}

void dwt_delay_ms(volatile uint32_t ms) {
	while (ms--)
		dwt_delay_us(1000);
}
