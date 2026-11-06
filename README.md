Industrial Automation Course Project: Humidity Controller

A humidity controller based on DHT22 sensor, implemented on a st-nucleo-f103rb
development board. The application is interface via UART (available directly on
the USB port on the board). There is a minimal unix-like shell for interface.

A minimal RTOS had to be implemented, since the DHT22 sensor took 2s for one
sample.

## Code Organization:
`src` consists of the main source code.

`cmd` consists of code for implemented commands.

`rtos` has code for the rtos.

`periph` consists of code for various internal and external peripherals (uart,
DHT22, etc.)
