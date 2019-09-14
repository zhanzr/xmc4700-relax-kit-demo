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
#include <math.h>
#endif

#include <XMC4700.h>
#include <xmc_scu.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include "RTE_Components.h"

#include "custom_def.h"
#include "led.h"

__IO uint8_t g_tmp_uart_rx_buf;
__IO uint32_t g_Ticks;

uint32_t HAL_GetTick(void) {
	return g_Ticks;
}

void HAL_Delay(uint32_t t) {
	uint32_t d = t + g_Ticks;
	while(d > g_Ticks) {
		__NOP();
	}
}

void XMC_AssertHandler(const char *const msg, const char *const file, uint32_t line) {
	XMC_DEBUG("%s %s %u\n", msg, file, line);
  abort();
}

extern uint32_t __Vectors;

int main(void) {		
  /* System timer configuration */
  SysTick_Config(SystemCoreClock / HZ);
	
	setbuf(stdout, NULL);

	XMC_SCU_EnableTemperatureSensor();
	XMC_SCU_StartTemperatureMeasurement();

	LED_Initialize();
	
	printf("XMC4700 ARMCC Test %u Hz\n", SystemCoreClock);
  printf("OSCHIPFreq:%u \n", OSCHP_GetFrequency());
	
	printf("%u Hz, %08X, CM:%d\n",
			SystemCoreClock,
			SCB->CPUID,
			__CORTEX_M);
					
	printf("Boot Mode:%u, Vector:%08X\n",
	XMC_SCU_GetBootMode(), (uint32_t)(&__Vectors));
	
  #ifdef RTE_Compiler_IO_STDOUT_User
	printf("RTE_Compiler_IO_STDOUT_User\n");
	#endif
	
  #ifdef RTE_Compiler_IO_STDOUT_EVR
	printf("RTE_Compiler_IO_STDOUT_EVR\n");
	#endif

  #ifdef RTE_Compiler_IO_STDOUT_ITM
	printf("RTE_Compiler_IO_STDOUT_ITM\n");
	#endif
		
  #ifdef __FPU_PRESENT
	printf("__FPU_PRESENT = %u\n", __FPU_PRESENT);
	#else
	printf("__FPU_PRESENT NA\n");
	#endif
	
  #ifdef __FPU_USED
	printf("__FPU_USED = %u\n", __FPU_USED);
	#else
	printf("__FPU_USED NA\n");
	#endif
	
  #ifdef __TARGET_FPU_NONE
	printf("__TARGET_FPU_NONE = %u\n", __TARGET_FPU_NONE);
	#else
	printf("__TARGET_FPU_NONE NA\n");
	#endif		
	
  #ifdef __TARGET_FPU_VFP
	printf("__TARGET_FPU_VFP = %u\n", __TARGET_FPU_VFP);
	#else
	printf("__TARGET_FPU_VFP NA\n");
	#endif	
	
  #ifdef __TARGET_FPU_SOFTVFP
	printf("__TARGET_FPU_SOFTVFP = %u\n", __TARGET_FPU_SOFTVFP);
	#else
	printf("__TARGET_FPU_SOFTVFP NA\n");
	#endif		
	
	printf("xmc4700 %p %d\n", &SystemCoreClock, sizeof(double));
			
  while (1) {
		printf("%u %u\n",
		SystemCoreClock,
		HAL_GetTick());
		
	printf("xmc4700 %p %d\n", &SystemCoreClock, sizeof(double));
		
		HAL_Delay(2000);
	}
}
