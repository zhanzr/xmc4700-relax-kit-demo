#include <stdio.h>

int Missing_Semicolons(void) {
    printf("Hello, world!\n") // no semicolon
    return 0 // no semicolon
}

// ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_error_0.c...
test_warn_error_0.c(5): error:  #65: expected a ";"
      return 0 // no semicolon
test_warn_error_0.c(6): warning:  #940-D: missing return statement at end of non-void function "Missing_Semicolons" 
  }

//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
test_warn_error_0.c(4): error: expected ';' after expression
    printf("Hello, world!\n") // no semicolon
                             ^
                             ;
test_warn_error_0.c(5): error: expected ';' after return statement
    return 0 // no semicolon
            ^
            ;
2 errors generated.
compiling test_warn_error_0.c...