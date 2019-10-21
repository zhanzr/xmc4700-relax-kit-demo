#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <arm_math.h>

#include "utilis.h"

/* Array of angles in Q1.31 format, regularly incremented from 0 to 2*pi */
static int32_t aAnglesCordic[ARRAY_SIZE] = {
    0x00000000, 0x04000000, 0x08000000, 0x0C000000, 0x10000000, 0x14000000,
    0x18000000, 0x1C000000, 0x20000000, 0x24000000, 0x28000000, 0x2C000000,
    0x30000000, 0x34000000, 0x38000000, 0x3C000000, 0x40000000, 0x44000000,
    0x48000000, 0x4C000000, 0x50000000, 0x54000000, 0x58000000, 0x5C000000,
    0x60000000, 0x64000000, 0x68000000, 0x6C000000, 0x70000000, 0x74000000,
    0x78000000, 0x7C000000, 0x80000000, 0x84000000, 0x88000000, 0x8C000000,
    0x90000000, 0x94000000, 0x98000000, 0x9C000000, 0xA0000000, 0xA4000000,
    0xA8000000, 0xAC000000, 0xB0000000, 0xB4000000, 0xB8000000, 0xBC000000,
    0xC0000000, 0xC4000000, 0xC8000000, 0xCC000000, 0xD0000000, 0xD4000000,
    0xD8000000, 0xDC000000, 0xE0000000, 0xE4000000, 0xE8000000, 0xEC000000,
    0xF0000000, 0xF4000000, 0xF8000000, 0xFC000000};

/* Array of angles for CMSIS DSP library Q1.31 format, regularly incremented
 * from 0 to 2*pi */
// Note: The CMSIS DSP Q Format is different from CORDIC Q Format
static int32_t aAnglesLib[ARRAY_SIZE] = {
    0x00000000, 0x02000000, 0x04000000, 0x06000000, 0x08000000, 0x0A000000,
    0x0C000000, 0x0E000000, 0x10000000, 0x12000000, 0x14000000, 0x16000000,
    0x18000000, 0x1A000000, 0x1C000000, 0x1E000000, 0x20000000, 0x22000000,
    0x24000000, 0x26000000, 0x28000000, 0x2A000000, 0x2C000000, 0x2E000000,
    0x30000000, 0x32000000, 0x34000000, 0x36000000, 0x38000000, 0x3A000000,
    0x3C000000, 0x3E000000, 0x40000000, 0x42000000, 0x44000000, 0x46000000,
    0x48000000, 0x4A000000, 0x4C000000, 0x4E000000, 0x50000000, 0x52000000,
    0x54000000, 0x56000000, 0x58000000, 0x5A000000, 0x5C000000, 0x5E000000,
    0x60000000, 0x62000000, 0x64000000, 0x66000000, 0x68000000, 0x6A000000,
    0x6C000000, 0x6E000000, 0x70000000, 0x72000000, 0x74000000, 0x76000000,
    0x78000000, 0x7A000000, 0x7C000000, 0x7E000000};

/* Array of reference sines in Q1.31 format */
static int32_t aRefSin[ARRAY_SIZE] = {
    0x00000000, 0x0C8BD35E, 0x18F8B83C, 0x25280C5D, 0x30FBC54D, 0x3C56BA6F,
    0x471CECE6, 0x5133CC93, 0x5A827999, 0x62F201AB, 0x6A6D98A3, 0x70E2CBC5,
    0x7641AF3C, 0x7A7D055A, 0x7D8A5F3F, 0x7F62368E, 0x7FFFFFFF, 0x7F62368E,
    0x7D8A5F3F, 0x7A7D055A, 0x7641AF3C, 0x70E2CBC5, 0x6A6D98A3, 0x62F201AB,
    0x5A827999, 0x5133CC93, 0x471CECE6, 0x3C56BA6F, 0x30FBC54D, 0x25280C5D,
    0x18F8B83C, 0x0C8BD35E, 0x00000000, 0xF3742CA2, 0xE70747C4, 0xDAD7F3A3,
    0xCF043AB3, 0xC3A94591, 0xB8E3131A, 0xAECC336D, 0xA57D8667, 0x9D0DFE55,
    0x9592675D, 0x8F1D343B, 0x89BE50C4, 0x8582FAA6, 0x8275A0C1, 0x809DC972,
    0x80000001, 0x809DC972, 0x8275A0C1, 0x8582FAA6, 0x89BE50C4, 0x8F1D343B,
    0x9592675D, 0x9D0DFE55, 0xA57D8667, 0xAECC336D, 0xB8E3131A, 0xC3A94591,
    0xCF043AB3, 0xDAD7F3A3, 0xE70747C4, 0xF3742CA2,
};

/* Array of reference cosines in Q1.31 format */
static int32_t aRefCos[ARRAY_SIZE] = {
    0x7FFFFFFF, 0x7F62368E, 0x7D8A5F3F, 0x7A7D055A, 0x7641AF3C, 0x70E2CBC5,
    0x6A6D98A3, 0x62F201AB, 0x5A827999, 0x5133CC93, 0x471CECE6, 0x3C56BA6F,
    0x30FBC54D, 0x25280C5D, 0x18F8B83C, 0x0C8BD35E, 0x00000000, 0xF3742CA2,
    0xE70747C4, 0xDAD7F3A3, 0xCF043AB3, 0xC3A94591, 0xB8E3131A, 0xAECC336D,
    0xA57D8667, 0x9D0DFE55, 0x9592675D, 0x8F1D343B, 0x89BE50C4, 0x8582FAA6,
    0x8275A0C1, 0x809DC972, 0x80000001, 0x809DC972, 0x8275A0C1, 0x8582FAA6,
    0x89BE50C4, 0x8F1D343B, 0x9592675D, 0x9D0DFE55, 0xA57D8667, 0xAECC336D,
    0xB8E3131A, 0xC3A94591, 0xCF043AB3, 0xDAD7F3A3, 0xE70747C4, 0xF3742CA2,
    0x00000000, 0x0C8BD35E, 0x18F8B83C, 0x25280C5D, 0x30FBC54D, 0x3C56BA6F,
    0x471CECE6, 0x5133CC93, 0x5A827999, 0x62F201AB, 0x6A6D98A3, 0x70E2CBC5,
    0x7641AF3C, 0x7A7D055A, 0x7D8A5F3F, 0x7F62368E,
};

/* Array of calculation results in Q1.31 format.
   Will contain alternatively Sine and Cosine of input angles */
static int32_t aResults[2 * ARRAY_SIZE];

/**
  * @brief  Check delta between two values is below threshold
  * @param  VarA First input variable
  * @param  VarB Second input variable
  * @param  MaxError Maximum delta allowed between VarA and VarB
  * @retval Status
  *           PASS: Delta is below threshold
  *           FAIL: Delta is above threshold
  */
uint32_t Check_Residual_Error(int32_t VarA, int32_t VarB, uint32_t MaxError) {
  uint32_t status = PASS;

  if ((VarA - VarB) >= 0) {
    if ((VarA - VarB) > MaxError) {
      status = FAIL;
    }
  } else {
    if ((VarB - VarA) > MaxError) {
      status = FAIL;
    }
  }

  return status;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void Error_Handler(void) {
  while (1) {
    /* LED2 is blinking */
    // BSP_LED_Toggle(LED2);
    HAL_Delay(500);
  }
}

void fpu_perfmance_test(void) {
  uint32_t start_ticks;
  uint32_t end_ticks;

  /*################### Calculation using CMSIS DSP library #############*/
  start_ticks = HAL_GetTick();
  for (uint32_t i = 0; i < LOOP_NB; i++) {
    for (uint32_t j = 0; j < ARRAY_SIZE; j++) {
      /* Calculate sine */
      aResults[2 * j] = arm_sin_q31(aAnglesLib[j]);

      /* Calculate cosine */
      aResults[(2 * j) + 1] = arm_cos_q31(aAnglesLib[j]);
    }
  }
  end_ticks = HAL_GetTick();

  /* Compare calculated results to the reference values */
  for (uint32_t i = 0; i < ARRAY_SIZE; i++) {
    //		 printf("0x%08X, 0x%08X,\n", aResults[2*i], aResults[(2*i) +
    //1]);
    if ((Check_Residual_Error(aResults[2 * i], aRefSin[i], DELTA) == FAIL) ||
        (Check_Residual_Error(aResults[(2 * i) + 1], aRefCos[i], DELTA) ==
         FAIL)) {
      Error_Handler();
    }
  }

  /* Correct output values: Turn LED2 on */
  // BSP_LED_On(LED2);
  printf("Correct CMSIS DSP output[%u-%u = %u]\n", start_ticks, end_ticks,
         end_ticks - start_ticks);
  /*################### Calculation using FPU(Single Precision) #############*/
  // Generate the radian angle
  float angle_floats[ARRAY_SIZE];
  for (uint32_t i = 0; i < ARRAY_SIZE; ++i) {
    angle_floats[i] = (i * PI * 2) / ARRAY_SIZE;
  }

  start_ticks = HAL_GetTick();
  for (uint32_t i = 0; i < LOOP_NB; i++) {
    for (uint32_t j = 0; j < ARRAY_SIZE; j++) {
      /* Calculate sine */
      aResults[2 * j] = sinf(angle_floats[j]);

      /* Calculate cosine */
      aResults[(2 * j) + 1] = cosf(angle_floats[j]);
    }
  }
  end_ticks = HAL_GetTick();

  /* Correct output values: Turn LED2 on */
  // BSP_LED_On(LED2);
  printf("Correct Single precision output[%u-%u = %u]\n", start_ticks,
         end_ticks, end_ticks - start_ticks);

  /*################### Calculation using soft float library(Double Precision)
   * #############*/
  // Generate the radian angle
  double angle_doubles[ARRAY_SIZE];
  for (uint32_t i = 0; i < ARRAY_SIZE; ++i) {
    angle_doubles[i] = (i * PI * 2) / ARRAY_SIZE;
  }

  start_ticks = HAL_GetTick();
  for (uint32_t i = 0; i < LOOP_NB; i++) {
    for (uint32_t j = 0; j < ARRAY_SIZE; j++) {
      /* Calculate sine */
      aResults[2 * j] = sin(angle_doubles[j]);

      /* Calculate cosine */
      aResults[(2 * j) + 1] = cos(angle_doubles[j]);
    }
  }
  end_ticks = HAL_GetTick();

  /* Correct output values: Turn LED2 on */
  // BSP_LED_On(LED2);
  printf("Correct double precision output[%u-%u = %u]\n", start_ticks,
         end_ticks, end_ticks - start_ticks);
}
