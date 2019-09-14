#pragma once

#include <utility> 

#include <assert.h>      

#include <xmc_gpio.h>

//Valid LED GPIO Pins for this application
//5, 8 
//5, 9

typedef std::pair <XMC_GPIO_PORT_t*, uint8_t> led_position_pair;
	
class LED {
	public:	
	static XMC_GPIO_MODE_t InitMode;
	static XMC_GPIO_MODE_t UnInitMode;
	static XMC_GPIO_OUTPUT_LEVEL_t OutputLevel;
	static XMC_GPIO_OUTPUT_STRENGTH Strength;
	
	static const uint8_t LED_COUNT = 2;
		
		LED(void);
		
		LED(const uint8_t num);
	
		~LED(void);
		
		void Init(const bool inv_logic);
		void UnInit(void);
	
		void On(void);
		void Off(void);		
		void Toogle(void);
		
	private:
		uint8_t m_num;
		bool m_IsInv;
		bool m_HasInit;
};
