#include <stdio.h>

int JavaStyleConcat(void) {
    int value = 4;
    const char *string = "value = " + value; // This isn't Java!
    printf("%s\n", string);
    return 0;
}

//ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_error_4.c...

//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
test_warn_error_4.c(5): warning: adding 'int' to a string does not append to the string [-Wstring-plus-int]
    const char *string = "value = " + value; // This isn't Java!
                         ~~~~~~~~~~~^~~~~~~
test_warn_error_4.c(5): note: use array indexing to silence this warning
    const char *string = "value = " + value; // This isn't Java!
                                    ^
                         &          [      ]
1 warning generated.
compiling test_warn_error_4.c...