#include "mistmaker.h"
#include "system.h"
#include <cmsis/stm32f1xx.h>
#include <stdint.h>

#define TIM1EN (uint32_t)(1 << 11)
#define IOPAEN (uint32_t)(1 << 2)

#define CR1_CEN (uint32_t)(1 << 0)
#define EGR_UG (uint32_t)(1 << 0)
#define CCMR1_PWM1 (uint32_t)(0x6 << 4)
#define OC1PE (uint32_t)(1 << 3);
#define CR1_ARPE (uint32_t)(1 << 7)
#define BDTR_MOE (uint32_t)(1 << 15)

void mistmaker_init(void)
{
	RCC->APB2ENR |= TIM1EN | IOPAEN;
	GPIOA->CRH &= ~((uint32_t)0xF); /* Clear CNF and MODE */
	GPIOA->CRH |= ((uint32_t)0xB);  /* 2MHz alternate function output */

	TIM1->PSC = 1;
	TIM1->ARR = 0x0FFF;

	TIM1->CCMR1 |= CCMR1_PWM1 | OC1PE;
	TIM1->CCR1 = 0x07FF;
	TIM1->CCER |= 1;

	TIM1->BDTR |= BDTR_MOE;
	TIM1->CR1 |= CR1_CEN;
}

void mistmaker_set(uint16_t ccr1)
{
	TIM1->CCR1 = ccr1;
}
