#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <numeric>
#include <limits>
#include <typeinfo>
#include <map>
#include <functional>
#include <string>
#include <memory>
#include <iomanip>
#include <array>

#include <XMC4700.h>
#include <xmc_scu.h>
#include <xmc_uart.h>
#include <xmc_gpio.h>

#include "RTE_Components.h"

#include "custom_def.h"
#include "led.h"
#include "utilis.h"
#include "test_function.h"

#if defined(__cplusplus) && (__cplusplus <= 201103)
namespace std {
typedef decltype(nullptr) nullptr_t;
}
#else
#ifndef nullptr
#define nullptr NULL
#endif
#endif

using namespace std;

__IO uint8_t g_tmp_uart_rx_buf;

extern uint32_t __Vectors;

void dump_bin(uint8_t *data, uint32_t len, string topics) {
  cout << topics << ':' << endl;

  cout << hex << endl;
  cout << setw(2);
  cout << setfill('0');
  for (uint32_t i = 0; i < len; ++i) {
    cout << *(data + i) << ' ';
  }
  cout << dec << endl;
}

void test_normal_ptr(void) {
  cout << __func__ << endl;

  LED *p_normal_led1(new LED(0));
  __IO uint32_t tmpTick;

  LED *p_normal_led2 = p_normal_led1;

  HAL_Delay(400);

  p_normal_led2->Toogle();
  HAL_Delay(400);

  p_normal_led1->Toogle();

  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
  cout << p_normal_led1 << ' ' << p_normal_led2 << endl;
  cout << dec << endl;

  // Leaking memory
  cout << "exit " << __func__ << endl;
}

void test_auto_ptr(void) {
  cout << __func__ << endl;

  auto_ptr<LED> p_smart_led1(new LED(0));

  // Copy and transfer ownership.
  // p_smart_led1 gets set to empty!
  auto_ptr<LED> p_smart_led2 = p_smart_led1;

  HAL_Delay(400);

  // Works.
  if (nullptr != p_smart_led2.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led2.get()->Toogle();

  } else {
    cout << "null pointer, will not work" << endl;
  }

  HAL_Delay(400);

  // Hopefully raises some NULL pointer exception.
  if (nullptr != p_smart_led1.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led1.get()->Toogle();
  } else {
    cout << "null pointer, will not work" << endl;
  }

  HAL_Delay(400);

  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
  cout << p_smart_led1.get() << ' ' << p_smart_led2.get() << endl;
  cout << dec << endl;

  if (nullptr != p_smart_led1.get()) {
    cout << p_smart_led1.get()->ToString() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  if (nullptr != p_smart_led2.get()) {
    cout << p_smart_led2.get()->ToString() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  // put it into vector
  cout << "test auto pointer with vector" << endl;
  vector<auto_ptr<LED>> vLed;
#warning "no matching constructor for initialization of '__1::auto_ptr<LED>'"
#if 0
	vLed.push_back(p_smart_led2);
	
	if(nullptr != p_smart_led1.get()) {
		cout << "valid pointer, will work" << endl;
		p_smart_led1.get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}	
	
	if(nullptr != p_smart_led2.get()) {
		cout << "valid pointer, will work" << endl;
		p_smart_led2.get()->Toogle(); 
	
	} else {
		cout << "null pointer, will not work" << endl;
	}
			
	if(nullptr != vLed.begin()->get()) {
		cout << "valid pointer, will work" << endl;
		vLed.begin()->get()->Toogle(); 
	} else {
		cout << "null pointer, will not work" << endl;
	}
#endif

  // test with array
  cout << "test with array" << endl;
  size_t len = 10;
#warning                                                                       \
    "error: no matching constructor for initialization of 'auto_ptr<std::uint32_t []>'"
#if 0
  auto_ptr<uint32_t[]> arr_u32 {new uint32_t[len]};
	for (size_t i {}; i<len; ++i) {    
		arr_u32[i] = i*i;
		cout << arr_u32[i] << ' ';
  }
	cout << endl;
#endif

  // Will call destructor via the owner of the pointer
  cout << "exit " << __func__ << endl;
}

#if defined(__cplusplus) && (__cplusplus >= 201103)
void test_unique_ptr(void) {
  cout << __func__ << endl;

  unique_ptr<LED> p_smart_led1(new LED(0));

  // Copy and transfer ownership.
  // p_smart_led1 gets set to empty!
  unique_ptr<LED> p_smart_led2 = move(p_smart_led1);

  HAL_Delay(400);

  if (nullptr != p_smart_led2.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led2.get()->Toogle();

  } else {
    cout << "null pointer, will not work" << endl;
  }

  HAL_Delay(400);

  if (nullptr != p_smart_led1.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led1.get()->Toogle();
  } else {
    cout << "null pointer, will not work" << endl;
  }

  HAL_Delay(400);

  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
  cout << p_smart_led1.get() << ' ' << p_smart_led2.get() << endl;
  cout << dec << endl;

  if (nullptr != p_smart_led1.get()) {
    cout << p_smart_led1.get()->ToString() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  if (nullptr != p_smart_led2.get()) {
    cout << p_smart_led2.get()->ToString() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  // put it into vector
  cout << "test unique pointer with vector" << endl;
  vector<unique_ptr<LED>> vLed;
  vLed.push_back(move(p_smart_led2));

  if (nullptr != p_smart_led1.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led1.get()->Toogle();
  } else {
    cout << "null pointer, will not work" << endl;
  }

  if (nullptr != p_smart_led2.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led2.get()->Toogle();

  } else {
    cout << "null pointer, will not work" << endl;
  }

  if (nullptr != vLed.begin()->get()) {
    cout << "valid pointer, will work" << endl;
    vLed.begin()->get()->Toogle();
  } else {
    cout << "null pointer, will not work" << endl;
  }

  // test with array
  cout << "test with array" << endl;
  size_t len = 10;
  unique_ptr<uint32_t[]> arr_u32{new uint32_t[len]};
  for (size_t i{}; i < len; ++i) {
    arr_u32[i] = i * i;
    cout << arr_u32[i] << ' ';
  }
  cout << endl;

  // Will call destructor via the owner of the pointer
  cout << "exit " << __func__ << endl;
}
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103)
void test_shared_ptr(void) {
  cout << __func__ << endl;

  shared_ptr<LED> p_smart_led1(new LED(0));

  // sharing
  shared_ptr<LED> p_smart_led2 = p_smart_led1;

  HAL_Delay(400);

  if (nullptr != p_smart_led2.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led2.get()->Toogle();

  } else {
    cout << "null pointer, will not work" << endl;
  }

  HAL_Delay(400);

  if (nullptr != p_smart_led1.get()) {
    cout << "valid pointer, will work" << endl;
    p_smart_led1.get()->Toogle();
  } else {
    cout << "null pointer, will not work" << endl;
  }

  HAL_Delay(400);

  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
  cout << p_smart_led1.get() << ' ' << p_smart_led2.get() << endl;
  cout << dec << endl;

  if (nullptr != p_smart_led1.get()) {
    cout << p_smart_led1.get()->ToString()
         << ", Count:" << p_smart_led1.use_count() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  if (nullptr != p_smart_led2.get()) {
    cout << p_smart_led2.get()->ToString()
         << ", Count:" << p_smart_led2.use_count() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  // put it into vector
  cout << "test shared pointer with vector" << endl;
  vector<shared_ptr<LED>> vLed;
  vLed.push_back(p_smart_led1);
  vLed.push_back(p_smart_led2);

  if (nullptr != p_smart_led1.get()) {
    cout << p_smart_led1.get()->ToString()
         << ", Count:" << p_smart_led2.use_count() << endl;
    p_smart_led1.get()->Toogle();
  } else {
    cout << "null pointer, will not work" << endl;
  }

  if (nullptr != p_smart_led2.get()) {
    cout << p_smart_led2.get()->ToString()
         << ", Count:" << p_smart_led2.use_count() << endl;
    p_smart_led2.get()->Toogle();

  } else {
    cout << "null pointer, will not work" << endl;
  }

  // dump the smart pointer in the vector container
  for (auto it = vLed.begin(); it != vLed.end(); ++it) {
    if (nullptr != it->get()) {
      cout << it->get()->ToString() << ", Count:" << it->use_count() << endl;
      it->get()->Toogle();
    } else {
      cout << "null pointer, will not work" << endl;
    }
  }

  // remove the reference count
  for (auto it = vLed.begin(); it != vLed.end(); ++it) {
    if (nullptr != it->get()) {
      it->reset();
    }

    if (nullptr != p_smart_led1.get()) {
      cout << p_smart_led1.get()->ToString()
           << ", Count:" << p_smart_led2.use_count() << endl;
      p_smart_led1.get()->Toogle();
    } else {
      cout << "null pointer, will not work" << endl;
    }
  }
  // test with array
  cout << "test with array" << endl;
#warning                                                                       \
    "error: no matching constructor for initialization of 'shared_ptr<std::uint32_t []>'	"
  //	size_t len = 10;
  //  shared_ptr<uint32_t[]> arr_u32 {new uint32_t[len]};
  //
  //	for (size_t i {}; i<len; ++i) {
  //		arr_u32[i] = i*i;
  //		cout << arr_u32[i] << ' ';
  //  }
  //	cout << endl;

  // Will call destructor via the owner of the pointer
  cout << "exit " << __func__ << endl;
}
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103)
void test_weak_ptr(void) {
  cout << __func__ << endl;
  weak_ptr<LED> p_smart_led2;

  // build a scope
  {
    shared_ptr<LED> p_smart_led1(new LED(0));

    if (nullptr != p_smart_led1.get()) {
      cout << p_smart_led1.get()->ToString()
           << ", Count:" << p_smart_led1.use_count() << endl;
    } else {
      cout << "null pointer, will not dump" << endl;
    }

    // weak sharing
    p_smart_led2 = p_smart_led1;

    if (auto tmp_share_ptr = p_smart_led2.lock()) {
      cout << tmp_share_ptr.get()->ToString()
           << ", Count:" << tmp_share_ptr.use_count() << endl;
    } else {
      cout << "null pointer, will not dump" << endl;
    }

    cout << hex << endl;
    cout << setw(8);
    cout << setfill('0');
    cout << p_smart_led1.get() << ' ' << p_smart_led2.lock().get() << endl;
    cout << dec << endl;
  }

  if (auto tmp_share_ptr = p_smart_led2.lock()) {
    cout << tmp_share_ptr.get()->ToString()
         << ", Count:" << tmp_share_ptr.use_count() << endl;
  } else {
    cout << "null pointer, will not dump" << endl;
  }

  HAL_Delay(400);

  // Will call destructor via the owner of the pointer
  cout << "exit " << __func__ << endl;
}
#endif

// this algorithm (k=33) was first reported by dan bernstein many years ago in
// comp.lang.c.
// another version of this algorithm (now favored by bernstein) uses xor:
// hash(i) = hash(i - 1) * 33 ^ str[i];
// the magic of number 33 (why it works better than many other constants,
// prime or not) has never been adequately explained.
uint32_t djb2_hash(const char* str) {
  uint32_t hash = 5381;

  /* hash * 33 + c */
  while (true) {
    auto c = *str++;
    if (0 == c) {
      break;
    }
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

// this algorithm was created for sdbm (a public-domain reimplementation of
// ndbm) database library.
// it was found to do well in scrambling bits, causing better distribution of
// the keys and fewer splits.
//	it also happens to be a good general hashing function with good
//distribution.
// the actual function is hash(i) = hash(i - 1) * 65599 + str[i];
// what is included below is the faster version used in gawk. [there is even a
// faster,
// duff-device version] the magic constant 65599 was picked out of thin air
// while experimenting with different constants,
//	and turns out to be a prime. this is one of the algorithms used in
//berkeley db (see

uint32_t sdbm_hash(const char* str) {
  uint32_t hash = 0;

  while (true) {
    auto c = *str++;
    if (0 == c) {
      break;
    }
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
  return hash;
}

// This hash function appeared in K&R (1st ed) but at least the reader was
// warned:
//		"This is not the best possible algorithm, but it has the merit of
//extreme simplicity."
//		This is an understatement; It is a terrible hashing algorithm,
//		and it could have been much better without sacrificing its
//"extreme simplicity."
//			[see the second edition!] Many C programmers use this
//function without actually testing it,
//				or checking something like Knuth's Sorting and
//Searching, so it stuck.
//					It is now found mixed with otherwise respectable
//code, eg. cnews. sigh. [see also: tpop]
uint32_t lose_lose_hash(const char* str) {
  uint32_t hash = 0;

  while (true) {
    auto c = *str++;
    if (0 == c) {
      break;
    }
    hash += c;
  }
  return hash;
}

const uint32_t TEST_N = 10;

void test_hash_str_djb2(void) {
  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
	
	string s1(__func__);
	for(auto i=0; i<TEST_N; ++i) {
		s1 += 'A';
    cout << s1 << "\t-\t" <<djb2_hash(s1.c_str()) << endl;
	}

	string s2(std::to_string(__cplusplus));
	for(auto i=0; i<TEST_N; ++i) {
		s2 += 'B';
    cout << s2 << "\t-\t" <<djb2_hash(s2.c_str()) << endl;
	}
	
  cout << dec << endl;	
}

void test_hash_str_sdbm(void) {
  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
	
	string s1(__func__);
	for(auto i=0; i<TEST_N; ++i) {
		s1 += 'A';
    cout << s1 << "\t-\t" <<sdbm_hash(s1.c_str()) << endl;
	}

	string s2(std::to_string(__cplusplus));
	for(auto i=0; i<TEST_N; ++i) {
		s2 += 'B';
    cout << s2 << "\t-\t" <<sdbm_hash(s2.c_str()) << endl;
	}
	
  cout << dec << endl;	
}

void test_hash_str_lose_lose(void) {
  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
	
	string s1(__func__);
	for(auto i=0; i<TEST_N; ++i) {
		s1 += 'A';
    cout << s1 << "\t-\t" <<lose_lose_hash(s1.c_str()) << endl;
	}

	string s2(std::to_string(__cplusplus));
	for(auto i=0; i<TEST_N; ++i) {
		s2 += 'B';
    cout << s2 << "\t-\t" <<lose_lose_hash(s2.c_str()) << endl;
	}
	
  cout << dec << endl;	
}

void test_hash_std(void) {
  std::hash<std::string> str_hash;

  cout << hex << endl;
  cout << setw(8);
  cout << setfill('0');
	
	string s1(__func__);
	for(auto i=0; i<TEST_N; ++i) {
		s1 += 'A';
    cout << s1 << "\t-\t" <<str_hash(s1) << endl;
	}

	string s2(std::to_string(__cplusplus));
	for(auto i=0; i<TEST_N; ++i) {
		s2 += 'B';
    cout << s2 << "\t-\t" <<str_hash(s2) << endl;
	}
	
  cout << dec << endl;	
}

// Test Vector of Base64 Encoding according to https://tools.ietf.org/html/rfc4648
//10.  Test Vectors

//   BASE64("") = ""

//   BASE64("f") = "Zg=="

//   BASE64("fo") = "Zm8="

//   BASE64("foo") = "Zm9v"

//   BASE64("foob") = "Zm9vYg=="

//   BASE64("fooba") = "Zm9vYmE="

//   BASE64("foobar") = "Zm9vYmFy"

void base64_test_case(string in, string ref_out) {
	cout <<"input:" << in << ",ref out:" << ref_out << ",len:" << ref_out.size() << endl;
	
	// Encode
	size_t out_len = 0;
	uint8_t* tmp_test_out = base64_encode((const uint8_t*)in.c_str(), in.size(), &out_len);
	string tmp_test_out_str((const char*)tmp_test_out, out_len);
	cout << "encoded:" << tmp_test_out_str << ",len:" << tmp_test_out_str.size() << endl;
	XMC_ASSERT("Equal", (0 == tmp_test_out_str.compare(ref_out)));
	
	// Decode
	size_t decode_len = 0;
	uint8_t* tmp_test_decode = base64_decode((const uint8_t*)tmp_test_out, out_len, &decode_len);
	string tmp_test_dec_str((const char*)tmp_test_decode);
	cout << "decoded:" << tmp_test_dec_str << endl;
	XMC_ASSERT("Equal", (0 == tmp_test_dec_str.compare(in)));
	
	free(tmp_test_decode);
	free(tmp_test_out);
}

void base64_test(void) {
	cout << __func__ << endl;
	auto test_step = 0;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string(""), string(""));
	test_step ++;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string("f"), string("Zg=="));
	test_step ++;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string("fo"), string("Zm8="));
	test_step ++;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string("foo"), string("Zm9v"));
	test_step ++;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string("foob"), string("Zm9vYg=="));
	test_step ++;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string("fooba"), string("Zm9vYmE="));
	test_step ++;
	
	cout << endl << "test step:" << test_step << endl;
	base64_test_case(string("foobar"), string("Zm9vYmFy"));
	test_step ++;
	
	while(true) {
	}
}

void carr_func(int * arr, size_t size) {
    std::cout << "carr_func - arr: " << arr << std::endl;
}

void array_test(void) {
	//Declares an array of 10 ints. Size is always required
	std::array<int, 10> a1;

	//Declare and initialize with an initializer list
	std::array<int, 5> a2 = {-1, 1, 3, 2, 0};
	
	//Making a new array via copy
	auto a3 = a2;

	//This works too:
	auto a4(a2);
	
	//Assign a2 to a3's values:
	a2 = a3;

	// But you can only use the '=' operator on arrays of equivalent size.
	//Error:
	//a1 = a2; //<[...],10> vs <[...],5>! invalid
	
	//Assigning values works as expected
	a3[0] = -2;
	
	std::cout << "a2.at(4): " << a2.at(4) << std::endl;

	// Bounds checking can generate exceptions. Try:
	//auto b = a2.at(10);
	
	//Error:
	//carr_func(a2, a2.size());

	//OK:
	carr_func(a2.data(), a2.size());
}

void normal_func(void) {
// cout << "Enter " << __func__ << endl;	
	__NOP();
// cout << "Exit " << __func__ << endl;	
}

[[ noreturn ]] void noreturn_a(void) {
// cout << "Enter " << __func__ << endl;	
	__NOP();
// cout << "Exit " << __func__ << endl;	
	// Should not Return
}

void noreturn_b(void) __attribute__((noreturn));

void noreturn_b(void) {
// cout << "Enter " << __func__ << endl;	
	__NOP();
// cout << "Exit " << __func__ << endl;	
}

__declspec(noreturn) void noreturn_c(void);

void noreturn_c(void) {
// cout << "Enter " << __func__ << endl;	
	__NOP();
// cout << "Exit " << __func__ << endl;	
}

void test_noreturn(void) {
	normal_func();
	
	noreturn_a();
	
	noreturn_b();
	
	noreturn_c();
}

class DemoClass { 
	public:
		DemoClass() = default;
		~DemoClass() = default;

	mutable uint32_t u32_a = 0;
	uint32_t u32_b = 1;
}; 

const DemoClass demo_a;

void test_const_mutable(void) {
	const DemoClass demo_b;
  
	cout << "demo_a @ " << hex << setfill('0') << setw(8) << reinterpret_cast<uint32_t>(&demo_a) << endl;
	
	cout << "demo_b @ " << hex << setfill('0') << setw(8) << reinterpret_cast<uint32_t>(&demo_b) << endl;
	
	demo_a.u32_a = 2;
	demo_b.u32_a = 2;

	cout << dec;
}

int main(void) {
  /* System timer configuration */
  ::SysTick_Config(SystemCoreClock / HZ);

  setbuf(stdout, NULL);

  cout << "Start @" << SystemCoreClock / 1000000 << " MHz XMCLib"
       << XMC_LIB_MAJOR_VERSION << '.' << XMC_LIB_MINOR_VERSION << '.'
       << XMC_LIB_PATCH_VERSION << endl;

  XMC_SCU_EnableTemperatureSensor();
  XMC_SCU_StartTemperatureMeasurement();

  printf("OSCHIPFreq:%u \n", OSCHP_GetFrequency());

  printf("%u Hz, %08X, CM:%d\n", SystemCoreClock, SCB->CPUID, __CORTEX_M);

  printf("Boot Mode:%u, Vector:%08X\n", XMC_SCU_GetBootMode(),
         (uint32_t)(&__Vectors));

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
    cout << endl << "C++ Standard " << __cplusplus << endl;

    // T_DTS = (RESULT - 605) / 2.05 [°C]
    float tmpDts = XMC_SCU_GetTemperatureMeasurement();
    float tmpCel = (tmpDts - 605) / 2.05f;
    cout << "DTS: " << tmpCel << endl;
    float tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
    float tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
    cout << "Voltage: " << tmpV13 << ' ' << tmpV33 << endl;

    XMC_SCU_StartTemperatureMeasurement();

    //		test_normal_ptr();
    //		cout << endl;
    //		test_auto_ptr();
    //		cout << endl;

//    test_hash_str_djb2();
//    cout << endl;
//    HAL_Delay(2000);
//    test_hash_str_sdbm();
//    cout << endl;
//		HAL_Delay(2000);
//    test_hash_str_lose_lose();
//    cout << endl;		
//		HAL_Delay(2000);
//    test_hash_std();
//    cout << endl;	

//		fpu_perfmance_test();
//		crc_perfmance_test();
//		base64_test();
//		array_test();

  cout << __VERSION__ << endl;
  cout << __TIMESTAMP__ << endl;
  cout << __FILE__ << endl;
  cout << __func__ << endl;
  cout << __LINE__ << endl;
  cout << __COUNTER__ << endl;
  cout << __COUNTER__ << endl;

//		test_function();
		
//		test_noreturn();
		
		test_const_mutable();
		
#if defined(__cplusplus) && (__cplusplus >= 201103)
    //		test_unique_ptr();
    //		cout << endl;
    //		test_shared_ptr();
    //		cout << endl;
    //		test_weak_ptr();
    //		cout << endl;

    HAL_Delay(2000);
#endif
  }
}
