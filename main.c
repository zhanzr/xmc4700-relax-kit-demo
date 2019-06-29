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

#include <arm_math.h>

#include "EventRecorder.h"

#include "RTE_Components.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

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
	.baudrate = 921600,
	.data_bits = 8U,
	.frame_length = 8U,
	.stop_bits = 1U,
	.parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
};

int stdout_putchar (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH0, (uint8_t)ch);
	return ch;
}

TaskHandle_t g_filter_task_handle;
TaskHandle_t g_disturb_gen_task_handle;
TaskHandle_t g_noise_gen_task_handle;
TaskHandle_t g_sine_gen_task_handle;
TaskHandle_t g_sync_task_handle;

#define ENABLE_CONFIG 1
// =============================
//   <o>Oscillator Sampling Frequency [Hz] <1000-10000>
//   <i> Set the oscillator sampling frequency.
//   <i> Default: 5000  (5 KHz)
#define SAMPLING_FREQ 1000  // generating task (5 KHz)

//   <o>Noise Frequency [Hz] <50-10000>
//   <i> Set the noise signal frequency.
//   <i> Default: 1500 Hz
#define NOISE_FREQ    50  // noise (2 KHz)

//   <o>Signal Frequency [Hz] <10-1000>
//   <i> Set the signal frequency.
//   <i> Default: 330 Hz
#define SIGNAL_FREQ    10  // disturbed signal (250 Hz)

// </e>
//------------- <<< end of configuration section >>> -----------------------


//sine_generator_q15_t Signal_set;
//sine_generator_q15_t Noise_set;

q15_t sine;
q15_t noise;
q15_t disturbed;
q15_t filtered;

//void sine_gen(void) {
//  while(1) { 
//		xTaskNotifyWait( 0x00,      /* Don't clear any notification bits on entry. */
//                         UINT32_MAX, /* Reset the notification value to 0 on exit. */
//                         NULL,
//                         portMAX_DELAY );  /* Block indefinitely. */

////    sine = sine_calc_sample_q15(&Signal_set) / 2;

////		xTaskNotify( g_noise_gen_task_handle, 0, eNoAction );
//  }
//}

extern volatile uint32_t test_val32[3];
void sync_tsk(void) {
	TickType_t xLastWakeTime;
  while(1) {			
		
//		xLastWakeTime = xTaskGetTickCount ();    
//		xTaskNotify( g_sine_gen_task_handle, 0, eNoAction );
//    vTaskDelayUntil( &xLastWakeTime, 10 / portTICK_PERIOD_MS );
		
		XMC_SCU_StartTemperatureMeasurement();		
				
		printf("t:%u\n", xTaskGetTickCount());
		//T_DTS = (RESULT - 605) / 2.05 [°C]
		tmpDts = XMC_SCU_GetTemperatureMeasurement();
		tmpCel = (tmpDts-605)/2.05;
		printf("%.1f\n", tmpCel);

		tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		printf("%.1f %.1f\n", tmpV13, tmpV33);	
		
		#ifdef tskKERNEL_VERSION_NUMBER
		printf("OS Ver:%s\n", tskKERNEL_VERSION_NUMBER);
		#else
		printf("CC Ver:%u\n", __ARMCC_VERSION);		
		#endif
		
    vTaskDelay(500 / portTICK_PERIOD_MS);	
						
		LED_Toggle(0);
		LED_Toggle(1);
//		xTaskNotify( g_sine_gen_task_handle, 0, eNoAction );
  }
}

#define TEST_BAUDRATE	(921600)
int main(void) {
  EventRecorderInitialize(EventRecordAll, 1);

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

	printf("%u Hz, %08X, CM:%d, FPU_USED:%d, SCU_IDCHIP:%08X\n",
			SystemCoreClock, SCB->CPUID,
			__CORTEX_M, __FPU_USED,
			SCU_GENERAL->IDCHIP);
	printf("Boot Mode:%u\n", XMC_SCU_GetBootMode());
	
  #ifdef RTE_Compiler_IO_STDOUT_User
	printf("RTE_Compiler_IO_STDOUT_User\n");
	#endif
	
  #ifdef RTE_Compiler_IO_STDOUT_EVR
	printf("RTE_Compiler_IO_STDOUT_EVR\n");
	#endif

  #ifdef RTE_Compiler_IO_STDOUT_ITM
	printf("RTE_Compiler_IO_STDOUT_ITM\n");
	#endif
	
//	  // compute coefficients for IIR sine generators
//  sine_generator_init_q15(&Signal_set, SIGNAL_FREQ, SAMPLING_FREQ);
//  sine_generator_init_q15(&Noise_set, NOISE_FREQ, SAMPLING_FREQ);
//  printf ("Sine Generator Initialised\n\r");

  // initialize low pass filter
//  low_pass_filter_init();
//  printf ("Low Pass Filter Initialised\n\r");

  // initialize the timing system to activate the four tasks 
  // of the application program
//		xTaskCreate((TaskFunction_t)filter_tsk,
//							(const portCHAR *)"filter_tsk",
//							384,
//							NULL,
//							2,
//							&g_filter_task_handle);							
//	printf ("filter_tsk Task Initialised\n\r");

//		xTaskCreate((TaskFunction_t)disturb_gen,
//							(const portCHAR *)"disturb_gen",
//							384,
//							NULL,
//							2,
//							&g_disturb_gen_task_handle);
//	printf ("disturb_gen Task Initialised\n\r");
//								xTaskCreate((TaskFunction_t)noise_gen,
//							(const portCHAR *)"noise_gen",
//							384,
//							NULL,
//							2,
//							&g_noise_gen_task_handle);
//  printf ("noise_gen Task Initialised\n\r");
																			
//	xTaskCreate((TaskFunction_t)sine_gen,
//							(const portCHAR *)"sine_gen",
//							256,
//							NULL,
//							tskIDLE_PRIORITY+2,
//							&g_sine_gen_task_handle);							
//  printf ("sine_gen Task Initialised\n\r");
							
							xTaskCreate((TaskFunction_t)sync_tsk,
							(const portCHAR *)"sync_tsk",
							256,
							NULL,
							tskIDLE_PRIORITY+3,
							&g_sync_task_handle);							
//  printf ("sync_tsk Task Initialised\n\r");
//  printf ("Application Running\n\r");
								
  /* Start scheduler */  
	vTaskStartScheduler();
	
//Should never come here
							
  while (1) {
		__BKPT(0x99);
	}
}
