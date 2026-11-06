#include "internal.h"
#include <periph/mistmaker.h>
#include <periph/uart.h>
#include <stdint.h>

int cmd_mistmaker_set(int argc, char **argv)
{
	uint32_t setval;
	uint32_t percent;

	if (cmd_checkargc(argc, 1))
		return 1;

	percent = cmd_atou(argv[1]);
	if (percent > 100) {
		uart_write("E: Invalid argument\r\n");
		return 2;
	}

	setval = (0x0FFF * percent)/100;
	mistmaker_set(setval & 0xFFFF);
	return 0;
}
