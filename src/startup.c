#include <stdint.h>
#include <periph/uart.h>

#define SRAM_START (0x20000000U)
#define SRAM_SIZE (20U * 1024U)
#define SRAM_END (SRAM_START + SRAM_SIZE)
#define STACK_POINTER_INIT_ADDRESS (SRAM_END)
#define ISR_VECTOR_SIZE_WORDS 84

void reset_handler(void);
void default_handler(void);

// Cortex-M system exceptions
void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handler(void) __attribute__((weak, alias("default_handler")));
void mem_manage_fault_handler(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handler(void) __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void timebase_interrupt(void) __attribute__((weak, alias("default_handler")));

uint32_t isr_vector[ISR_VECTOR_SIZE_WORDS] __attribute__((section(".isr_vector"))) = {
	STACK_POINTER_INIT_ADDRESS,
	// Cortex-M system exceptions
	(uint32_t)&reset_handler,
	(uint32_t)&nmi_handler,
	(uint32_t)&hard_fault_handler,
	(uint32_t)&mem_manage_fault_handler,
	(uint32_t)&bus_fault_handler,
	(uint32_t)&usage_fault_handler,
	0,
	0,
	0,
	0,
	(uint32_t)&svcall_handler,
	(uint32_t)&debug_monitor_handler,
	0,
	(uint32_t)&pendsv_handler,
	(uint32_t)&timebase_interrupt,
 
	// STM32F410 interrupt handlers
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
	(uint32_t)&default_handler,
};

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _sidata;
void main(void);

void reset_handler(void)
{
	// Copy .data from FLASH to SRAM
	uint32_t data_size = (uint32_t)&_edata - (uint32_t)&_sdata;
	uint8_t *flash_data = (uint8_t*) &_sidata; // Data load address (in flash)
	uint8_t *sram_data = (uint8_t*) &_sdata; // Data virtual address (in sram)
	
	for (uint32_t i = 0; i < data_size; i++)
	{
	  sram_data[i] = flash_data[i];
	}

	// Zero-fill .bss section in SRAM
	uint32_t bss_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	uint8_t *bss = (uint8_t*) &_sbss;

	for (uint32_t i = 0; i < bss_size; i++)
	{
	  bss[i] = 0;
	}
	
	main();
}

void default_handler(void)
{
	uart_init();
	uart_write("ERROR: Default interrupt handler called\r\n");
	while(1);
}
