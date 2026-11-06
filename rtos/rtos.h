#ifndef RTOS_RTOS_H
#define RTOS_RTOS_H

#include <stdint.h>

struct rtos_tcb {
	uint32_t *tos;
	struct rtos_tcb *next;
};

void rtos_thread_add(void (*thread)(void), struct rtos_tcb *tcb, uint32_t *tos);

void rtos_thread_yield(void);

void rtos_kernel_launch(uint32_t quanta);

#endif /* RTOS_RTOS_H */
