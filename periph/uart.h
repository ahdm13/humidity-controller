#ifndef PERIPH_UART_H
#define PERIPH_UART_H

#include <stddef.h>

void uart_init(void);

void uart_putchar(char c);

void uart_write(const char *c);

int uart_getchar(void);

void uart_read(char *dst, size_t dstsz);

#endif /* PERIPH_UART_H */
