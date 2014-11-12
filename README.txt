Simple MIPS assembler using FLEX/BISON and C. 
Implemented instructions on types.txt, also grouped by their "subtypes" under R/I/J-Types.

v0.1
Now with error handling and helpful error messages.
Still no binary though.

v0.0
Parsing text and checking syntax. Still pretty simple and errors are not handled.
Still isn't generating binary code.
Makefile included.
Compilation: make (make flex would be used for flex debugging, but the debugging feature needs "debugging" :D).
Usage: ./asm FILE (It will spit to STDOUT the instructions read on each step and some rules).
