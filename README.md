# XMC4700F144K2048 Relax Kit test.

LED + UART.

## FPU hardware/software performance test.

144 MHz

### Flash cached

ARMCLANG 6.24
#### With FPU
-O1
Correct CMSIS DSP       227
Correct Single precision        520
Correctdouble precision 9395

-Ofast
Correct CMSIS DSP       227
Correct Single precision        524
Correct double precision        9411

#### Without FPU
-O1


-Ofast
Correct CMSIS DSP       227
Correct Single precision        2856
Correct double precision        8653