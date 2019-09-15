#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <numeric>
#include <limits>
#include <typeinfo>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <iomanip>

#include <XMC4700.h>
#include <xmc_scu.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include "RTE_Components.h"

#include "custom_def.h"
#include "led.h"

#if defined(__cplusplus) && (__cplusplus >= 201103)
namespace std {
	typedef decltype(nullptr) nullptr_t;
}
#else
#ifndef nullptr
#define	nullptr	NULL
#endif
#endif

using namespace std;

__IO uint8_t g_tmp_uart_rx_buf;
__IO uint32_t g_ticks;

uint32_t HAL_GetTick(void) {
	return g_ticks;
}

void HAL_Delay(uint32_t t) {
	uint32_t d = t + g_ticks;
	while(d > g_ticks) {
		__NOP();
	}
}

extern uint32_t __Vectors;

void dump_bin(uint8_t* data, uint32_t len, string topics) {
	cout << topics << ':' << endl;
		
	cout << hex << endl;
	cout << setw(2);
	cout << setfill('0');
	for(uint32_t i=0; i<len; ++i) {
		cout << *(data + i) << ' ';
	}
	cout << dec << endl;
}

void test_normal_ptr(void) {
	cout << __func__ << endl;
	
	LED* p_normal_led1 (new LED(0));
	__IO uint32_t tmpTick;
	
	LED* p_normal_led2 = p_normal_led1; 
	
	HAL_Delay(400);
	
	p_normal_led2->Toogle(); 
	HAL_Delay(400);

	p_normal_led1->Toogle(); 	
	
	cout << hex << endl;
	cout << setw(8);
	cout << setfill('0');
	cout << p_normal_led1  << ' ' << p_normal_led2 << endl;
	cout << dec << endl;

		//Leaking memory
	cout << "exit " << __func__ << endl;		
}

void test_auto_ptr(void) {
	cout << __func__ << endl;

	auto_ptr<LED> p_smart_led1 (new LED(0));
	
	// Copy and transfer ownership. 																			
	// p_smart_led1 gets set to empty!
	auto_ptr<LED> p_smart_led2 = p_smart_led1; 
	
	HAL_Delay(400);
	
	// Works.
	if(nullptr != p_smart_led2.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led2.get()->Toogle(); 
	
	} else {
		cout << "null pointer, will not work" << endl;
	}
		
	HAL_Delay(400);
	
	// Hopefully raises some NULL pointer exception.		
	if(nullptr != p_smart_led1.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led1.get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}		
	
	HAL_Delay(400);

	cout << hex << endl;
	cout << setw(8);
	cout << setfill('0');
	cout << p_smart_led1.get()  << ' ' << p_smart_led2.get() << endl;
	cout << dec << endl;
		
	if( nullptr != p_smart_led1.get() ) {
		cout << p_smart_led1.get()->ToString() << endl;
	} else {
		cout << "null pointer, will not dump" << endl;
	}
	
	if( nullptr != p_smart_led2.get() ) {
		cout << p_smart_led2.get()->ToString() << endl;
	} else {
		cout << "null pointer, will not dump" << endl;
	}
		
	//put it into vector
	cout << "test auto pointer with vector" << endl;
	vector< auto_ptr<LED> > vLed;
#warning "no matching constructor for initialization of '__1::auto_ptr<LED>'"
#if 0
	vLed.push_back(p_smart_led2);
	
	if(nullptr != p_smart_led1.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led1.get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}	
	
	if(nullptr != p_smart_led2.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led2.get()->Toogle(); 
	
	} else {
		cout << "null pointer, will not work" << endl;
	}
			
	if(nullptr != vLed.begin()->get()) {
		cout << "non null pointer, will work" << endl;
		vLed.begin()->get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}		
#endif

	// test with array
	cout << "test with array" << endl;
	size_t len = 10;
#warning "error: no matching constructor for initialization of 'auto_ptr<std::uint32_t []>'"
#if 0
  auto_ptr<uint32_t[]> arr_u32 {new uint32_t[len]};
	for (size_t i {}; i<len; ++i) {    
		arr_u32[i] = i*i;
		cout << arr_u32[i] << ' ';
  }
	cout << endl;
#endif

	//Will call destructor via the owner of the pointer
	cout << "exit " << __func__ << endl;
}

#if defined(__cplusplus) && (__cplusplus >= 201103)
void test_unique_ptr(void) {
	cout << __func__ << endl;

	unique_ptr<LED> p_smart_led1 (new LED(0));
	
	// Copy and transfer ownership. 																			
	// p_smart_led1 gets set to empty!
	unique_ptr<LED> p_smart_led2 = move(p_smart_led1); 
	
	HAL_Delay(400);
	
	if(nullptr != p_smart_led2.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led2.get()->Toogle(); 
	
	} else {
		cout << "null pointer, will not work" << endl;
	}
		
	HAL_Delay(400);
	
	if(nullptr != p_smart_led1.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led1.get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}		
	
	HAL_Delay(400);

	cout << hex << endl;
	cout << setw(8);
	cout << setfill('0');
	cout << p_smart_led1.get()  << ' ' << p_smart_led2.get() << endl;
	cout << dec << endl;
		
	if( nullptr != p_smart_led1.get() ) {
		cout << p_smart_led1.get()->ToString() << endl;
	} else {
		cout << "null pointer, will not dump" << endl;
	}
	
	if( nullptr != p_smart_led2.get() ) {
		cout << p_smart_led2.get()->ToString() << endl;
	} else {
		cout << "null pointer, will not dump" << endl;
	}
	
	//put it into vector
	cout << "test unique pointer with vector" << endl;
	vector< unique_ptr<LED> > vLed;
	vLed.push_back(move(p_smart_led2));
	
	if(nullptr != p_smart_led1.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led1.get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}	
	
	if(nullptr != p_smart_led2.get()) {
		cout << "non null pointer, will work" << endl;
		p_smart_led2.get()->Toogle(); 
	
	} else {
		cout << "null pointer, will not work" << endl;
	}
			
	if(nullptr != vLed.begin()->get()) {
		cout << "non null pointer, will work" << endl;
		vLed.begin()->get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}	
	
	// test with array
	cout << "test with array" << endl;
	size_t len = 10;
  unique_ptr<uint32_t[]> arr_u32 {new uint32_t[len]};
	for (size_t i {}; i<len; ++i) {    
		arr_u32[i] = i*i;
		cout << arr_u32[i] << ' ';
  }
	cout << endl;
	 
	//Will call destructor via the owner of the pointer
	cout << "exit " << __func__ << endl;
}
#endif

int main(void) {
  /* System timer configuration */
  ::SysTick_Config(SystemCoreClock / HZ);
	
	setbuf(stdout, NULL);
	
	cout << "Start @" << SystemCoreClock/1000000 << " MHz XMCLib"
	    << XMC_LIB_MAJOR_VERSION << '.' << XMC_LIB_MINOR_VERSION << '.' << XMC_LIB_PATCH_VERSION <<endl;
		
	XMC_SCU_EnableTemperatureSensor();
	XMC_SCU_StartTemperatureMeasurement();
	
  printf("OSCHIPFreq:%u \n", OSCHP_GetFrequency());
	
	printf("%u Hz, %08X, CM:%d\n",
			SystemCoreClock,
			SCB->CPUID,
			__CORTEX_M);
					
	printf("Boot Mode:%u, Vector:%08X\n",
	XMC_SCU_GetBootMode(), (uint32_t)(&__Vectors));
	
  #ifdef RTE_Compiler_IO_STDOUT_User
	printf("RTE_Compiler_IO_STDOUT_User\n");
	#endif
	
  #ifdef RTE_Compiler_IO_STDOUT_EVR
	printf("RTE_Compiler_IO_STDOUT_EVR\n");
	#endif

  #ifdef RTE_Compiler_IO_STDOUT_ITM
	printf("RTE_Compiler_IO_STDOUT_ITM\n");
	#endif
		
  #ifdef __FPU_PRESENT
	printf("__FPU_PRESENT = %u\n", __FPU_PRESENT);
	#else
	printf("__FPU_PRESENT NA\n");
	#endif
	
  #ifdef __FPU_USED
	printf("__FPU_USED = %u\n", __FPU_USED);
	#else
	printf("__FPU_USED NA\n");
	#endif
	
  #ifdef __TARGET_FPU_NONE
	printf("__TARGET_FPU_NONE = %u\n", __TARGET_FPU_NONE);
	#else
	printf("__TARGET_FPU_NONE NA\n");
	#endif		
	
  #ifdef __TARGET_FPU_VFP
	printf("__TARGET_FPU_VFP = %u\n", __TARGET_FPU_VFP);
	#else
	printf("__TARGET_FPU_VFP NA\n");
	#endif	
	
  #ifdef __TARGET_FPU_SOFTVFP
	printf("__TARGET_FPU_SOFTVFP = %u\n", __TARGET_FPU_SOFTVFP);
	#else
	printf("__TARGET_FPU_SOFTVFP NA\n");
	#endif		
	
  cout << setprecision(3);					
  while (1) {
		cout << endl << "C++ Standard " << __cplusplus  << endl;
		
		//T_DTS = (RESULT - 605) / 2.05 [°C]
		float tmpDts = XMC_SCU_GetTemperatureMeasurement();
		float tmpCel = (tmpDts-605)/2.05f;		
		cout << "DTS: " << tmpCel << endl;
		float tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
		float tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
		cout << "Voltage: " << tmpV13 << ' ' << tmpV33 << endl;
		
		XMC_SCU_StartTemperatureMeasurement();

		test_normal_ptr();
		
		cout << endl;
		
		HAL_Delay(4000);
		
		test_auto_ptr();	
		
		cout << endl;	
		
		HAL_Delay(4000);

#if defined(__cplusplus) && (__cplusplus >= 201103)
		test_unique_ptr();	
		
		cout << endl;	
		
		HAL_Delay(4000);
#endif		
	}
}
