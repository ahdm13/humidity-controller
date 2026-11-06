#ifndef RTOS_INTERNAL_H
#define RTOS_INTERNAL_H

#include "rtos.h"

extern struct rtos_tcb *rtos_thread_current;
extern struct rtos_tcb *rtos_thread_last;

void rtos_scheduler_launch(void);

#endif /* RTOS_INTERNAL_H */
