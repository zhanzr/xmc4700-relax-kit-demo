#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <arm_math.h>

#include "utilis.h"

#include <XMC4700.h>
#include <xmc_scu.h>
#include <xmc_fce.h>

/*Data Packet 1 */
int8_t usecase1_Data1[] = "Lorem ipsum dolor sit amet, ";
/*Data Packet 2 */
int8_t usecase1_Data2[] = "consectetur adipiscing elit. Donec metus eros, a";
/*Data Packet 3 */
int8_t usecase1_Data3[] =
    "ccumsan ut vestibulum id, suscipit nec augue. Aenean et lectus ut libero "
    "lacinia faucibus ut vel lectus. Pellentesque iaculis libero ac lectus "
    "blandit accumsan. Pellentesque at nulla eget metus aliquam tincidunt. "
    "Aenean cursus purus vitae lacus cursus pharetra. In hac habitasse platea "
    "dictumst. Curabitur nulla odio, porttitor eu pharetra at, pulvinar "
    "gravida velit. Aenean eu dapibus purus. Proin blandit feugiat urna, at "
    "iaculis elit accumsan ut. Pellentesque molestie pharetra erat, eget "
    "semper urna vehicula nec. Nam tristique sollicitudin diam, faucibus "
    "aliquet velit pharetra a. Duis a placerat risus. Phasellus vel diam nibh, "
    "quis elementum nisl. Phasellus lectus quam, mollis eu consequat ut, "
    "bibendum eget arcu. Nullam at felis a elit auctor suscipit eu quis ipsum. "
    "Mauris luctus, diam sit amet iaculis malesuada, urna orci convallis "
    "tellus, vitae molestie diam justo sed metus. Etiam volutpat volutpat "
    "justo, vel facilisis mi eleifend ut. Aenean egestas, sem eu vulputate "
    "lacinia, odio ligula mollis risus, a semper eros risus sed arcu. Sed "
    "feugiat augue eget erat bibendum vitae consequat purus tempus. Morbi "
    "lobortis nunc eget ligula vehicula non pharetra dolor commodo. "
    "Pellentesque ligula nibh, eleifend blandit aliquam vel, euismod non "
    "tellus. Quisque dictum laoreet feugiat. Maecenas a varius sapien. Ut "
    "semper nulla id turpis cursus ornare. Nullam quis erat et augue imperdiet "
    "pharetra nec a sem. Vestibulum ante ipsum primis in faucibus orci luctus "
    "et ultrices posuere cubilia Curae; Aenean feugiat, orci ultricies "
    "pellentesque viverra, nisl elit molestie augue, et scelerisque risus "
    "felis nec nulla. Maecenas congue arcu ac lectus bibendum at lacinia elit "
    "tristique. Cras fringilla vestibulum lectus. Praesent quis nisi turpis, "
    "sed tristique sem. Nam adipiscing posuere faucibus. In iaculis placerat "
    "semper. Curabitur in nunc quis enim vehicula aliquam quis at sapien. In "
    "hac habitasse platea orci aliquam...";

int8_t usecase1_Data4[] = "abcd";

uint8_t test_vec5[]= {0xAA, 0xBB }; // 0xF90A

/* FCE configuration for CRC32 operation using Kernal 0
 * Algorithm: IR Byte Wise Reflection disabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC enabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t g_FCE_0 = {.kernel_ptr =
                              FCE_KE0, /**< FCE Kernel Pointer */
                          .fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
                          .fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
                          .fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
                          .seedvalue = 0U};

/* FCE configuration for CRC32 operation using Kernal 1
 * Algorithm: IR Byte Wise Reflection disabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC disabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t g_FCE_1 = {.kernel_ptr =
                             FCE_KE1, /**< FCE Kernel Pointer */
                         .fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
                         .fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
                         .fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
                         .seedvalue = 0xFFFFFFFFU};

/* FCE configuration for CRC16 operation using Kernal 2
 * Algorithm: IR Byte Wise Reflection enabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC disabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t g_FCE_2 = {.kernel_ptr = FCE_KE2, /**< FCE Kernel Pointer */
                         .fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
                         .fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
                         .fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
                         .seedvalue = 0xFFFFU};

/* FCE configuration for CRC8 operation using Kernal 3
 * Algorithm: IR Byte Wise Reflection disabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC disabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t g_FCE_3 = {.kernel_ptr = FCE_KE3, /**< FCE Kernel Pointer */
                         .fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
                         .fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
                         .fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
                         .seedvalue = 0U};

/**
  * @brief  This function is executed in case of Error_Handler occurrence.
  * @retval None
  */
static void Error_Handler(void) {
  printf("%s %d\n", __func__, __LINE__);

  while (1) {
    /* LED2 is blinking */
    // BSP_LED_Toggle(LED2);
    HAL_Delay(500);
  }
}

/** \brief This function calculates CRC.
*
* \param[in] length number of bytes in buffer
* \param[in] data pointer to data for which CRC should be calculated
* \param[out] crc pointer to 16-bit CRC
*/
void crc16_algorithm_1(uint8_t length, uint8_t *data, uint16_t *crc) {
  uint8_t counter;
  uint16_t crc_register = 0;
  // uint16_t crc_register = 0xffff;
	
#define POLY 0x1021
//#define POLY 0x8005

  uint8_t shift_register;
  uint8_t data_bit, crc_bit;
  for (counter = 0; counter < length; counter++) {
    for (shift_register = 0x01; shift_register > 0x00; shift_register <<= 1) {
      data_bit = (data[counter] & shift_register) ? 1 : 0;
      crc_bit = crc_register >> 15;
      crc_register <<= 1;
      if (data_bit != crc_bit)
        crc_register ^= POLY;
    }
  }
  *crc = crc_register;
}


unsigned short crc16_algorithm_2(uint16_t CRC_acc, uint8_t* p_input, uint32_t len) {
#define POLY 0x1021

    uint16_t CRC_acc_cal = CRC_acc;
  for(uint32_t idx = 0; idx < len; ++idx) {
      // Create the CRC "dividend" for polynomial arithmetic (binary arithmetic
      // with no carries)
      CRC_acc_cal = CRC_acc_cal ^ (p_input[idx] << 8);
      // "Divide" the poly into the dividend using CRC XOR subtraction
      // CRC_acc_cal holds the "remainder" of each divide
      //
      // Only complete this division for 8 bits since input is 1 byte
      for (uint8_t i = 0; i < 8; i++) {
        // Check if the MSB is set (if MSB is 1, then the POLY can "divide"
        // into the "dividend")
        if ((CRC_acc_cal & 0x8000) == 0x8000) {
          // if so, shift the CRC value, and XOR "subtract" the poly
          CRC_acc_cal = CRC_acc_cal << 1;
          CRC_acc_cal ^= POLY;
        } else {
          // if not, just shift the CRC value
          CRC_acc_cal = CRC_acc_cal << 1;
        }
      }
  }

  return CRC_acc_cal;
}


void crc_perfmance_test(void) {
  uint32_t start_ticks;
  uint32_t end_ticks;

  uint32_t Read_CRCResult32;
  uint16_t Read_CRCResult16;
  uint8_t Read_CRCResult8;
  uint32_t temp_length;
  uint32_t temp_mismatch;
  uint32_t CRC_result;
  bool flagstatus;
  XMC_FCE_STATUS_t fce_status;

  /* Enable FCE module */
  XMC_FCE_Enable();

  /* Initialize the FCE Configuration */
//	XMC_FCE_Init(&FCE_config0);
  XMC_FCE_Init(&g_FCE_1);
  XMC_FCE_Init(&g_FCE_2);
  XMC_FCE_Init(&g_FCE_3);

  temp_mismatch = 0;

  //	/* Step 1: Performs a CRC32 check using Kernel 0 on Usecase1_Data1
  //	 * Seed value is set to 0.
  //	 * => CRC = 0xbb8d49a6, RES = 0x4472b659
  //	 */

  XMC_FCE_t *p_fce;

  p_fce = &g_FCE_0;
	
	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
	p_fce->seedvalue = 0;
	
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
	p_fce->seedvalue = 0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
	p_fce->seedvalue = 0;								 
	XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
	p_fce->seedvalue = 0;								 
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
	p_fce->seedvalue = 0;								 
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
	p_fce->seedvalue = 0;								 
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
	p_fce->seedvalue = 0;								 
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
	p_fce->seedvalue = 0;								 
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
	p_fce->seedvalue = UINT32_MAX;
	XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

	p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
	p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_SET;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data1,
                         strlen((const char *)(usecase1_Data1)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X %08X\n", CRC_result, Read_CRCResult32);

  printf("\n");
												 
	CRC_result = mss_ethernet_crc((uint8_t*)usecase1_Data1, strlen((const char *)(usecase1_Data1)));
  printf("Soft CRC32 IEEE802.3 Result: %08X\n", CRC_result);
  printf("\n");
												 
												 p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);

  p_fce = &g_FCE_0;
  XMC_FCE_Init(p_fce);
  XMC_FCE_CalculateCRC32(p_fce, (uint32_t *)usecase1_Data4,
                         strlen((const char *)(usecase1_Data4)),
                         &Read_CRCResult32);
  XMC_FCE_GetCRCResult(p_fce, &CRC_result);
  printf("Result: %08X\n", CRC_result);
  printf("\n");
  /* Step 2: Performs a CRC32 check using Kernel 1 on Usecase1_Data1
   * Seed value is set to 0. CRC check comparison is enabled
   * CRC checksum is using result from earlier CRC check
   * No CRC mismatch found.
   * CRC = 0xbb8d49a6, RES = 0xbb8d49a6
   */
  XMC_FCE_EnableOperation(&g_FCE_1, XMC_FCE_CFG_CONFIG_CCE);

  /* Used in 32bit FCE, therefore temp_length is divided by 4*/
  temp_length = (strlen((char *)(usecase1_Data1))) >> 2;
  XMC_FCE_UpdateCRCCheck(&g_FCE_1, Read_CRCResult32);
  XMC_FCE_UpdateLength(&g_FCE_1, temp_length);

  XMC_FCE_InitializeSeedValue(&g_FCE_1, 0);
  fce_status = XMC_FCE_CalculateCRC32(&g_FCE_1, (uint32_t *)usecase1_Data1,
                                      strlen((const char *)(usecase1_Data1)),
                                      &Read_CRCResult32);
  while (fce_status == XMC_FCE_STATUS_ERROR) {

    Error_Handler();
  }

  XMC_FCE_GetCRCResult(&g_FCE_1, &CRC_result);
  printf("Result: %08X\n", CRC_result);
  if (XMC_FCE_GetEventStatus(&g_FCE_1, XMC_FCE_STS_MISMATCH_CRC)) {
    temp_mismatch += 1U;
  }

  /* Step 3: Performs a CRC32 check using Kernel 1 on Usecase1_Data2
   * Seed value is set to 0. CRC check comparison is enabled
   * CRC checksum is using result from earlier CRC check
   * CRC mismatch found and Length Error found.
   * CRC = 0x8f2d7440, RES = 0x8f2d7440
   */
  XMC_FCE_InitializeSeedValue(&g_FCE_1, 0);
  XMC_FCE_UpdateLength(&g_FCE_1, temp_length);

  fce_status = XMC_FCE_CalculateCRC32(&g_FCE_1, (uint32_t *)usecase1_Data2,
                                      strlen((const char *)(usecase1_Data2)),
                                      &Read_CRCResult32);
  while (fce_status == XMC_FCE_STATUS_ERROR) {

    Error_Handler();
  }

  XMC_FCE_GetCRCResult(&g_FCE_1, &CRC_result);
  printf("Result: %08X\n", CRC_result);
  if (XMC_FCE_GetEventStatus(&g_FCE_1, XMC_FCE_STS_MISMATCH_CRC)) {
    temp_mismatch += 2U;
  }

  /* Step 4: Performs a CRC16 check using Kernel 2 on Usecase1_Data3
   * Seed value is set to 0.
   * CRC = 0x191e, RES = 0x191e
   */
	
	printf(usecase1_Data1);
	printf("\n");
	printf(usecase1_Data2);
	printf("\n");
	printf(usecase1_Data3);
	printf("\n");
	printf(usecase1_Data4);
	printf("\n");
	
	p_fce = &g_FCE_2;
  XMC_FCE_Init(p_fce);
//  XMC_FCE_InitializeSeedValue(p_fce, 0xffff);
  fce_status = XMC_FCE_CalculateCRC16Ex(p_fce, usecase1_Data1, strlen(usecase1_Data1),
                                      &Read_CRCResult16);
  while (fce_status == XMC_FCE_STATUS_ERROR) {
    Error_Handler();
  }
  printf("Result: %04X\n", Read_CRCResult16);	

	p_fce = &g_FCE_2;
  XMC_FCE_Init(p_fce);
//  XMC_FCE_InitializeSeedValue(p_fce, 0xffff);
  printf("engine:%p, cfg:%08X, init:%08X\n", p_fce->kernel_ptr, p_fce->fce_cfg_update.regval, p_fce->seedvalue);
  fce_status = XMC_FCE_CalculateCRC16Ex(p_fce, usecase1_Data2, strlen(usecase1_Data2),
                                      &Read_CRCResult16);
  while (fce_status == XMC_FCE_STATUS_ERROR) {
    Error_Handler();
  }
  printf("Result: %04X\n", Read_CRCResult16);	

	p_fce = &g_FCE_2;
  XMC_FCE_Init(p_fce);
//  XMC_FCE_InitializeSeedValue(p_fce, 0xffff);
  printf("engine:%p, cfg:%08X, init:%08X\n", p_fce->kernel_ptr, p_fce->fce_cfg_update.regval, p_fce->seedvalue);
  fce_status = XMC_FCE_CalculateCRC16Ex(p_fce, usecase1_Data3, strlen(usecase1_Data3),
                                      &Read_CRCResult16);
  while (fce_status == XMC_FCE_STATUS_ERROR) {
    Error_Handler();
  }
  printf("Result: %04X\n", Read_CRCResult16);	

	p_fce = &g_FCE_2;
  XMC_FCE_Init(p_fce);
//  XMC_FCE_InitializeSeedValue(p_fce, 0xffff);
  printf("engine:%p, cfg:%08X, init:%08X\n", p_fce->kernel_ptr, p_fce->fce_cfg_update.regval, p_fce->seedvalue);
  fce_status = XMC_FCE_CalculateCRC16Ex(p_fce, usecase1_Data4, strlen(usecase1_Data4),
                                      &Read_CRCResult16);
  while (fce_status == XMC_FCE_STATUS_ERROR) {
    Error_Handler();
  }
  printf("Result: %04X\n", Read_CRCResult16);	

	p_fce = &g_FCE_2;
  XMC_FCE_Init(p_fce);
//  XMC_FCE_InitializeSeedValue(p_fce, 0xffff);
  printf("engine:%p, cfg:%08X, init:%08X\n", p_fce->kernel_ptr, p_fce->fce_cfg_update.regval, p_fce->seedvalue);
  fce_status = XMC_FCE_CalculateCRC16Ex(p_fce, test_vec5, sizeof(test_vec5),
                                      &Read_CRCResult16);
  while (fce_status == XMC_FCE_STATUS_ERROR) {
    Error_Handler();
  }
  printf("Result: %04X\n", Read_CRCResult16);	
	

	uint16_t res0 = crc16_algorithm_2(0xffff, usecase1_Data1, strlen(usecase1_Data1));
	printf("Ref value:%04X\n", res0);

	uint16_t res1 = crc16_algorithm_2(0xffff, usecase1_Data2, strlen(usecase1_Data2));
	printf("Ref value:%04X\n", res1);
	
	uint16_t res2 = crc16_algorithm_2(0xffff, usecase1_Data3, strlen(usecase1_Data3));
	printf("Ref value:%04X\n", res2);
	
	uint16_t res3 = crc16_algorithm_2(0xffff, usecase1_Data4, strlen(usecase1_Data4));
	printf("Ref value:%04X\n", res3);
	
	uint16_t res5 = crc16_algorithm_2(0xffff, test_vec5, sizeof(test_vec5));
	printf("Ref value:%04X\n", res5);	
  /* Step 5: Performs a CRC8 check using Kernel 3 on Usecase1_Data4
   * Seed value is set to 0. CRC = 0xbe, RES = 0xbe
   */
  XMC_FCE_InitializeSeedValue(&g_FCE_3, 0);
  fce_status = XMC_FCE_CalculateCRC8(&g_FCE_3, (uint8_t *)usecase1_Data3,
                                     strlen((const char *)(usecase1_Data3)),
                                     &Read_CRCResult8);
  while (fce_status == XMC_FCE_STATUS_ERROR) {

    Error_Handler();
  }
  printf("Result: %08X\n", Read_CRCResult8);

  /* Step 6: Trigger a mismatch flag
   */
  flagstatus = XMC_FCE_GetEventStatus(&g_FCE_3, XMC_FCE_STS_MISMATCH_CRC);
  while (flagstatus) {
    /* endless loop if mismatch flag is triggered */
  }
  XMC_FCE_TriggerMismatch(&g_FCE_3, XMC_FCE_CTR_MISMATCH_CRC);
  flagstatus = XMC_FCE_GetEventStatus(&g_FCE_3, XMC_FCE_STS_MISMATCH_CRC);
	
	//CRC32 Performance benchmark
	{
		uint32_t start_ticks;
		uint32_t end_ticks;

		/*################### Calculation using Hardware FCE0 #############*/
		start_ticks = HAL_GetTick();
		for (uint32_t i = 0; i < LOOP_NB; i++) {		
			p_fce = &g_FCE_0;
			p_fce->fce_cfg_update.config_refin = XMC_FCE_REFIN_SET;
			p_fce->fce_cfg_update.config_refout = XMC_FCE_REFIN_SET;
			p_fce->fce_cfg_update.config_xsel = XMC_FCE_REFIN_RESET;
			p_fce->seedvalue = UINT32_MAX;			
			XMC_FCE_Init(p_fce);
			XMC_FCE_CalculateCRC32Ex(p_fce, (const uint8_t*)usecase1_Data3,
														 strlen((const char *)(usecase1_Data3)),
														 &Read_CRCResult32);
			XMC_FCE_GetCRCResult(p_fce, &CRC_result);
		}
		end_ticks = HAL_GetTick();
		printf("Result: %08X [%u-%u]=%u\n", CRC_result, end_ticks, start_ticks, end_ticks-start_ticks);

		printf("\n");

		/*################### Calculation using Software CRC Algorithm 2 #############*/		
		start_ticks = HAL_GetTick();							 
		for (uint32_t i = 0; i < LOOP_NB; i++) {
			CRC_result = mss_ethernet_crc((uint8_t*)usecase1_Data3, strlen((const char *)(usecase1_Data3)));
		}		
		end_ticks = HAL_GetTick();
		printf("Soft CRC32 IEEE802.3 Result: %08X [%u-%u]=%u\n", CRC_result, end_ticks, start_ticks, end_ticks-start_ticks);
		printf("\n");		
	}
	
	
  while (flagstatus) {
    printf("mismatch flag is triggered\n");
    Error_Handler();
  }
}
