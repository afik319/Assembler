# C Project: 12-bit Assembler

## Usage

1. Run `make`
2. Run `./assembler fileName1 fileName2 ...`

The assembler will output *fileName.am*, *fileName.ent*, *fileName.ext* and *fileName.ob*.

## Structure

- `assembler.c` - main function definition, argv & argc processing and files processing.
- 
-'pre_assembler.c' - for macros deployment.

-'line_parser.c' - for the parsering and classification of each line part.

-'first_pass.c' - for checking the validation of each line and between the lines.

-'second_pass.c' - for converting each line to its binary and 64-base representation, for validations that could not be done during the first pass, and for the creation of the output files.

-'help_functions.c'

## Computer and Language Structure

### Computer Structure
The imaginary computer consists of CPU, Registers and RAM (some of the RAM is utilized as stack).

The CPU has 8 registers (r0-r7). each register size is 12 bits.

Memory size is 1023 and each memory cell size is 12 bits.

The computer only works with integers.

### Word and Sentence Structure

Each computer instruction adds between 1 to 3 words (of 12 bit) which are encoded in the following manner:

|  opcode |

| first operand  |

|  second operand | 

if there are two registers as operands, there will be only two line:

|  opcode |

| first and second operand | 

This model consists of 16 operations, each of them has a different funct and opcode (*mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts and stop*).

There are 4 kinds of sentences the assembler recognizes:

1. Empty Sentence - A line that only contains whitespaces.

2. Comment Sentence - A line that begins with `;`.

3. Instruction Sentence - Variables assignment and declaration.

4. Command Sentence - Creates an action for the machine to execute upon running the program.

Maximum line length is 82. 

#### macro deployment:

Includes 3 steps:

1. declaration with the word "mcro" and the macro name.
the macro name cannot be instruction name,  it cannot start with a digit, it has to be at most 30 characters long and include only letters and digits.
2. lines of content for that macro.
3. declaration of "endmcro"

for example:
mcro m1
mov @r1,@r3
add @r5, LABEL1
endmcro

Usage of labels is optional. A label is any word (reserved words not allowed) which is declared at the beginning of the sentence and ends with `:`. For example `myLabel:`. It cannot start with a digit, and has to be at most 31 characters long.

#### Instruction Sentence

Instruction Sentence may or may not start with a label. Valid instructions are: 

1. **.data** - declaration of integers. For example: `.data 58, +3, -92`.

2. **.string** - declaration of a string wrapped within *" "*. For example: `.string "This is a string."`.

3. **.extern** - reference to an external label, declared in another file. For example `.extern externalLabel`.

4. **.entry** - reference to an internal label, that must be declared in the program. For example `.entry entryLabel`.

#### Command Sentence

Command Sentence may or may not start with a label. Valid commands are: 

**mov** - copies origin operand to destination

**cmp** - performs comparison between 2 operands

**add** - destination operand receives the addition result of origin and destination

**lea** - load effective address

**clr** - clears operand

**not** - logical not, reverses all bits in operand

**inc** - increments operand's content by 1

**dec** - decrements operand's content by 1

**jmp** - jumps to instruction in destination operand

**bne** - branch if not equal to zero

**jsr** - calls a subroutine

**red** - reads a char from stdin

**prn** - prints char into stdout

**rts** - returns from subroutine

**stop** - stops the program

## The Algorithm

The assembler receives an input in assembly language. It goes over the input twice. The reason for that is because there are references to labels and instructions that have unknown addresses during the first pass.

The assembler implements a dynamic array for spreading the macros. It also implements a Data Structure that joins together 3 dynamic arrays. One is used for the Symbol Table - which stores the addresses of the labels declared. And the other two are used for the Code Image, and Data Image.

The Code Image stores the command sentences in machine code, and the Data Image stores the instruction sentences in machine code.

The machine code is essentially built during first pass, though operands of type "DIRECT" that refer to labels, are updated during the second pass. Through it all, errors are thoroughly checked. If any errors are found during first pass, or second pass, the program prints the errors and continues to the next file. 
