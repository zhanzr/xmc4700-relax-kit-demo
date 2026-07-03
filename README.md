# XMC4700F144K2048 Relax Kit test.

LED + UART.

## coremark

144 MHz, Standard Lib

### Flash cached
ARMCLANG 6.24
#### With FPU
===================================================
-O1
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 34057
Total time (secs): 34.057000
Iterations/Sec   : 293.625393
Iterations       : 10000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
-o ./Objects/*.o -MMD
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 293.625393 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
===================================================
-Ofast
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 23825
Total time (secs): 23.825000
Iterations/Sec   : 419.727177
Iterations       : 10000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 419.727177 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
========================
#### Without FPU
===================================================
-O1
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 34057
Total time (secs): 34.057000
Iterations/Sec   : 293.625393
Iterations       : 10000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 293.625393 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
===================================================
-Ofast
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 23823
Total time (secs): 23.823000
Iterations/Sec   : 419.762414
Iterations       : 10000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 419.762414 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
===================================================

### Flash uncached
ARMCLANG 6.24
#### With FPU
===================================================
-O1
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 91848
Total time (secs): 91.848000
Iterations/Sec   : 108.875533
Iterations       : 10000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 108.875533 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
===================================================
-Ofast
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 59591
Total time (secs): 59.591000
Iterations/Sec   : 167.810575
Iterations       : 10000
Compiler version : GCCClang 20.0.0git
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 167.810575 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
===================================================


### PSRAM
```C
__attribute__((section("RAM_CODE"))) 
void MyCriticalFunction(void) {
    // This code will execute from PSRAM
}


#pragma clang section text="RAM_CODE"

void FunctionOne(void) {
    // Executes from PSRAM
}

void FunctionTwo(void) {
    // Executes from PSRAM
}

#pragma clang section text="" // Reset back to default flash execution
```
#### With FPU
===================================================
-Ofast
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 23791
Total time (secs): 23.791000
Iterations/Sec   : 420.327014
Iterations    t
Compiler flags   : -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 420.327014 / GCCClang 20.0.0git -xc -std=c11 --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -c -fno-rtti -funsigned-char -fshort-enums -fshort-wchar / STACK
===================================================

