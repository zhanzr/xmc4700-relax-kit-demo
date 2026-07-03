# XMC4700F144K2048 Relax Kit test.

LED + UART.

## dhrystone

144 MHz, Standard Lib

### Flash cached
ARMCLANG 6.24
#### With FPU
===================================================
-O1
MicroSecond for one run through Dhrystone[4-5825]:       2.911
Dhrystones per Second:  343583.562
DMIPS/MHz:      1.358
===================================================
-Ofast
MicroSecond for one run through Dhrystone[4-5422]:       2.709
Dhrystones per Second:  369139.906
DMIPS/MHz:      1.459
===================================================
#### Without FPU
===================================================
-O1
MicroSecond for one run through Dhrystone[4-5797]:       2.896
Dhrystones per Second:  345244.250
DMIPS/MHz:      1.365
===================================================
-Ofast
MicroSecond for one run through Dhrystone[4-5450]:       2.723
Dhrystones per Second:  367242.000
DMIPS/MHz:      1.452
===================================================

### Flash uncached
ARMCLANG 6.24
#### With FPU
===================================================
-O1
MicroSecond for one run through Dhrystone[5-13537]:      6.766
Dhrystones per Second:  147797.812
DMIPS/MHz:      0.584
===================================================
-Ofast
MicroSecond for one run through Dhrystone[5-12384]:      6.189
Dhrystones per Second:  161563.938
DMIPS/MHz:      0.639
===================================================
#### Without FPU
===================================================
-O1
MicroSecond for one run through Dhrystone[5-13189]:      6.592
Dhrystones per Second:  151699.031
DMIPS/MHz:      0.600
===================================================
-Ofast
MicroSecond for one run through Dhrystone[5-12384]:      6.189
Dhrystones per Second:  161563.938
DMIPS/MHz:      0.639
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
#### Without FPU
===================================================
-Ofast
MicroSecond for one run through Dhrystone[4-4908]:       2.452
Dhrystones per Second:  407830.344
DMIPS/MHz:      1.612
===================================================

