*** Compile:
gcc *.c -std=c11 -o pl.exe

*** Run:
./pl.exe <file_name>
E.g:
./pl.exe test.pl0

*** Output:
- Results will show up on screen directly.
- 2 files: .asm (text file), .bin (binary file)

*** Note:
- Name of file must contain extension .pl0
- MAX CODE and MAX DATA: 3000 (Interpreter.h)
- Write, Writeln, Read, Readln have 1 and only 1 parameter, that's an expression.
- The index of array starts with 0.
- Interpreter.h and Interpreter.c has been modified for supporting mod operator (%).