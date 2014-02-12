PutStr:
    pop r1
    comp r1, $0x00
    je $0x04
    push r1
    syscall 1
    call :PutStr
    return

.entry
    # The string is assembled backwards due to this recursive PutStr definition
    push $0x00
    push $0x0A
    push $0x21
    push $0x64
    push $0x6C
    push $0x72
    push $0x6F
    push $0x77
    push $0x20
    push $0x6F
    push $0x6C
    push $0x6C
    push $0x65
    push $0x48
    call :PutStr
