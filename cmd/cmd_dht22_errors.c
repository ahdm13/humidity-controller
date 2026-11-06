#include "internal.h"
#include <periph/dht22.h>
#include <periph/uart.h>
#include <stdbool.h>

extern bool dht22_errors;

static int string_equals(const char *str1, const char *str2)
{
	size_t i;

	if (str1 == NULL && str2 == NULL)
		return 1;

	if (str1 == NULL || str2 == NULL)
		return 0;

	for (i = 0; *(str1 + i) != '\0' && *(str2 + i) != '\0'; i++) {
		if (*(str1 + i) != *(str2 + i))
			return 0;
	}

	if (*(str1 + i) != *(str2 + i))
		return 0;

	return 1;
}

int cmd_dht22_errors(int argc, char **argv)
{
	if (argc == 1) {
		if (dht22_errors)
			uart_write("enabled\r\n");
		else
			uart_write("disabled\r\n");

		return 0;
	}

	if (cmd_checkargc(argc, 1))
		return 1;

	if (string_equals(argv[1], "enable")) {
		dht22_errors = true;
		return 0;
	}

	if (string_equals(argv[1], "disable")) {
		dht22_errors = false;
		return 0;
	}

	uart_write("E: Invalid argument\r\n");
	return 2;
}
