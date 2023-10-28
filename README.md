# CrazeCC Compiler

CrazeCC is a hobby compiler project written in C++20. It is a simple compiler designed to handle a limited set of C++-like language features. Please note that this project is in an unfinished and abandoned state.

## Language Features

CrazeCC supports the following language features:

- **Functions**: You can define and call functions in your code.

- **Function Calls**: You can call functions from within your code.

- **Preprocessor**: The compiler includes basic preprocessor functionality.

- **`#include` Directive**: The preprocessor only supports `#include` for including external files.

- **Return Values**: Functions can return values, and you can use these return values in your code.

- **Variables**: You can declare and use variables.

- **Variable Types**: CrazeCC supports three variable types: `char`, `bool`, and `int`.

- **If Statements**: You can use `if` statements for conditional branching in your code. Note that there is no support for `else` or `else if` statements.

## Getting Started

To build and use CrazeCC, follow these steps:

Clone the repository:
   ```markdown
   git clone https://github.com/ScriptedDeveloper/crazecc.git
   cd CrazeCC
   cmake . && make 
   ```
## Usage
1. Compile CrazeCC:
    ```
    ./CrazeCC source_file -o output
    ```
2.  Call x86-64 assembler:
    ```
    nasm -f elf64 output 
    ```
3.  Call GCC to link executeable & construct _start function
    ```
    gcc nasm_output
    ```
4. Run!
    ```
    ./a.out
    ```


# CrazeCC Compiler

CrazeCC is a hobby compiler project written in C++20. It is a simple compiler designed to handle a limited set of C++-like language features. Please note that this project is in an unfinished and abandoned state.

## Language Features

CrazeCC supports the following language features:

- **Functions**: You can define and call functions in your code.

- **Function Calls**: You can call functions from within your code.

- **Preprocessor**: The compiler includes basic preprocessor functionality.

- **`#include` Directive**: The preprocessor only supports `#include` for including external files.

- **Return Values**: Functions can return values, and you can use these return values in your code.

- **Variables**: You can declare and use variables.

- **Variable Types**: CrazeCC supports three variable types: `char`, `bool`, and `int`.

- **If Statements**: You can use `if` statements for conditional branching in your code. Note that there is no support for `else` or `else if` statements.

## Getting Started

To build and use CrazeCC, follow these steps:

Clone the repository:
   ```markdown
   git clone https://github.com/ScriptedDeveloper/crazecc.git
   cd CrazeCC
   cmake . && make 
   ```
## Usage
1. Compile CrazeCC:
    ```
    ./CrazeCC source_file -o output
    ```
2.  Call x86-64 assembler:
    ```
    nasm -f elf64 output 
    ```
3.  Call GCC to link executeable & construct _start function
    ```
    gcc nasm_output
    ```
4. Run!
    ```
    ./a.out
    ```



