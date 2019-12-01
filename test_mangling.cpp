#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <XMC4700.h>

#include "test_mangling.h"

int func_e(void) {
	return 1; 
}

int func_f(int input) {
	return 0;
}

void func_g(void) {
	int i = func_e();
  int j = func_f(0);
} 
