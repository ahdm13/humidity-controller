#include "rtos.h"
#include <cmsis/stm32f1xx.h>
#include <stddef.h>
#include <stdint.h>

/* 
   Stack frame structure:
   	PSR
   	PC
   	LR
   	R12
   	R3
   	R2
   	R1
   	R0 <- TOS

   The last 32 bytes of a tcb stack stores its context information.
 */

#define PENDSTSET (uint32_t)(1 << 26)

static uint32_t *stack_init(uint32_t *tos, void (*thread)(void));

struct rtos_tcb *rtos_thread_current = NULL;
struct rtos_tcb *rtos_thread_last = NULL;

void rtos_thread_add(void (*thread)(void), struct rtos_tcb *tcb, uint32_t *tos)
{
	__disable_irq();

	if (!rtos_thread_current)
		rtos_thread_current = tcb;

	if (!rtos_thread_last)
		rtos_thread_last = tcb;
	else
		rtos_thread_last->next = tcb;

	tos = stack_init(tos, thread);
	tcb->tos = tos;
	tcb->next = NULL;

	rtos_thread_last = tcb;

	__enable_irq();
}

static uint32_t *stack_init(uint32_t *tos, void (*thread)(void))
{
	*tos = 1 << 24; /* Set T bit (thumb mode). */

	*(--tos) = (uint32_t)thread;

	/* Dummy stack content (for debugging purposes): */
	*(--tos) = 0xAAAAAAAA; /* R14, i.e, LR */
	*(--tos) = 0xAAAAAAAA; /* R12 */
	*(--tos) = 0xAAAAAAAA; /* R3 */
	*(--tos) = 0xAAAAAAAA; /* R2 */
	*(--tos) = 0xAAAAAAAA; /* R1 */
	*(--tos) = 0xAAAAAAAA; /* R0 */
	*(--tos) = 0xAAAAAAAA; /* R11 */
	*(--tos) = 0xAAAAAAAA; /* R10 */
	*(--tos) = 0xAAAAAAAA; /* R9 */
	*(--tos) = 0xAAAAAAAA; /* R8 */
	*(--tos) = 0xAAAAAAAA; /* R7 */
	*(--tos) = 0xAAAAAAAA; /* R6 */
	*(--tos) = 0xAAAAAAAA; /* R5 */
	*(--tos) = 0xAAAAAAAA; /* R4 */

	return tos;
}

void rtos_thread_yield(void)
{
	/* Clear SysTick value: */
	SysTick->VAL = 0;

	/* Set SysTick interrupt to pending: */
	SCB->ICSR |= PENDSTSET;
}
