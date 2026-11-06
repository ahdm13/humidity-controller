#ifndef PERIPH_DHT22_H
#define PERIPH_DHT22_H

#include <stdint.h>

void dht22_init(void);

/* Returns 0 if successful, -1 if sensor times out, -2 if checksum check failed. */
int dht22_get_data(uint32_t *result);

#endif /* PERIPH_DHT22_H */
