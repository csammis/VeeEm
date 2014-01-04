load r0, $0xFF
load r1, $0xEE
decr r0
comp r0, r1
jne $-0x02
syscall 0

load r2, $0xAF
load r3, $0xB0
incr r2
comp r2, r3
je $-0x02
syscall 0

load r4, $0xAE
load r5, $0xB0
incr r4
comp r4, r5
jlte $-0x02
syscall 0

load r6, $0xC0
load r7, $0xD0
incr r6
comp r6, r7
jlt $-0x02
syscall 0

load r8, $0xD0
load r9, $0xC0
decr r8
comp r8, r9
jgt $-0x02
syscall 0

load r10, $0xD0
load r11, $0xC0
decr r10
comp r10, r11
jgte $-0x02
syscall 0
