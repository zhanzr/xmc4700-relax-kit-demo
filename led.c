#include <xmc_common.h>
#include <xmc_gpio.h>

#include "led.h"

/* LED GPIO Pins */
static const GPIO_PIN LED_PIN[] = {
    {XMC_GPIO_PORT5, 9}, {XMC_GPIO_PORT5, 8}, {XMC_GPIO_PORT2, 10}};

#define LED_COUNT (sizeof(LED_PIN) / sizeof(GPIO_PIN))

/**
  \fn          int32_t LED_Initialize (void)
  \brief       Initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Initialize(void) {
  XMC_GPIO_CONFIG_t gpio_config;

  gpio_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
  gpio_config.output_level = XMC_GPIO_OUTPUT_LEVEL_LOW;
  gpio_config.output_strength = XMC_GPIO_OUTPUT_STRENGTH_WEAK;

  for (uint8_t n = 0; n < LED_COUNT; ++n) {
    XMC_GPIO_Init(LED_PIN[n].port, LED_PIN[n].pin, &gpio_config);
		LED_Off(n);
  }
}

/**
  \fn          int32_t LED_Uninitialize (void)
  \brief       De-initialize LEDs
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Uninitialize(void) {
  for (uint8_t n = 0; n < LED_COUNT; ++n) {
		LED_Off(n);
    XMC_GPIO_SetMode(LED_PIN[n].port, LED_PIN[n].pin,
                     XMC_GPIO_MODE_INPUT_TRISTATE);
  }
}

/**
  \fn          int32_t LED_On (uint32_t num)
  \brief       Turn on requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_On(uint8_t num) {
  switch (num) {
  case 0:
  case 1:
    XMC_GPIO_SetOutputHigh(LED_PIN[num].port, LED_PIN[num].pin);
    break;

  case 2:
    XMC_GPIO_SetOutputLow(LED_PIN[num].port, LED_PIN[num].pin);
    break;

  default:
    XMC_ASSERT("led number unexpected", 0);
    break;
  }
}

/**
  \fn          int32_t LED_Off (uint32_t num)
  \brief       Turn off requested LED
  \param[in]   num  LED number
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
void LED_Off(uint8_t num) {
  switch (num) {
  case 0:
  case 1:
    XMC_GPIO_SetOutputLow(LED_PIN[num].port, LED_PIN[num].pin);
    break;

  case 2:
    XMC_GPIO_SetOutputHigh(LED_PIN[num].port, LED_PIN[num].pin);
    break;

  default:
    XMC_ASSERT("led number unexpected", 0);
    break;
  }	
}

void LED_Toggle(uint8_t num) {
  XMC_GPIO_ToggleOutput(LED_PIN[num].port, LED_PIN[num].pin);
}