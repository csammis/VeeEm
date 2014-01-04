# VeeEm
A litle virtual machine I knocked together in C++. It's completely not useful and right now I'm not even trying to pioneer any new ideas. Just keeping up my system programming skills!

## Features
32 32-bit general purpose registers (guaranteed initialized to zero at execution start) and a RISCy instruction set.

## Syntax
The VM language is being created more or less off the top of my head and is loosely based on what I recall from RISC architecture, particularly the Texas Instruments MSP430.

### Instructions
An instruction is one instruction name followed by a space and a sequence of zero or more parameters separated by commas. The parameter list length is dependent on the instruction. Program parsing fails if an instruction is specified with too few or too many parameters.

The following sections describe the syntax of the various types of parameters. Note that not every instruction supports every kind of parameter; for example the conditional jump instructions don't support reading offsets from a register.

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

    Syntax: `add dest, src1, src2`

    Example:

        load r1, $4
        load r2, $3
        add r0, r1, r2

    The `add` instruction computes `src1 + src2` and stores the result in `dest`. `src1` and `src2` may be registers or constants. `dest` must be a register.

*   **comp** - Compares two values and sets a flag in the current execution context

    Syntax: `comp val1, val2`

    Example:

        load r1, $4
        load r2, $3
        comp r1, r2

    The `comp` instruction is used to set internal flags for the next conditional jump instruction. `val1` and `val2` may be registers or constants.

*   **decr** - A synonym for `sub dest, dest, 0x01`
    
    Syntax: `decr dest`

    Example:

        load r1, $3
        decr r1

*   **incr** - A synonym for `add dest, dest, 0x01`
    
    Syntax: `incr dest`

    Example:

        load r1, $3
        incr r1

*   **load** - Load a value into a register

    Syntax: `load dest, value`

    Example:

        load r1, $0x12345678
        load r2, r3

    `value` may be a register or a constant. `dest` must be a register.

*   **sub** - Subtracts one value from another and sets the result in a register

    Syntax: `sub dest, src1, src2`

    Example:

        load r1, $4
        load r2, $3
        sub r0, r1, r2

    The `sub` instruction computes `src1 - src2` and stores the result in `dest`. `src1` and `src2` may be registers or constants. `dest` must be a register.

*  **syscall** - Invoke a system-defined procedure

    Syntax: `syscall index`

    Example:

        load r1, $4
        syscall 0

    `index` is the zero-based index of the system call to invoke. See [System Calls](#system-calls) for the list of supported system calls.


#### Conditional Jumps
Jump to an offset if the last values by a `comp` instruction were:

*   **je** - equal

*   **jne** - not equal

*   **jlt** - `val1` less than `val2`

*   **jgt** - `val1` greater than `val2`

*   **jlte** - `val1` less than or equal to `val2`

*   **jgte** - `val1` greater than or equal to `val2`

Syntax: `je offset`

Example:

    load r1, $0x01
    comp r1, $0x01
    je $-2

`offset` is a constant value representing *number of instructions from current instruction*. In the preceding example the `load` instruction will be the next instruction executed after the jump.

Executing a conditional jump instruction without first executing a `comp` instruction results in a runtime error. Successful execution of a conditional jump clears the `comp` internal flags so that a subsequent conditional jump will require a new `comp` to have been performed. 

#### System Calls

*   **DUMPCONTEXT**

    Call index: 0

    Prints a formatted list of the current execution context's registers and flags to the console. This system call is automatically invoked when a runtime error occurs before the system halts.
