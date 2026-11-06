#ifndef RTOS_MUTEX_H
#define RTOS_MUTEX_H

#include <stdbool.h>

void rtos_mutex_init(bool *mutex);

int rtos_mutex_get(bool *mutex);

void rtos_mutex_rel(bool *mutex);

#endif /* RTOS_MUTEX_H */
