#include <xmc_gpio.h>
#include "buttons.h"

void Buttons_Initialize (void) {
  XMC_GPIO_CONFIG_t config;

  config.mode = XMC_GPIO_MODE_INPUT_TRISTATE;

  XMC_GPIO_Init(BUTTON1, &config);
  XMC_GPIO_Init(BUTTON2, &config);
}
