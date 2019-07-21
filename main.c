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
#include "serial.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "ethernetif.h"

#include "EventRecorder.h"

#include "netif/etharp.h"

#include "ethernetif.h"
#include "lwip/apps/httpd.h"

#if LWIP_DHCP == 1
#include <lwip/dhcp.h>
#endif

static uint32_t tmpDts;
static float tmpCel;
static float tmpV13;
static float tmpV33;

#define BUTTON1 P15_13
#define BUTTON2 P15_12

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

/* MAC ADDRESS*/
#define MAC_ADDR0   0x00
#define MAC_ADDR1   0x00
#define MAC_ADDR2   0x45
#define MAC_ADDR3   0x19
#define MAC_ADDR4   0x03
#define MAC_ADDR5   0x00

#define BUTTONS_TMR_INTERVAL 100

#define XMC_ETH_MAC_NUM_RX_BUF (4)
#define XMC_ETH_MAC_NUM_TX_BUF (8)

#if defined(__ICCARM__)
#pragma data_alignment=4
static XMC_ETH_MAC_DMA_DESC_t rx_desc[XMC_ETH_MAC_NUM_RX_BUF] @ ".dram";
#pragma data_alignment=4
static XMC_ETH_MAC_DMA_DESC_t tx_desc[XMC_ETH_MAC_NUM_TX_BUF] @ ".dram";
#pragma data_alignment=4
static uint8_t rx_buf[XMC_ETH_MAC_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] @ ".dram";
#pragma data_alignment=4
static uint8_t tx_buf[XMC_ETH_MAC_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] @ ".dram";
#elif defined(__CC_ARM) || (defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
static __ALIGNED(4) XMC_ETH_MAC_DMA_DESC_t rx_desc[XMC_ETH_MAC_NUM_RX_BUF] __attribute__((section ("RW_IRAM1")));
static __ALIGNED(4) XMC_ETH_MAC_DMA_DESC_t tx_desc[XMC_ETH_MAC_NUM_TX_BUF] __attribute__((section ("RW_IRAM1")));
static __ALIGNED(4) uint8_t rx_buf[XMC_ETH_MAC_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("RW_IRAM1")));
static __ALIGNED(4) uint8_t tx_buf[XMC_ETH_MAC_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("RW_IRAM1")));
#elif defined(__GNUC__)
static __ALIGNED(4) XMC_ETH_MAC_DMA_DESC_t rx_desc[XMC_ETH_MAC_NUM_RX_BUF] __attribute__((section ("ETH_RAM")));
static __ALIGNED(4) XMC_ETH_MAC_DMA_DESC_t tx_desc[XMC_ETH_MAC_NUM_TX_BUF] __attribute__((section ("ETH_RAM")));
static __ALIGNED(4) uint8_t rx_buf[XMC_ETH_MAC_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
static __ALIGNED(4) uint8_t tx_buf[XMC_ETH_MAC_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE] __attribute__((section ("ETH_RAM")));
#endif

static ETHIF_t ethif =
{
  .phy_addr = 0,
  .mac =
  {
    .regs = ETH0,
    .rx_desc = rx_desc,
    .tx_desc = tx_desc,
    .rx_buf = &rx_buf[0][0],
    .tx_buf = &tx_buf[0][0],
    .num_rx_buf = XMC_ETH_MAC_NUM_RX_BUF,
    .num_tx_buf = XMC_ETH_MAC_NUM_TX_BUF
  },
  .phy =
  {
    .interface = XMC_ETH_LINK_INTERFACE_RMII,
    .enable_auto_negotiate = true,
  }
};

static struct netif xnetif = 
{
  /* set MAC hardware address length */
  .hwaddr_len = (u8_t)ETHARP_HWADDR_LEN,

  /* set MAC hardware address */
  .hwaddr =  {(u8_t)MAC_ADDR0, (u8_t)MAC_ADDR1,
              (u8_t)MAC_ADDR2, (u8_t)MAC_ADDR3,
              (u8_t)MAC_ADDR4, (u8_t)MAC_ADDR5},
};


void LWIP_Init(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

#if LWIP_DHCP == 0
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#else
  ipaddr.addr = 0;
  netmask.addr = 0;
  gw.addr = 0;
#endif

  /* Create tcp_ip stack thread */
  tcpip_init( NULL, NULL );

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
  netif_add(&xnetif, &ipaddr, &netmask, &gw, &ethif, &ethernetif_init, &tcpip_input);
}

int8_t bx = 0;
static void buttons_task(void *arg)
{
  XMC_UNUSED_ARG(arg);

  while(1)
  {
    if (XMC_GPIO_GetInput(BUTTON1) != 0)
    {
      bx++;
    }

    if (XMC_GPIO_GetInput(BUTTON2) != 0)
    {
      bx--;
    }

    sys_arch_msleep(BUTTONS_TMR_INTERVAL);
  }
}

/* Initialisation of functions to be used with CGi*/
//  CGI handler to switch LED status
const char *ledcontrol_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  XMC_UNUSED_ARG(iIndex);
  XMC_UNUSED_ARG(iNumParams);
  XMC_UNUSED_ARG(pcParam);

  if(strcmp(pcValue[0], "led1") == 0) {
    LED_Toggle(0);
  } else {
    LED_Toggle(1);
		
		XMC_SCU_StartTemperatureMeasurement();		
		
		printf("t:%u\n", xTaskGetTickCount());
		//T_DTS = (RESULT - 605) / 2.05 [°C]
		tmpDts = XMC_SCU_GetTemperatureMeasurement();
		tmpCel = (tmpDts-605)/2.05;
		printf("%.1f\n", tmpCel);

		tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		printf("%.1f %.1f\n", tmpV13, tmpV33);			
  }
  return "/cgi.htm";
}

const char *data_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
  XMC_UNUSED_ARG(iIndex);
  XMC_UNUSED_ARG(iNumParams);
  XMC_UNUSED_ARG(pcParam);
  XMC_UNUSED_ARG(pcValue);

  return "/data.ssi";
}

tCGI led_handler_struct[] =
{
  {
    .pcCGIName = "/ledcontrol.cgi",
    .pfnCGIHandler = ledcontrol_handler
  },
  {
   .pcCGIName = "/data.cgi",
   .pfnCGIHandler = data_handler
  }
};

int cgi_init(void)
{
  http_set_cgi_handlers(led_handler_struct, 2);

  return 0;
}

/**
 * Initialize SSI handlers
 */
const char *TAGS[]={"bx"};

static uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen)
{
  XMC_UNUSED_ARG(iIndex);
  XMC_UNUSED_ARG(iInsertLen);

  return (sprintf(pcInsert, "%d", bx));
}

void ssi_init(void)
{
  http_set_ssi_handler(ssi_handler, (char const **)TAGS, 1);
}

#if LWIP_NETIF_STATUS_CALLBACK == 1
/* The status callback will be called anytime the interface is brought up and down. 
   For example, if you would like to log the IP address chosen by DHCP when it has got one, 
   and know when this address changes, then you can use the status callback hook.
   The function being called is netif_status_callback */

void netif_status_cb(struct netif *netif)
{
#if LWIP_DHCP
  if (dhcp_supplied_address(netif) > 0)
  {
    printf("Got IP:%s\r\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    
    /* Initialize HTTP server */
    httpd_init();
    cgi_init();
    ssi_init();
  }
#else
  if (netif_is_up(netif))
  {
    /* Initialize HTTP server */
    httpd_init();
    cgi_init();
    ssi_init();
  }
#endif
}
#endif

void vApplicationDaemonTaskStartupHook(void) {
  XMC_GPIO_CONFIG_t config;

  config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;

  XMC_GPIO_Init(BUTTON1, &config);
  XMC_GPIO_Init(BUTTON2, &config);

	LED_Initialize();

  LWIP_Init();

  sys_thread_new("buttons_task", buttons_task, NULL, configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY);
}

extern uint32_t __Vectors;

#define TEST_BAUDRATE	(921600)
int main(void) {
  EventRecorderInitialize(EventRecordAll, 1);
  serial_init();

	XMC_SCU_EnableTemperatureSensor();
	XMC_SCU_StartTemperatureMeasurement();
	
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

	XMC_SCU_StartTemperatureMeasurement();		
	
	printf("t:%u\n", xTaskGetTickCount());
	//T_DTS = (RESULT - 605) / 2.05 [°C]
	tmpDts = XMC_SCU_GetTemperatureMeasurement();
	tmpCel = (tmpDts-605)/2.05;
	printf("%.1f\n", tmpCel);

	tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
	tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
	printf("%.1f %.1f\n", tmpV13, tmpV33);	
	
  /* Start scheduler */  
	vTaskStartScheduler();
	
	//Should never come here
	while(1) {
		__NOP();
	}
}
