#include "dht22.h"
#include "dwt.h"
#include <cmsis/stm32f1xx.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* This module used DWT for delay. So it shouldn't be used elsewhere. */

#define IOPAEN ((uint32_t)(1 << 2))
#define PA10 ((uint32_t)(1 << 10))

#define SET_OUTPUT() do {                                             \
	/* Set PA10 as output pin: */                                 \
	GPIOA->CRH &= ~(uint32_t)(0xF << 8); /* Clear CNF and MODE */ \
	GPIOA->CRH |= (uint32_t)(0x6 << 8);  /* Open drain output */  \
} while (0)

#define SET_INPUT() do {                                              \
	/* Set PA10 as input pin: */                                  \
	GPIOA->CRH &= ~(uint32_t)(0xF << 8); /* Clear CNF and MODE */ \
	GPIOA->CRH |= (uint32_t)(0x4 << 8);  /* Floating input */     \
} while (0)

#define WAIT(var, val, cond)                                          \
	for ((var) = (val); (!(cond)) && ((var) > 0); (var)--)        \
		dwt_delay_us(1)

void dht22_init(void)
{
	/* Enable clock access to DHT port (PORTA): */
	RCC->APB2ENR |= IOPAEN;

	/* Set data line as output: */
	SET_OUTPUT();

	/* Set output to high by default: */
	GPIOA->ODR |= PA10;

	/* Initialize DWT: */
	dwt_init();
}

int dht22_get_data(uint32_t *result)
{
	int status;
	uint32_t timeout;
	uint8_t data[5], checksum;

	status = 0;

	/* 1. Send start signal, recieve response signal: */

	GPIOA->ODR &= ~PA10; /* Set data line low */
	dwt_delay_ms(5);    /* for 5ms. */

	GPIOA->ODR |= PA10;  /* Pull data line high */

	SET_INPUT();        /* Set data line as input */
	WAIT(timeout, 80, !(GPIOA->IDR & PA10)); /* Wait for data line low */
	if (timeout == 0) {
		status = -1;
		goto cleanup;
	}

	WAIT(timeout, 100, GPIOA->IDR & PA10); /* Wait for data line high */
	if (timeout == 0) {
		status = -2;
		goto cleanup;
	}

	/* 2. Receive data: */
	WAIT(timeout, 100, !(GPIOA->IDR & PA10)); /* Wait for data line low */
	if (timeout == 0) {
		status = -3;
		goto cleanup;
	}

	for (size_t i = 0; i < 5; i++) {
		for (size_t j = 0; j < 8; j++) {
			uint32_t count;

			 /* Wait for data line high: */
			WAIT(timeout, 70, GPIOA->IDR & PA10);

			for (count = 0; GPIOA->IDR & PA10; count++) {
				dwt_delay_us(5);
				if (count >= 40) {
					status = -4;
					goto cleanup;
				}
			}

			if (count < 3) {
				data[i] &= ~(1 << (7 - j));
			} else {
				data[i] |= (1 << (7 - j));
			}
		}
	}

	dwt_delay_us(50);

	/* 4. Verify checksum: */
	checksum = data[0] + data[1] + data[2] + data[3];
	if (checksum != data[4]) {
		status = -5;
		goto cleanup;
	}

	*result = ((uint32_t)data[0] << 24)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[3] << 0);

cleanup:

	/* 3. Pull data line high: */
	SET_OUTPUT();
	GPIOA->ODR |= PA10;
	return status;
}
