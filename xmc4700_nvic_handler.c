#include <XMC4700.h>

#include "serial.h"

extern __IO uint32_t g_ticks;
extern __IO uint8_t g_tmp_uart_rx_buf;

void SysTick_Handler(void) {	
	g_ticks ++;
}

void USIC0_0_IRQHandler(void) {
  g_tmp_uart_rx_buf = XMC_UART_CH_GetReceivedData(SERIAL_UART);
	
	putchar(g_tmp_uart_rx_buf);
}
