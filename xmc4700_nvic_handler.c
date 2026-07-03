#include <stdio.h>

#include <XMC4700.h>

#include <xmc_uart.h>

#include "custom_def.h"
#include "utilis.h"

extern volatile uint32_t g_ticks;
extern volatile uint8_t g_tmp_uart_rx_buf;

extern int stdout_putchar (int ch);
	
void SysTick_Handler(void) {	
	g_ticks ++;
}

void USIC0_0_IRQHandler(void) {
  g_tmp_uart_rx_buf = XMC_UART_CH_GetReceivedData(SERIAL_UART);
	
	stdout_putchar(g_tmp_uart_rx_buf);
}
