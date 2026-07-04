#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <stdint.h>
#include <xmc_gpio.h>

#define BUTTON1 P15_13
#define BUTTON2 P15_12

#define BUTTONS_TMR_INTERVAL 100

void Buttons_Initialize (void);

#endif //	#endif //LED_H
