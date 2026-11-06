#include "internal.h"
#include <periph/uart.h>
#include <stdint.h>

#define UART_DUMP_U(val, x) do {                               \
	uart_write("0b");                                      \
	for (size_t i = x; i > 0; i--) {                       \
		if ((val) & (1 << (i - 1)))                    \
			uart_write("1");                       \
		else                                           \
			uart_write("0");                       \
	}                                                      \
	uart_write("\r\n");                                    \
} while (0)

extern uint32_t dht22_data;

int cmd_dht22_data(int argc, char **argv)
{
	(void)argv;
	if (cmd_checkargc(argc, 0))
		return 1;

	UART_DUMP_U(dht22_data, 32);
	return 0;
}
