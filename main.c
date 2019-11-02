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

#include "RTE_Components.h"
#include "EventRecorder.h"

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
#include "netif/etharp.h"
#include "ethernetif.h"

#include "lwip/apps/httpd.h"

#if LWIP_DHCP == 1
#include <lwip/dhcp.h>
#endif

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

#define PLOT_VALUE_UPDATE_INTERVAL 800

#define XMC_ETH_MAC_NUM_RX_BUF (4)
#define XMC_ETH_MAC_NUM_TX_BUF (8)

static XMC_ETH_MAC_DMA_DESC_t rx_desc[XMC_ETH_MAC_NUM_RX_BUF];
static XMC_ETH_MAC_DMA_DESC_t tx_desc[XMC_ETH_MAC_NUM_TX_BUF];
static uint8_t rx_buf[XMC_ETH_MAC_NUM_RX_BUF][XMC_ETH_MAC_BUF_SIZE];
static uint8_t tx_buf[XMC_ETH_MAC_NUM_TX_BUF][XMC_ETH_MAC_BUF_SIZE];

static ETHIF_t ethif = {
  .phy_addr = 0,
  
	.mac = {
    .regs = ETH0,
    .rx_desc = rx_desc,
    .tx_desc = tx_desc,
    .rx_buf = &rx_buf[0][0],
    .tx_buf = &tx_buf[0][0],
    .num_rx_buf = XMC_ETH_MAC_NUM_RX_BUF,
    .num_tx_buf = XMC_ETH_MAC_NUM_TX_BUF
  },
	
  .phy = {
    .interface = XMC_ETH_LINK_INTERFACE_RMII,
    .enable_auto_negotiate = true,
  }
};

static struct netif xnetif = {
  /* set MAC hardware address length */
  .hwaddr_len = (u8_t)ETHARP_HWADDR_LEN,

  /* set MAC hardware address */
  .hwaddr =  {(u8_t)MAC_ADDR0, (u8_t)MAC_ADDR1,
              (u8_t)MAC_ADDR2, (u8_t)MAC_ADDR3,
              (u8_t)MAC_ADDR4, (u8_t)MAC_ADDR5},
};

void LWIP_Init(void) {
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

int8_t g_plot_value_i8;
static void plot_value_task(void *arg) {
  XMC_UNUSED_ARG(arg);

  while(1) {
//    if (XMC_GPIO_GetInput(BUTTON1) != 0) {
//      g_plot_value_i8++;
//    } else if (XMC_GPIO_GetInput(BUTTON2) != 0) {
//      g_plot_value_i8--;
//    } else {
//			;
//		}
		XMC_SCU_StartTemperatureMeasurement();		
		
//		printf("t:%u\n", xTaskGetTickCount());
		
		float tmp_V13 = XMC_SCU_POWER_GetEVR13Voltage();
		float tmp_V33 = XMC_SCU_POWER_GetEVR33Voltage();
//		printf("%.1f %.1f\n", tmp_V13, tmp_V33);	
		
		//T_DTS = (RESULT - 605) / 2.05 [�C]
		uint32_t raw_dts_sample = XMC_SCU_GetTemperatureMeasurement();
		float dts_cel_f32 = (raw_dts_sample-605)/2.05f;
//		printf("%.2f\n", dts_cel_f32);
		g_plot_value_i8 = (int8_t)dts_cel_f32;
		
    sys_arch_msleep(PLOT_VALUE_UPDATE_INTERVAL);
  }
}

/* Initialisation of functions to be used with CGi*/
//  CGI handler to switch LED status
const char *ledcontrol_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
  XMC_UNUSED_ARG(iIndex);
  XMC_UNUSED_ARG(iNumParams);
  XMC_UNUSED_ARG(pcParam);

  if(strcmp(pcValue[0], "led0") == 0) {
    LED_Toggle(0);
  } else if(strcmp(pcValue[0], "led1") == 0) {
    LED_Toggle(1);
  } else if(strcmp(pcValue[0], "led2") == 0) {
    LED_Toggle(2);
  } else if(strcmp(pcValue[0], "voltage") == 0) {
		float tmp_V13 = XMC_SCU_POWER_GetEVR13Voltage();
		float tmp_V33 = XMC_SCU_POWER_GetEVR33Voltage();
		XMC_DEBUG("%.1f %.1f\n", tmp_V13, tmp_V33);	
	} else if(strcmp(pcValue[0], "dump_phy") == 0) {
		dump_phy_reg(&xnetif);
	} else if(strcmp(pcValue[0], "get_dts") == 0) {
		XMC_SCU_StartTemperatureMeasurement();		
		XMC_DEBUG("t:%u\t", xTaskGetTickCount());
		//T_DTS = (RESULT - 605) / 2.05 [�C]
		uint32_t raw_dts_sample = XMC_SCU_GetTemperatureMeasurement();
		float dts_cel_f32 = (raw_dts_sample-605)/2.05f;
		XMC_DEBUG("%.2f\n", dts_cel_f32);	
  } else {
		XMC_ASSERT("no implentmation", 0);
	}
  return "/cgi.htm";
}

const char *data_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
  XMC_UNUSED_ARG(iIndex);
  XMC_UNUSED_ARG(iNumParams);
  XMC_UNUSED_ARG(pcParam);
  XMC_UNUSED_ARG(pcValue);

  return "/data.ssi";
}

tCGI led_handler_struct[] = {
  {
    .pcCGIName = "/ledcontrol.cgi",
    .pfnCGIHandler = ledcontrol_handler
  },
	
  {
   .pcCGIName = "/data.cgi",
   .pfnCGIHandler = data_handler
  }
};

int cgi_init(void) {
  http_set_cgi_handlers(led_handler_struct, 2);

  return 0;
}

/**
 * Initialize SSI handlers
 */
const char *TAGS[]={"bx"};

static uint16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
  XMC_UNUSED_ARG(iIndex);
  XMC_UNUSED_ARG(iInsertLen);

  return (sprintf(pcInsert, "%d", g_plot_value_i8));
}

void ssi_init(void) {
  http_set_ssi_handler(ssi_handler, (char const **)TAGS, 1);
}

#if LWIP_NETIF_STATUS_CALLBACK == 1
/* The status callback will be called anytime the interface is brought up and down. 
   For example, if you would like to log the IP address chosen by DHCP when it has got one, 
   and know when this address changes, then you can use the status callback hook.
   The function being called is netif_status_callback */

void netif_status_cb(struct netif *netif) {
#if LWIP_DHCP
  if (dhcp_supplied_address(netif) > 0) {
    printf("Got IP:%s\r\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    
    /* Initialize HTTP server */
    httpd_init();
    cgi_init();
    ssi_init();
  }
#else
  if (netif_is_up(netif)) {
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

  sys_thread_new("plot_value_task", plot_value_task, NULL, configMINIMAL_STACK_SIZE, tskIDLE_PRIORITY);
}

extern uint32_t __Vectors;

extern uint32_t __initial_sp;
extern uint32_t Stack_Size;
extern uint32_t __heap_base;
extern uint32_t __heap_limit;

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

	//Discard the first DTS sample value
	uint32_t tmp_dts = XMC_SCU_GetTemperatureMeasurement();

	float tmp_V13 = XMC_SCU_POWER_GetEVR13Voltage();
	float tmp_V33 = XMC_SCU_POWER_GetEVR33Voltage();
	printf("%.1f %.1f\n", tmp_V13, tmp_V33);	
	
	printf("Rx_desc: %p size:%u \n", rx_desc, sizeof(rx_desc));
	printf("Tx_desc: %p size:%u \n", tx_desc, sizeof(tx_desc));
	printf("Rx_buf: %p size:%u \n", rx_buf, sizeof(rx_buf));
	printf("Tx_buf: %p size:%u \n", tx_buf, sizeof(tx_buf));

	printf("__initial_sp: %08X \n", (uint32_t)&__initial_sp);
	printf("Stack_Size: %08X \n", (uint32_t)&Stack_Size);
	printf("__heap_base: %08X \n", (uint32_t)&__heap_base);
	printf("__heap_limit: %08X \n", (uint32_t)&__heap_limit);
	
  /* Start scheduler */  
	vTaskStartScheduler();
	
	//Should never come here
	while(1) {
		__NOP();
	}
}
