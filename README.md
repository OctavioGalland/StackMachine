# Overview

The project was divided into two parts, an assembler for the instruction set, and the machine which can execute binaries produced by the assembler.

### Instruction Set Architecture

The instruction size is variable, opcodes are always 8 bits long and if the first bit of the opcode is 1 that means it takes one 32-bit operand and the instruction itself is 5 bytes long. This results in a trade-off between instruction decoding complexity and binary size. Each instruction was given an opcode starting from `0x01` (in the case of operation with no operands) or `0x81` (in the case of operations with only one operand). The correspondance between opcodes and opcode values is as follows:

| Operation | Opcode Number | Description |
| -- | :--: | --: |
|  READ  | 0x01 | Reads a value from stdin and pushes it into the stack|
| WRITE | 0x02 | Pops a value from the stack and writes it into stdout |
| DUP | 0x03 | Duplicate the top value of the stack |
| MUL | 0x04 | Pops two values from the stack, multiplies them and pushes the result into the stack |
| ADD | 0x05 | Pops two values from the stack, adds them and pushes the result into the stack  |
| SUB | 0x06 | Pops two values from the stack, subtracts the second to top value from the top value, and pushes the result into the stack |
| GT | 0x07 | Pops two values from the stack, and pushes a `1` if the top value was greater than the second value |
| LT | 0x08 | Pops two values from the stack, and pushes a `1` if the top value was less than the second value |
| EQ | 0x09 | Pops two values from the stack, and pushes a `1` if the two values are equal |
| JMPZ | 0x0A | Pops two values from the stack, and jumps to the top value if the second value is zero |
| OFF | 0x0B | Terminates execution |
| PUSH | 0x81 | Pushes a 32 bit signed number in the stack |
| POP | 0x82 | Pops an arbitrary amount of numbers from the stack |
| ROT | 0x83 | Rotates the top N values of the stack towards the top of the stack |

With only the last 3 opreation requiring an operand. All operands and results are stored in little-endian.

The memory is byte-adderssable though arbitrary memory accessing is not allowed.

### Assembler and assembly language

The assembler basically reads each line up to a `'#'` character (if one is found) discarding any whitespace found at either the beginning or end of the line. The delimiter between instructions is always a line break (though empty lines are not forbidden) and the delimiter between operator and operand can be a stream of either tabs or spaces (though this can be customized quite easily).

The code is parsed one first time splitting each non-empty line into the corresponding words, checking that each instruction number is valid, saving labels references (any non-numeric string where an operand is expected is assumed to be a label), saving label definitions and also checking that operands are supplied when necessary. After the first pass, an iteration is made through all the unresolved labels and errors are produced accordingly.
It is worth noting that the assembler produces a binary which will only work if loaded at address `0x00000000` (it could be stored elsewhere if the machine used virtual memory).

### Machine

The machine is intended to imitate how a real proccessor would work (at least, on a conceptual level), so both the program and the stack are stored in the same linear array which is considered it's memory, and has a set of 'registers' that can address up to `4GB` of ram. Although both the program and it's data are stored in the same memory, no segmentation/paging was used, so a poorly/maliciously written program could overwrite it's own code by pushing enough values on to the stack.

# Usage

### Copmilation

Both projects are stored in their corresponding subdirectories whithn the project's root (namely, `./Assembler/` and `./Machine/`). To build both projects it's enough to run `make` from within the project's folder, the same goes for `make clean`.

#### Portability

The code was compiled and tested locally using g++ with the C++11 standard and GNU extensions disabled, so it should be portable, but other compilers were not tested. Also, the code only uses standard C++ features, so it should also be portable accross operating systems as well.

### Assembly

To assemble a source file, just run `./Assembler/assembler input.asm output.bin`. The output parameter can be ommited, if so, the result will be placed in `a.out` at the folder from which the binary was executed.

### Running programs

To run a program use `./Machine/machine input.bin`. The input file must be a valid series of instructions stored as raw bytes. If hand-writing binary code remember the binary will always be loaded at address `0x00000000` and the stack pointer initially has the maximum memmory available as it's value (as before writing into the stack, the stack pointer needs to be decremented, this doesn't result in errors as long as the amount of popped values is less than or equal to the ammount of pushed values).

The machine also supports an optional parameter `-m` which allows the user to indicate the desired amount of RAM (in bytes) to allocate for the machine. The default value for this parameter is `4096` bytes.

A little example program is given that reads an input from the stack, and prints -1 if it less than 0, or 1 otherwise.
