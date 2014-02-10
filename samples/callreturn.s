DoSyscall:
    syscall 0
    return

.entry
load r1, $3
call :DoSyscall
load r2, $5
call :DoSyscall
