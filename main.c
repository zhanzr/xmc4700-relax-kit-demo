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

//#include "EventRecorder.h"

#include "RTE_Components.h"

#include "custom_def.h"
#include "led.h"

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
	.baudrate = SERIAL_BAUDRATE,
	.data_bits = 8U,
	.frame_length = 8U,
	.stop_bits = 1U,
	.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};


uint32_t g_ticks;

uint32_t HAL_GetTick(void) {
	return g_ticks;
}

void HAL_Delay(uint32_t t) {
	uint32_t d = t + g_ticks;
	while(d > g_ticks) {
		__NOP();
	}
}

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH0, (uint8_t)ch);
	return ch;
}

extern volatile uint32_t test_val32[3];


int main(void) {
//	EventRecorderInitialize(EventRecordAll, 1);

	SysTick_Config(SystemCoreClock / HZ);
	
	XMC_SCU_EnableTemperatureSensor();
	XMC_SCU_StartTemperatureMeasurement();

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
	
	printf("XMC4700 ARMCC Test @ %u Hz\n", SystemCoreClock);
	
	printf("CC: %s\n", COMPILER_NAME);		

	printf("%u Hz, %08X, CM:%d, FPU_USED:%d, SCU_IDCHIP:%08X\n",
			SystemCoreClock, SCB->CPUID,
			__CORTEX_M, __FPU_USED,
			SCU_GENERAL->IDCHIP);
	printf("Boot Mode:%u\n", XMC_SCU_GetBootMode());
	
		
	//T_DTS = (RESULT - 605) / 2.05 [°C]
	tmpDts = XMC_SCU_GetTemperatureMeasurement();
	tmpCel = (tmpDts-605)/2.05;
	printf("%.1f\n", tmpCel);

	tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
	tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
	printf("%.1f %.1f\n", tmpV13, tmpV33);	
						
	LED_Toggle(0);
	
	XMC_SCU_StartTemperatureMeasurement();		
								
	while (1) {
		//T_DTS = (RESULT - 605) / 2.05 [°C]
		tmpDts = XMC_SCU_GetTemperatureMeasurement();
		tmpCel = (tmpDts-605)/2.05;
		printf("%.1f\n", tmpCel);

		tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		printf("%.1f %.1f\n", tmpV13, tmpV33);	
							
		LED_Toggle(0);
		
		XMC_SCU_StartTemperatureMeasurement();	
		
		HAL_Delay(HZ * 2);
		LED_Toggle(1);
	}
}
