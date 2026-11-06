#include <periph/uart.h>
#include <stddef.h>
#include <stdint.h>

static const char *w_extra_args = "W: Redundant arguments\r\n";
static const char *e_insuf_args = "E: Insufficient arguments\r\n";

uint32_t cmd_atou(const char *str)
{
	uint32_t ret;
	ret = 0;

	for (; *str != '\0'; str++) {
		if (*str < '0' || *str > '9')
			break;
		ret *= 10;
		ret += *str - '0';
	}

	return ret;
}

size_t cmd_utoa(uint32_t value, char *dst, size_t dstsz)
{
	size_t i;

	if (dstsz < 2)
		return 0;

	if (value == 0) {
		*dst = '0';
		*(dst + 1) = '\0';
		return 1;
	}

	i = 0;
	while (1) {
		uint32_t digit;

		if (i + 1 >= dstsz || value == 0)
			break;

		digit = value % 10;
		dst[i] = '0' + digit;

		i++;
		value /= 10;
	}
	dst[i] = '\0';

	for (size_t j = 0; j < i/2; j++) {
		char tmp = dst[j];
		dst[j] = dst[i - j - 1];
		dst[i - j - 1] = tmp;
	}

	return i;
}

int cmd_checkargc(int argc, int nargs)
{
	if (argc < nargs + 1) {
		uart_write(e_insuf_args);
		return 1;
	}

	if (argc > nargs + 1)
		uart_write(w_extra_args);

	return 0;
}
