#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

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
#include "utils.h"

#include "dhry.h"

static uint32_t tmpDts;
static float tmpCel;
static float tmpV13;
static float tmpV33;

uint8_t g_tmp_uart_rx_buf;
	
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

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH0, (uint8_t)ch);
	return ch;
}

extern uint32_t __Vectors;
extern uint32_t __Vectors_End;
extern uint32_t __Vectors_Size;

extern void Proc_5 (void);

[[ noreturn ]] int main(void) {
//	EventRecorderInitialize(EventRecordAll, 1);

	SysTick_Config(SystemCoreClock / configTICK_RATE_HZ);
	
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
	printf("vector: %08X %08X %08X %08X %08X\n", (uint32_t)(&__Vectors), (uint32_t)(&__Vectors_End), (uint32_t)(&__Vectors_Size), (uint32_t)(&dhry_main), (uint32_t)(&Proc_5));

	//T_DTS = (RESULT - 605) / 2.05 [°C]
	tmpDts = XMC_SCU_GetTemperatureMeasurement();
	tmpCel = (tmpDts-605)/2.05;
	printf("%.1f\n", tmpCel);

	tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
	tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
	printf("%.1f %.1f\n", tmpV13, tmpV33);	
							
	XMC_SCU_StartTemperatureMeasurement();		
			
  dhry_main(SystemCoreClock);
				
	while (1) {
		LED_Toggle(0);
		
		//T_DTS = (RESULT - 605) / 2.05 [°C]
		tmpDts = XMC_SCU_GetTemperatureMeasurement();
		tmpCel = (tmpDts-605)/2.05;

		tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		printf("%.1f %.1f %.1f\n", tmpCel, tmpV13, tmpV33);	
							
		XMC_SCU_StartTemperatureMeasurement();	
				
		printf("\n");
		printf("OSCHIPFreq:%u \n", OSCHP_GetFrequency());
		printf("CC: %s %s\n", COMPILER_NAME, __VERSION__);		
		printf("%u Hz, %08X, CM:%d, FPU_USED:%d, SCU_IDCHIP:%08X\n",
				SystemCoreClock, SCB->CPUID,
				__CORTEX_M, __FPU_USED,
				SCU_GENERAL->IDCHIP);
		printf("Boot Mode:%u, FPU type:%u\n", XMC_SCU_GetBootMode(), SCB_GetFPUType());
		printf("vector: %08X %08X %08X\n", (uint32_t)(&__Vectors), (uint32_t)(&__Vectors_End), (uint32_t)(&__Vectors_Size));
				
		HAL_Delay(configTICK_RATE_HZ * 20);
		LED_Toggle(1);
	}
}
