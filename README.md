# simple-compiler

A simple compiler for a simple programming language.

This project was an exercise to better understand compilers and programming language design.

I utilized ChatGPT as a programming partner.

## Language

Currently, the language only supports the following:

- Integer data type only
- Variable assignment
- Addition and subtraction operations
- Print operation

Example program

```c
var1 = 5 + 6;
var2 = 2 - 1;
var3 = var1 - var2;
print(var3);

// Prints 10
```

## Getting Started

Build the compiler:

```bash
make compiler
```

Compile a program:

```bash
./compiler sample_program.txt sample_program
```

Run the program:

```bash
./sample_program
```

> Note: a temporary file called `temp.ll` will output to the current directory.
> This can be safely deleted.
