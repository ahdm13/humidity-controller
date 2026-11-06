#include "led.h"
#include <cmsis/stm32f1xx.h>
#include <stdint.h>

#define IOPAEN ((uint32_t)1 << 2)

#define LED_PIN5_SET ((uint32_t)1 << 21)
#define LED_PIN5_UNSET (((uint32_t)1 << 20) | ((uint32_t)1 << 22) \
		| ((uint32_t)1 << 23))

void led_init(void)
{
	/* Enable clock access to LED PORT (PORTA) */
	RCC->APB2ENR |= IOPAEN;

	/* Set LED pin (pin 5) as ouput pin */
	GPIOA->CRL |= LED_PIN5_SET;
	GPIOA->CRL &= ~LED_PIN5_UNSET;
}

void led_on(void)
{
	/* Set LED pin high */
	GPIOA->ODR |= (1 << 5);
}

void led_off(void)
{
	/* Set LED pin low */
	GPIOA->ODR &= ~(1 << 5);
}
