#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <XMC4700.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include "serial.h"

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(SERIAL_UART, (uint8_t)ch);
	return ch;
}

int stderr_putchar (int ch) {
	XMC_UART_CH_Transmit(SERIAL_UART, (uint8_t)ch);
	return ch;
}

void ttywrch (int ch) {
	XMC_UART_CH_Transmit(SERIAL_UART, (uint8_t)ch);
}
