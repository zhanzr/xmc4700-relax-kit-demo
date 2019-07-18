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

#include "custom_def.h"
#include "led.h"
#include "cmsis_os.h"

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "ethernetif.h"
#include "httpserver/httpserver-netconn.h"

//static uint32_t tmpDts;
//static float tmpCel;
//static float tmpV13;
//static float tmpV33;

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

void ttywrch (int ch) {
	XMC_UART_CH_Transmit(XMC_UART0_CH0, (uint8_t)ch);
}

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   11

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1

struct netif xnetif;

void LWIP_Init(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

  /* Create tcp_ip stack thread */
  tcpip_init( NULL, NULL );

  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
  struct ip_addr *netmask, struct ip_addr *gw,
  void *state, err_t (* init)(struct netif *netif),
  err_t (* input)(struct pbuf *p, struct netif *netif))

  Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/
  netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /*  Registers the default network interface.*/
  netif_set_default(&xnetif);

  /* Set Ethernet link flag */
  xnetif.flags |= NETIF_FLAG_LINK_UP;

  /* When the netif is fully configured this function must be called.*/
  netif_set_up(&xnetif);

}

void led1_task(void const *args) {
  while(1) {
		LED_Toggle(0);
    osDelay(500);
  }
}
osThreadDef(led1_task, osPriorityNormal, 1, 0);

void main_task(void const *args)
{
	LED_Initialize();

  LWIP_Init();
  http_server_netconn_init();

  osThreadCreate(osThread(led1_task), NULL);
}
osThreadDef(main_task, osPriorityNormal, 1, 0);

extern uint32_t __Vectors;

#define TEST_BAUDRATE	(921600)
int main(void) {
  /* System timer configuration */
  SysTick_Config(SystemCoreClock / CLOCKS_PER_SEC);
	
	XMC_SCU_EnableTemperatureSensor();
	XMC_SCU_StartTemperatureMeasurement();
	
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

	printf("Boot Mode:%u, Vector:%08X\n",
			XMC_SCU_GetBootMode(),
			(uint32_t)(&__Vectors));
	
  #ifdef RTE_Compiler_IO_STDOUT_User
	printf("RTE_Compiler_IO_STDOUT_User\n");
	#endif
	
  #ifdef RTE_Compiler_IO_STDOUT_EVR
	printf("RTE_Compiler_IO_STDOUT_EVR\n");
	#endif

  #ifdef RTE_Compiler_IO_STDOUT_ITM
	printf("RTE_Compiler_IO_STDOUT_ITM\n");
	#endif
	
	#ifdef __MICROLIB
	printf("With Microlib\n");
	#else
	printf("With StandardLib\n");
	#endif

  osKernelInitialize();

  osThreadCreate(osThread(main_task), NULL);

  osKernelStart();
}
