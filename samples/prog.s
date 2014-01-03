load r0, $0x10
incr r12
incr r0
incr r0
decr r0
add r0, r0, $3
load r3, $0x08
sub r3, r3, $5
sub r2, r0, r3
syscall 0
