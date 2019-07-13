#include <XMC4700.h>

extern __IO uint32_t g_Ticks;

void SysTick_Handler(void) {	
	g_Ticks++;
}
