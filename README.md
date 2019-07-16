XMC4700F144K2048 Relax Kit test.

===================================================
using flash address from 0x8000000
144 MHz, Standard LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[3-5477]:       2.737 
Dhrystones per Second:  365363.531 
DMIPS/MHz:      1.444
====================================================
using flash address from 0x8000000
144 MHz, Micro LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[4-7491]:       3.743 
Dhrystones per Second:  267129.688 
DMIPS/MHz:      1.056
====================================================

===================================================
using flash address from 0xC000000
144 MHz, Standard LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[4-13216]:      6.606 
Dhrystones per Second:  151377.531 
DMIPS/MHz:      0.598
====================================================
using flash address from 0xC000000
144 MHz, Micro LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[5-18538]:      9.266 
Dhrystones per Second:  107915.609 
DMIPS/MHz:      0.427
====================================================

===================================================
using flash address from 0x1FFE8000
144 MHz, Standard LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[3-5088]:       2.543 
Dhrystones per Second:  393313.656 
DMIPS/MHz:      1.555
====================================================
using flash address from 0x1FFE8000
144 MHz, Micro LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[4-7172]:       3.584 
Dhrystones per Second:  279017.844 
DMIPS/MHz:      1.103
====================================================



CoreMark

PSRAM
XMC4700 ARMCC Test @ 144000000 Hz
144000000 Hz, 410FC241, CM:4, FPU_USED:1, SCU_IDCHIP:00047001
Boot Mode:0, Vector:1FFE8000
RTE_Compiler_IO_STDOUT_User
With StandardLib
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 2628
Total time (secs): 26.280000
Iterations/Sec   : 380.517504
Iterations       : 10000
Compiler version : ARMCC 5060750
Compiler flags   : --c99 -c --cpu Cortex-M4.fp -g -O3 -Otime
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 380.517504 / ARMCC 5060750 --c99 -c --cpu Cortex-M4.fp -g -O3 -Otime / STAC


Cached Flash
XMC4700 ARMCC Test @ 144000000 Hz
144000000 Hz, 410FC241, CM:4, FPU_USED:1, SCU_IDCHIP:00047001
Boot Mode:0, Vector:08000000
RTE_Compiler_IO_STDOUT_User
With StandardLib
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 2628
Total time (secs): 26.280000
Iterations/Sec   : 380.517504
Iterations       : 10000
Compiler version : ARMCC 5060750
Compiler flags   : --c99 -c --cpu Cortex-M4.fp -g -O3 -Otime
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 380.517504 / ARMCC 5060750 --c99 -c --cpu Cortex-M4.fp -g -O3 -Otime / STAC


UnCached Flash
XMC4700 ARMCC Test @ 144000000 Hz
144000000 Hz, 410FC241, CM:4, FPU_USED:1, SCU_IDCHIP:00047001
Boot Mode:0, Vector:0C000000
RTE_Compiler_IO_STDOUT_User
With StandardLib
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 7564
Total time (secs): 75.640000
Iterations/Sec   : 132.205182
Iterations       : 10000
Compiler version : ARMCC 5060750
Compiler flags   : --c99 -c --cpu Cortex-M4.fp -g -O3 -Otime
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x988c
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 132.205182 / ARMCC 5060750 --c99 -c --cpu Cortex-M4.fp -g -O3 -Otime / STAC

