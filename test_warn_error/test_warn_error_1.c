int MissingBrace(void) {
    return 0;
// no closing brace

	//ARMCC
*** Using Compiler 'V5.06 update 6 (build 750)', folder: 'G:\Keil_v5\ARM\ARMCC\Bin'
compiling test_warn_error_1.c...
test_warn_error_1.c(3): error: At end of source:  #67: expected a "}"
	
//ARMCLANG
*** Using Compiler 'V6.12', folder: 'G:\Keil_v5\ARM\ARMCLANG\Bin'
test_warn_error_1.c(11): error: expected '}'
        
        ^
test_warn_error_1.c(1): note: to match this '{'
int MissingBrace(void) {
                       ^
1 error generated.
compiling test_warn_error_1.c...