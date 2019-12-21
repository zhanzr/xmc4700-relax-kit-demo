#include <stdlib.h>

int doesNotReturnAValue(void) {
    // no return value
}

int mightNotReturnAValue(void) {
    if (rand() % 2 == 0) {
        return 2;
    }
    // if rand() is odd, there is no return value
}

//ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_error_5.c...
test_warn_error_5.c(5): warning:  #940-D: missing return statement at end of non-void function "doesNotReturnAValue" 
  }
test_warn_error_5.c(12): warning:  #940-D: missing return statement at end of non-void function "mightNotReturnAValue" 
  }
	

//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
test_warn_error_5.c(5): warning: control reaches end of non-void function [-Wreturn-type]
}
^
test_warn_error_5.c(12): warning: control may reach end of non-void function [-Wreturn-type]
}
^
2 warnings generated.
compiling test_warn_error_5.c...