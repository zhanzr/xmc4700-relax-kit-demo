#ifdef __cplusplus
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include "LiquidCrystal.h"

using namespace std;
#else
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#endif

#include <XMC4700.h>
#include <xmc_scu.h>
#include <xmc_rtc.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>
#include <xmc_flash.h>
#include <xmc_vadc.h>

#include "RTE_Components.h"

#include "FreeRTOS.h"
#include "task.h"

#include "led.h"
#include "EventRecorder.h"

static uint32_t tmpDts;
static float tmpCel;
static float tmpV13;
static float tmpV33;

uint8_t g_tmp_uart_rx_buf;
  
#ifndef configTICK_RATE_HZ
#define	configTICK_RATE_HZ	1000
#endif

#define UART_RX P1_4
#define UART_TX P1_5

XMC_GPIO_CONFIG_t uart_tx;
XMC_GPIO_CONFIG_t uart_rx;

/* UART configuration */
const XMC_UART_CH_CONFIG_t uart_config = {	
	.baudrate = 921600,
	.data_bits = 8U,
	.frame_length = 8U,
	.stop_bits = 1U,
	.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};

int stdout_putchar(int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH0, (uint8_t)ch);
	return ch;
}

TaskHandle_t g_sync_task_handle;

void sync_tsk(void) {
  while(1) {							
		printf("t:%u\n", xTaskGetTickCount());
		
		#ifdef tskKERNEL_VERSION_NUMBER
		printf("OS Ver:%s\n", tskKERNEL_VERSION_NUMBER);
		#else
		printf("CC Ver:%u\n", __ARMCC_VERSION);		
		#endif
		
    vTaskDelay(500 / portTICK_PERIOD_MS);	
						
		LED_Toggle(0);
		LED_Toggle(1);
  }
}

void vApplicationIdleHook( void ) {
//		XMC_UART_CH_Transmit(XMC_UART0_CH0, '#');
}

void vApplicationTickHook( void ) {	
//	XMC_UART_CH_Transmit(XMC_UART0_CH0, '$');
}

int main(void) {
  EventRecorderInitialize(EventRecordAll, 1);
	
	LED_Initialize();
	
	/*Initialize the UART driver */
	uart_tx.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;
	uart_rx.mode = XMC_GPIO_MODE_INPUT_TRISTATE;
 /* Configure UART channel */
  XMC_UART_CH_Init(XMC_UART0_CH0, &uart_config);
  XMC_UART_CH_SetInputSource(XMC_UART0_CH0, XMC_UART_CH_INPUT_RXD,USIC0_C0_DX0_P1_4);
  
	/* Start UART channel */
  XMC_UART_CH_Start(XMC_UART0_CH0);

  /* Configure pins */
	XMC_GPIO_Init(UART_TX, &uart_tx);
  XMC_GPIO_Init(UART_RX, &uart_rx);
	  
	printf ("XMC47 Relax @%u Hz %08X\n",
	SystemCoreClock,
	SCU_GENERAL->IDCHIP	);
	
	xTaskCreate((TaskFunction_t)sync_tsk,
							(const portCHAR *)"sync_tsk",
							256,
							NULL,
							configMAX_PRIORITIES-1,
							&g_sync_task_handle);							
								
  /* Start scheduler */  
	vTaskStartScheduler();
	
//Should never come here					
  while (1) {
		__BKPT(0x99);
	}
}
