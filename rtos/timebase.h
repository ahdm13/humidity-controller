#ifndef RTOS_TIMEBASE_H
#define RTOS_TIMEBASE_H

#include <stdint.h>

#define TIMEBASE_FREQ 16000000

void timebase_init(uint32_t load);

#endif /* RTOS_TIMEBASE_H */
