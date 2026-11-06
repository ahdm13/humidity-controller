#include "embsh.h"
#include <cmd/cmd.h>
#include <periph/dht22.h>
#include <periph/dwt.h>
#include <periph/mistmaker.h>
#include <periph/uart.h>
#include <rtos/rtos.h>
#include <stdbool.h>
#include <stdint.h>

#define NTHREADS	2
#define STACKSIZE	128

static struct rtos_tcb tcb[NTHREADS];
static uint32_t stack[NTHREADS][STACKSIZE];

uint32_t dht22_data;
bool dht22_enabled;

void task_shell(void)
{
	struct embsh shell = { cmd, ncmd, uart_getchar, uart_putchar };
	while (1) {
		char ch[2];

		uart_write("\r\n");
		while (embsh_exec(shell, NULL) >= 0);

		uart_write("[Press any key to activate shell]");
		uart_write("\r\n");
		uart_read(ch, sizeof(ch));
	}
}

void task_dht22(void)
{
	while (1) {
		int status;

		while (!dht22_enabled);
		dwt_delay_ms(2000);

		status = dht22_get_data(&dht22_data);
		if (status != 0) {
			uart_write("ERROR: DHT22: ");
			if (status != -5) {
				uart_write("sensor timed out (-");
				uart_putchar('0' - status);
				uart_write(")");
//				dht22_enabled = false;
			} else {
				uart_write("checksum error");
			}
			uart_write("\r\n");
		}
	}
}

int main(void)
{
	dht22_init();
	uart_init();
	mistmaker_init();

	dht22_data = 0;
	dht22_enabled = true;

	rtos_thread_add(task_shell, &tcb[0], &stack[0][STACKSIZE - 1]);
	rtos_thread_add(task_dht22, &tcb[1], &stack[1][STACKSIZE - 1]);
	rtos_kernel_launch(10);

	while (1);
}
