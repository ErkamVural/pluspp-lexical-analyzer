# Plus++ Lexical Analyzer

A robust lexical analyzer (scanner) implementation for the Plus++ programming language, written in ANSI C. This tool tokenizes source code according to a custom grammar and supports high-precision integer handling.

Developed as part of the Computer Engineering coursework at Ege University.

---

## The Plus++ Language Specification

The analyzer is built to parse Plus++, a custom procedural language designed specifically for handling large integers.

---

## Core Concepts

### Data Type
The language supports a single data type: `number`.  
This is a signed integer capable of storing up to **100 decimal digits**.  
Floating-point numbers are not supported.

### Variables
All variables are **global** and **static**.  
They are automatically initialized to `0` upon declaration.

### Scope Rules
- Variable names are case-sensitive.
- Maximum identifier length: **20 characters**.

---

## Syntax Overview

### Comments
Multi-line comments are enclosed between asterisk characters:

```
* comment *
```

### Blocks
Code blocks are defined using curly braces and may be nested:

```
{ ... }
```

### Loops
Iteration is handled using the following structure:

```
repeat <count> times { ... }
```

### Input/Output
The `write` statement outputs strings and/or variables separated by the `and` keyword:

```
write "Value:" and variableName;
```

---

## Features

### Big Integer Support
Handles signed integer constants up to 100 decimal digits without overflow.

### Tokenization
Recognizes:
- Keywords (`number`, `repeat`, `times`, `write`, etc.)
- Identifiers
- Operators (`:=`, `+=`, `-=`)
- String literals
- Block symbols (`{`, `}`)

### Error Reporting
Detects lexical errors such as:
- Unclosed comments
- Unterminated string literals
- Identifier length violations (>20 characters)
- Malformed numeric formats
- Invalid characters

### Dependency-Free
Implemented purely in ANSI C using only standard libraries.

---

## Project Structure

```
PlusPlus-Lexical-Analyzer/
│
├── src/
│   └── la.c              # Main lexical analyzer source file
│
├── examples/
│   ├── test1.plus
│   ├── test2.plus
│   └── test3.plus
│
└── README.md
```

---

## Build Instructions

Requires a standard GCC compiler.

```bash
cd src
gcc la.c -o ../la
```

---

## Test Scenarios & Expected Outputs

The following examples demonstrate successful parsing and lexical error detection.

---

### Case 1: Success Scenario (Complex Logic)

**Description:**  
Tests a valid program containing keywords, big integers, comments, and nested blocks.

**Input (`examples/test3.plus`):**

```
number size;
size := 5;
repeat size times *ignore me* { write size and newline; }
```

**Result:**  
Success. Generates `test3.lx` containing tokens such as:

```
Keyword(number)
Identifier(size)
Semicolon
Identifier(size)
Assignment(:=)
Number(5)
...
```

---

### Case 2: Error Handling — Invalid Character

**Description:**  
Detects an unrecognized character within an operation.

**Input (`examples/test1.plus`):**

```
number sayi;
sayi+:=5;
```

**Expected Console Output:**

```
Invalid character '+' detected at 5.line 5.char!
```

---

### Case 3: Error Handling — Malformed Number

**Description:**  
Detects invalid numeric formatting where a number is not properly separated.

**Input (`examples/test2.plus`):**

```
number number_;
number_+=55+;
```

**Expected Console Output:**

```
Invalid number format: number cannot contain '+' at 2.line and 12.char!
```