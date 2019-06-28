#include <math.h>

#include <errno.h>

#include <xmc4700.h>
#include <xmc_scu.h>

#include <arm_math.h>

#include "EventRecorder.h"

#include "RTE_Components.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "custom_def.h"
#include "Board_LED.h"
#include "Driver_USART.h"

static uint32_t tmpDts;
static float tmpCel;
static float tmpV13;
static float tmpV33;

/* USART Driver */
extern ARM_DRIVER_USART Driver_USART0;
static ARM_DRIVER_USART *UARTdrv = &Driver_USART0; 
uint8_t g_tmp_uart_rx_buf;

void myUART_callback(uint32_t event) {
    switch (event)
    {
    case ARM_USART_EVENT_RECEIVE_COMPLETE:  
//		 if (cmd == 0x0D)        /* Send back the message if Enter is recived*/
//		 {
//			 UARTdrv->Send("\nHello World!\r\n", 15);
//			 cmd = 0;   
//   		 UARTdrv->Receive(&cmd, 1);
//		 }
		   		 
		UARTdrv->Receive(&g_tmp_uart_rx_buf, 1);
		 break;
		 
    case ARM_USART_EVENT_TRANSFER_COMPLETE:
    case ARM_USART_EVENT_SEND_COMPLETE:
    case ARM_USART_EVENT_TX_COMPLETE:
     
        break;
 
    case ARM_USART_EVENT_RX_TIMEOUT:
         __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
        break;
 
    case ARM_USART_EVENT_RX_OVERFLOW:
    case ARM_USART_EVENT_TX_UNDERFLOW:
        __breakpoint(0);  /* Error: Call debugger or replace with custom error handling */
        break;
    }
}
  
int stdout_putchar (int ch) {
	UARTdrv->Send((uint8_t*)&ch, 1);
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

void sine_gen(void) {
  while(1) { 
		xTaskNotifyWait( 0x00,      /* Don't clear any notification bits on entry. */
                         UINT32_MAX, /* Reset the notification value to 0 on exit. */
                         NULL,
                         portMAX_DELAY );  /* Block indefinitely. */

//    sine = sine_calc_sample_q15(&Signal_set) / 2;

//		xTaskNotify( g_noise_gen_task_handle, 0, eNoAction );
  }
}

extern volatile uint32_t test_val32[3];
void sync_tsk(void) {
	TickType_t xLastWakeTime;
  while(1) {			
		
//		xLastWakeTime = xTaskGetTickCount ();    
//		xTaskNotify( g_sine_gen_task_handle, 0, eNoAction );
//    vTaskDelayUntil( &xLastWakeTime, 10 / portTICK_PERIOD_MS );
		
		XMC_SCU_StartTemperatureMeasurement();		
				
		printf("t:%u\n", xTaskGetTickCount());
		//T_DTS = (RESULT - 605) / 2.05 [�C]
		tmpDts = XMC_SCU_GetTemperatureMeasurement();
		tmpCel = (tmpDts-605)/2.05;
		printf("%.1f\n", tmpCel);

		tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		printf("%.1f %.1f\n", tmpV13, tmpV33);	
				
//    vTaskDelay(500 / portTICK_PERIOD_MS);	
    vTaskDelay(100);	
						
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
	UARTdrv->Initialize(myUART_callback);
	/*Power up the UART peripheral */
	UARTdrv->PowerControl(ARM_POWER_FULL);

	UARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
									 ARM_USART_DATA_BITS_8 |
									 ARM_USART_PARITY_NONE |
									 ARM_USART_STOP_BITS_1 , TEST_BAUDRATE);
	 
	/* Enable the Transmitter line */
	UARTdrv->Control (ARM_USART_CONTROL_TX, 1);
	
	printf("XMC4500 ARMCC Test @ %u Hz\n", SystemCoreClock);

	printf("%u Hz, %08X, CM:%d, FPU_USED:%d, SCU_IDCHIP:%08X\n",
			SystemCoreClock, SCB->CPUID,
			__CORTEX_M, __FPU_USED,
			SCU_GENERAL->IDCHIP);
	printf("Boot Mode:%u\n", XMC_SCU_GetBootMode());
	
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
																			
	xTaskCreate((TaskFunction_t)sine_gen,
							(const portCHAR *)"sine_gen",
							256,
							NULL,
							tskIDLE_PRIORITY+2,
							&g_sine_gen_task_handle);							
  printf ("sine_gen Task Initialised\n\r");
							
							xTaskCreate((TaskFunction_t)sync_tsk,
							(const portCHAR *)"sync_tsk",
							256,
							NULL,
							tskIDLE_PRIORITY+3,
							&g_sync_task_handle);							
  printf ("sync_tsk Task Initialised\n\r");
  printf ("Application Running\n\r");
	
  /* Start scheduler */  
	vTaskStartScheduler();
	
//Should never come here
							
  while (1) {
		//T_DTS = (RESULT - 605) / 2.05 [�C]
		tmpDts = XMC_SCU_GetTemperatureMeasurement();
		tmpCel = (tmpDts-605)/2.05;
		printf("%f\n", tmpCel);

		tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		printf("%f %f\n", tmpV13, tmpV33);
		
		printf("%u MHz\n", SystemCoreClock/1000000);

//		tmp_cyccnt = DWT->CYCCNT;
//		tmp_cpicnt = DWT->CPICNT;
//		tmp_exccnt = DWT->EXCCNT;
//		tmp_sleepcnt = DWT->SLEEPCNT;
//		tmp_lsucnt = DWT->LSUCNT;
//		tmp_foldcnt = DWT->FOLDCNT;
		
		//CYCCNT - CPICNT - EXCCNT - SLEEPCNT - LSUCNT + FOLDCNT
//		printf("DWT CYCCNT:%u\n", tmp_cyccnt);
//		printf("DWT CPICNT:%u\n", tmp_cpicnt);
//		printf("DWT EXCCNT:%u\n", tmp_exccnt);
//		printf("DWT SLEEPCNT:%u\n", tmp_sleepcnt);
//		printf("DWT LSUCNT:%u\n", tmp_lsucnt);
//		printf("DWT FOLDCNT:%u\n", tmp_foldcnt);
//		printf("CYCCNT - CPICNT - EXCCNT - SLEEPCNT - LSUCNT + FOLDCNT = %u\n",
//		tmp_cyccnt - tmp_cpicnt -tmp_exccnt - tmp_sleepcnt - tmp_lsucnt + tmp_foldcnt);
		
//		test_div_flash();
//		
//		test_div_sram();
//		test_alloca();

  #ifdef RTE_Compiler_IO_STDOUT_User
	printf("RTE_Compiler_IO_STDOUT_User\n");
	#endif
	
  #ifdef RTE_Compiler_IO_STDOUT_EVR
	printf("RTE_Compiler_IO_STDOUT_EVR\n");
	#endif

  #ifdef RTE_Compiler_IO_STDOUT_ITM
	printf("RTE_Compiler_IO_STDOUT_ITM\n");
	#endif
		
		XMC_SCU_StartTemperatureMeasurement();		
	}
}
