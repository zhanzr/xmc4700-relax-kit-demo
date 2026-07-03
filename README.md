# XMC4700F144K2048 Relax Kit test.

LED + UART.

## CRC hardware/software performance test.
```
start to test
Step 1: Performs a CRC32 check using Kernel 0 on Usecase1_Data1
Result: 6F2C8A1D 6F2C8A1D
Result: 4EBC757C 4EBC757C
Result: B85134F6 6F2C8A1D
Result: 3EAE3D72 4EBC757C
Result: 90D375E2 6F2C8A1D
Result: B1438A83 4EBC757C
Result: 47AECB09 6F2C8A1D
Result: C151C28D 4EBC757C
Result: 073D7BE3 073D7BE3
Result: 26AD8482 26AD8482
Result: C7DEBCE0 073D7BE3
Result: 4121B564 26AD8482
Result: F8C2841C 073D7BE3
Result: D9527B7D 26AD8482
Result: 3821431F 073D7BE3
Result: BEDE4A9B 26AD8482

Soft CRC32 IEEE802.3 Result: 4121B564

Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA
Result: B2EF92DA

Step 2: Performs a CRC32 check using Kernel 1 on Usecase1_Data1
Result: BB8D49A6
Step 3: Performs a CRC32 check using Kernel 1 on Usecase1_Data2
Result: 8F2D7440
Step 4: Performs a CRC16 check using Kernel 2 on Usecase1_Data3.
Lorem ipsum dolor sit amet,
consectetur adipiscing elit. Donec metus eros, a
ccumsan ut vestibulum id, suscipit nec augue. Aenean et lectus ut libero lacinia faucibus ut vel lectus. Pellentesque iaculis libero ac lectus blandit accumsan. Pellentesque at nulla eget metus aliquam tincidunt. Aenean cursus purus vitae lacus cursus pharetra. In hac habitasse platea dictumst. Curabitur nulla odio, porttitor eu pharetra at, pulvinar gravida velit. Aenean eu dapibus purus. Proin blandit feugiat urna, at iaculis elit accumsan ut. Pellentesque molestie pharetra erat, eget semper urna vehicula nec. Nam tristique sollicitudin diam, faucibus aliquet velit pharetra a. Duis a placerat risus. Phasellus vel diam nibh, quis elementum nisl. Phasellus lectus quam, mollis eu consequat ut, bibendum eget arcu. Nullam at felis a elit auctor suscipit eu quis ipsum. Mauris luctus, diam sit amet iaculis malesuada, urna orci convallis tellus, vitae molestie diam justo sed metus. Etiam volutpat volutpat justo, vel facilisis mi eleifend ut. Aenean egestas, sem eu vulputate lacinia, odio ligula mollis risus, a semper eros risus sed arcu. Sed feugiat augue eget erat bibendum vitae consequat purus tempus. Morbi lobortis nunc eget ligula vehicula non pharetra dolor commodo. Pellentesque ligula nibh, eleifend blandit aliquam vel, euismod non tellus. Quisque dictum laoreet feugiat. Maecenas a varius sapien. Ut semper nulla id turpis cursus ornare. Nullam quis erat et augue imperdiet pharetra nec a sem. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aenean feugiat, orci ultricies pellentesque viverra, nisl elit molestie augue, et scelerisque risus felis nec nulla. Maecenas congue arcu ac lectus bibendum at lacinia elit tristique. Cras fringilla vestibulum lectus. Praesent quis nisi turpis, sed tristique sem. Nam adipiscing posuere faucibus. In iaculis placerat semper. Curabitur in nunc quis enim vehicula aliquam quis at sapien. In hac habitasse platea orci aliquam...
abcd
Result: 0C7E
engine:50020060, cfg:00000000, init:0000FFFF
Result: 220C
engine:50020060, cfg:00000000, init:0000FFFF
Result: DDDF
engine:50020060, cfg:00000000, init:0000FFFF
Result: 2CF6
engine:50020060, cfg:00000000, init:0000FFFF
Result: F90A
Ref value:0C7E
Ref value:220C
Ref value:DDDF
Ref value:2CF6
Ref value:F90A
Step 5: Performs a CRC8 check using Kernel 3 on Usecase1_Data4.
Result: 000000BE
Step 6: Trigger a mismatch flag.
CRC32 Performance benchmark.
Result: 8D25B0FF [2300-2046]=254

Soft CRC32 IEEE802.3 Result: 8D25B0FF [3533-2301]=1232

mismatch flag is triggered
Error_Handler 107
```

