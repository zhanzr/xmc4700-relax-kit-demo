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
MicroSecond for one run through Dhrystone[4-5074]:       2.535 
Dhrystones per Second:  394477.312 
DMIPS/MHz:      1.559 TickHz:1000
====================================================
using flash address from 0x1FFE8000
144 MHz, Micro LibC
-O3 -Otime
MicroSecond for one run through Dhrystone[4-7158]:       3.577 
Dhrystones per Second:  279563.875 
DMIPS/MHz:      1.105 TickHz:1000
====================================================
TickHz: 100
144 MHz, Standard LibC
MicroSecond for one run through Dhrystone[0-510]:        2.550 
Dhrystones per Second:  392156.875 
DMIPS/MHz:      1.550 TickHz:100
====================================================
144 MHz, Micro LibC
MicroSecond for one run through Dhrystone[0-718]:        3.590 
Dhrystones per Second:  278551.531 
DMIPS/MHz:      1.101 TickHz:100
====================================================

