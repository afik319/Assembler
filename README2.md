# Assembler
run:
To run the program, follow these steps:

1. Open a terminal window and navigate to the directory where the program files are located.
2. Run the make command to compile the program.
3. Run the ./assembler command followed by the name of the files you want to assemble.
The files should be in the .as format and the input should be provided without the .as suffix in the terminal.

If there is no errors in the file, the program will generate 4 output files in the same directory:
1. .am - Contains the macros from the original file spread, comment lines (starting with ;) deleted, and empty lines deleted.
2. .ob - Contains the binary code of the source code.
3. .ex - Contains the extern symbols in the source code.
4. ent - Contains the entry symbols in the source code.
If the files has errors the assembler won't produce any output file except the .am file, and will print to stdout  all the errors if found. 

Fundamental Project Guidelines:

A macro invocation can only occur when the macro's name is present within the line (possibly accompanied by white spaces).

During the preliminary assembly stage, if an error is identified in the macro declaration, such as an invalid name, the program will terminate and display an appropriate error message.

If a line exceeds 82 characters in length, this will be detected in the preliminary assembly stage. An appropriate notification will be issued, and the program will halt at the pre-assembler stage. Consequently, any errors from the initial pass will remain concealed until this error is rectified.

A symbol of type " within .string (e.g., .string "abc"def") is deemed an error.

If a label is declared twice, either as .entry V or .extern V, it will not generate an error since it has no immediate logical impact.

An error will be raised if a label is declared both as .entry and .extern.
It is permissible to define a label as V: and subsequently declare it as .entry V.

Particular label-related errors will only emerge following the correction of prior errors, as they may have a chronological sequence.

The declaration of a label as .extern without subsequent utilization is permissible.

Occasionally, a secondary error may manifest following the correction of the initial error. For instance, if a line is initially written as .data a 5,3,-3, an error will manifest for the missing comma. After rectifying it to .data a,5,3,-3, an error will arise due to the presence of a character in lieu of a numeric value.

The assembler we have constructed scrutinizes numerous types of errors and issues error messages correspondingly. 

Illustrated below are several examples of errors identified within the provided files:

In "test1":
Invalidation of the initial word with an illicit initial character - Line 1
Label mirroring an instruction's name - Line 2
Label mirroring a register's name - Line 3
Unlawful addressing method - Lines 4 and 5
Absence of a comma subsequent to an instruction - Lines 6-8
Absent comma between operands - Line 9
Excess operands - Line 10
Insufficient operands - Lines 11-13
Extra characters post .string "content" - Line 14
Dual consecutive commas - Line 15
Missing comma between operands - Line 16
Vacant instruction - Lines 17-21
Invalid initial word that does not denote a label - Line 22
Excessive label name length - Line 23
Label name containing prohibited characters - Line 24
Label name commencing with a non-alphabetical character - Line 25
Invalid argument value (immediate value exceeding 512) - Line 26
Data value exceeding permissible range - Line 27

Test2:
An instruction with an invalid register name: @r9 - Line 1
A comma after the instruction name: Line 2

Test3:
Characters found between .string and the string content - Line 15

Label1:
Definition of two labels with the same name - Line 6

Label3 and Label2:
Declaration of a label as extern in the file - Line 2

Label4:
The first word is too long - Line 1
Defining a label as both extern and entry in the same file - Line 3

Label5:
Declaration of a label with an invalid name in .entry or .extern format - 
Lines 1-2

Label6:
Warning about a label declared as entry but not defined in the file - Line 2
(Note: This warning will only appear if no errors were detected during the first pass.)

Mcro1:
A macro name that includes more than one word - Line 5

Mcro2:
A macro name that is the same as an instruction name - Line 12

Mcro3:
Defining a macro when a macro with the same name is already defined - Line 1

Mcro4:
A line too long in the assembly file (over 82 characters) - Line 16
Additionally:

A file name that is too long: You can check this by attempting to call a program with a file name longer than 255 characters (at least 256).

A file name that doesn't exist: You can check this by providing a non-existent file name in the terminal.
For "w1," "w2," and "w3," there are three potential issues:

Exceeding memory limits due to ic.
Exceeding memory limits due to dc.
Exceeding memory limits due to ic+dc.
