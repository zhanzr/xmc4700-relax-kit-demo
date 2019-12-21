#include <iostream>

int NoNameSpace() {
    cout << "Hello, world!\n"; // should be std::cout
    return 0;
}

//ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_error_6.cpp...
test_warn_error_6.cpp(4): error:  #20: identifier "cout" is undefined
      cout << "Hello, world!\n"; // should be std::cout

//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
test_warn_error_6.cpp(4): error: use of undeclared identifier 'cout'; did you mean 'std::cout'?
    cout << "Hello, world!\n"; // should be std::cout
    ^~~~
    std::cout
G:\Keil_v5\ARM\ARMCLANG\Bin\..\include\libcxx\iostream(53): note: 'std::cout' declared here
extern _LIBCPP_FUNC_VIS ostream cout;
                                ^
1 error generated.
compiling test_warn_error_6.cpp...