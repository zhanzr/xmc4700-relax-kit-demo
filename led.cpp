#include <xmc_gpio.h>
#include "led.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Valid LED GPIO Pins for this application
// 5, 8
// 5, 9

XMC_GPIO_MODE_t LED::InitMode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;
XMC_GPIO_MODE_t LED::UnInitMode = XMC_GPIO_MODE_INPUT_TRISTATE;

XMC_GPIO_OUTPUT_LEVEL_t LED::OutputLevel = XMC_GPIO_OUTPUT_LEVEL_LOW;
XMC_GPIO_OUTPUT_STRENGTH LED::Strength = XMC_GPIO_OUTPUT_STRENGTH_WEAK;

// TODO: This variable will be inited two times
led_position_pair LedPositions[LED::LED_COUNT];

// Default Cons
LED::LED(void) {
  m_num = LED::LED_COUNT;
  m_IsInv = false;
  m_HasInit = false;

  if (NULL == LedPositions[0].first) {
    LedPositions[0] = std::make_pair(XMC_GPIO_PORT5, 8);
    LedPositions[1] = std::make_pair(XMC_GPIO_PORT5, 9);
  }

  printf("Cons default\r\n");
}

LED::LED(const uint8_t num) {
  // TODO:Check the port and pin are valid num
  assert(num < LED_COUNT);

  if (NULL == LedPositions[0].first) {
    LedPositions[0] = std::make_pair(XMC_GPIO_PORT5, 8);
    LedPositions[1] = std::make_pair(XMC_GPIO_PORT5, 9);
  }

  m_num = num;

  XMC_GPIO_CONFIG_t gpio_config;

  gpio_config.mode = InitMode;
  gpio_config.output_level = OutputLevel;
  gpio_config.output_strength = Strength;

  ::XMC_GPIO_Init(LedPositions[num].first, LedPositions[num].second,
                  &gpio_config);

  m_IsInv = false;

  m_HasInit = true;

  Off();

  printf("Cons of %u\r\n", m_num);
}

LED::~LED(void) {
  UnInit();
  printf("Destr of %u\r\n", m_num);
}

void LED::Init(const bool inv_logic) {
  assert(m_num < LED::LED_COUNT);

  if (m_HasInit) {
    UnInit();
  }

  m_IsInv = inv_logic;

  ::XMC_GPIO_SetMode(LedPositions[m_num].first, LedPositions[m_num].second,
                     InitMode);

  m_IsInv = false;

  m_HasInit = true;

  Off();
}

void LED::UnInit(void) {
  assert(m_num < LED::LED_COUNT);

  if (m_HasInit) {
    ::XMC_GPIO_SetMode(LedPositions[m_num].first, LedPositions[m_num].second,
                       UnInitMode);

    m_HasInit = false;
  }
}

void LED::On(void) {
  assert(m_num < LED::LED_COUNT);

  if (m_HasInit) {
    if (m_IsInv) {
      ::XMC_GPIO_SetOutputLow(LedPositions[m_num].first,
                              LedPositions[m_num].second);
    } else {
      ::XMC_GPIO_SetOutputHigh(LedPositions[m_num].first,
                               LedPositions[m_num].second);
    }
  }
}

void LED::Off(void) {
  assert(m_num < LED::LED_COUNT);

  if (m_HasInit) {
    if (m_IsInv) {
      ::XMC_GPIO_SetOutputHigh(LedPositions[m_num].first,
                               LedPositions[m_num].second);
    } else {
      ::XMC_GPIO_SetOutputLow(LedPositions[m_num].first,
                              LedPositions[m_num].second);
    }
  }
}

void LED::Toogle(void) {
  assert(m_num < LED::LED_COUNT);

  if (m_HasInit) {
    XMC_GPIO_ToggleOutput(LedPositions[m_num].first,
                          LedPositions[m_num].second);
  }
}

string LED::ToString(void) {
  char buff[256];
  snprintf(buff, sizeof(buff), "num:%u, inv:%u, has_init:%u", m_num, m_IsInv,
           m_HasInit);
  return string(buff);
}
