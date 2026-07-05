#ifndef __CUSTOM_DEF_H__
#define __CUSTOM_DEF_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

//#ifndef configTICK_RATE_HZ
//#define	configTICK_RATE_HZ	1000
//#endif

// Helper macro to convert numeric value to string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef __ARMCC_VERSION
    #define COMPILER_NAME "ARMClang " TOSTRING(__ARMCC_VERSION)
#elif defined(__GNUC__) && defined(__ARM_ARCH)
    #define COMPILER_NAME "GCC " __VERSION__
#else
    #define COMPILER_NAME "Unknown Compiler"
#endif

#define SERIAL_UART XMC_UART0_CH0
#define SERIAL_TX_PIN P1_5
#define SERIAL_TX_AF P1_5_AF_U0C0_DOUT0
#define SERIAL_RX_PIN P1_4
#define SERIAL_RX_INPUT USIC0_C0_DX0_P1_4

#define SERIAL_RX_IRQN USIC0_0_IRQn

/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   100
#define IP_ADDR3   11

/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   100
#define GW_ADDR3   1

/* MAC ADDRESS*/
#define MAC_ADDR0   0x00
#define MAC_ADDR1   0x00
#define MAC_ADDR2   0x45
#define MAC_ADDR3   0x19
#define MAC_ADDR4   0x03
#define MAC_ADDR5   0x00

#define XMC_ETH_MAC_NUM_RX_BUF (4)
#define XMC_ETH_MAC_NUM_TX_BUF (8)

#endif
