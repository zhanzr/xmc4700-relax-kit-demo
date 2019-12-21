#include <stdio.h>

int IfsWithoutBraces(int argc, char**argv) {
    if (argc > 1) // needs braces
        argc--;
        argv++;
    else
        printf("Usage: %s <arguments>\n", *argv); // (this would theoretically be UB because of the argv++)
    return 0;
}

//ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_eror_3.c...
test_warn_eror_3.c(7): error:  #127: expected a statement
      else
				
//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
test_warn_eror_3.c(7): error: expected expression
    else
    ^
1 error generated.
compiling test_warn_eror_3.c...