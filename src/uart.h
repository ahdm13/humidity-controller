#ifndef UART_H
#define UART_H

#include <stddef.h>

/* UART access wrapped in a mutex: */

void m_uart_init(void);

void m_uart_putchar(char c);

void m_uart_write(const char *c);

int m_uart_getchar(void);

void m_uart_read(char *dst, size_t dstsz);

#endif /* UART_H */
