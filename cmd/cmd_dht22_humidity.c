#include "internal.h"
#include <periph/uart.h>
#include <stdint.h>

extern uint32_t dht22_data;

int cmd_dht22_humidity(int argc, char **argv)
{
	uint16_t temp;
	size_t size;
	char data[5];

	(void)argv;
	if (cmd_checkargc(argc, 0))
		return 1;

	temp = ((dht22_data >> 16) & 0x0000FFFF);
	size = cmd_utoa(temp, data, sizeof(data));
	for (size_t i = 0; i < size - 1; i++)
		uart_putchar(data[i]);
	uart_putchar('.');
	uart_putchar(data[size - 1]);
	uart_write("%\r\n");
	return 0;
}
