# VeeEm
A litle virtual machine I knocked together in C++. It's completely not useful and right now I'm not even trying to pioneer any new ideas. Just keeping up my C++ and system programming skills!

## Features
32 32-bit general purpose registers (guaranteed initialized to zero at execution start) and a RISCy instruction set.

## Syntax
The VM language is being created more or less off the top of my head and is loosely based on what I recall from RISC architecture, particularly the Texas Instruments MSP430.

### Instructions
An instruction is one instruction name followed by a space and a sequence of zero or more parameters separated by commas. The parameter list length is dependent on the instruction. Program parsing fails if an instruction is specified with too few or too many parameters.

The following sections describe the syntax of the various types of parameters. Note that not every instruction supports every kind of parameter; for example the `jump` instructions don't support reading offsets from a register.

#### Registers
Registers are specified by strings starting with the letter `r` and followed by a decimal integer 0 through 31.

```
comp r0, r1
incr r22
```

#### Constants
Constants are integers designated by strings prefixed with a '$' and followed by anything that `strtol(3)` can parse with a base of 0 (decimal, octal, or hexadecimal).

```
load r0, $0xEEFF
load r1, $-3
```

## Instruction Set
*   **add** - Adds together two values and sets the result in a register

    `add dest, src1, src2`

        load r1, $4
        load r2, $3
        add r0, r1, r2

    `src1` and `src2` may be registers or constants. `dest` must be a register.

*   **incr** - A synonym for `add dest, dest, 0x01`
    
    `incr dest`

        load r1, $3
        incr r1
