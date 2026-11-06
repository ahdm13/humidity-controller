#ifndef PERIPH_DWT_H
#define PERIPH_DWT_H

#include <stdint.h>

void dwt_init(void);

void dwt_delay_us(volatile uint32_t us);

void dwt_delay_ms(volatile uint32_t ms);

#endif /* PERIPH_DWT_H */
