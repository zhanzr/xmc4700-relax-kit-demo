#include <stdio.h>

static const int array[4] = { 1, 2, 3, 4 };

int OutOfBounds(void) {
    for (int i = 0; i <= 4 /* should be < */; i++) {
        printf("%d ", array[i]);
    }
    return 0;
}

//ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_error_2.c...

//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
compiling test_warn_error_2.c...