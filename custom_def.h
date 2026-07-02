#ifndef __CUSTOM_DEF_H__
#define __CUSTOM_DEF_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef HZ
#define	HZ	1000
#endif

#define	TEST_LOOP_N	10000

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

#define SERIAL_BAUDRATE 921600

#define SERIAL_UART XMC_UART0_CH0
#define SERIAL_TX_PIN P1_5
#define SERIAL_TX_AF P1_5_AF_U0C0_DOUT0
#define SERIAL_RX_PIN P1_4
#define SERIAL_RX_INPUT USIC0_C0_DX0_P1_4

#define SERIAL_RX_IRQN USIC0_0_IRQn

#endif
