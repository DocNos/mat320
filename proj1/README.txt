# MAT 320 Project 1 - Complex Number Operations with Roots of Unity

**Author:** Nixx Varboncoeur  
**Course:** MAT 320 - Digital Signal Processing  
**Instructor:** Matt Klassen  

## Overview

This C++ command-line program performs complex number operations involving roots of unity, supporting four distinct mathematical operations commonly used in digital signal processing.

## Project Structure

mat320/proj1/
├── src/
│   ├── main.cpp                 # Command-line interface
│   ├── complex_operations.h     # Class definitions
│   └── complex_operations.cpp   # Implementation
├── input/                       # Input test files (generated)
├── output/                      # Expected output files (generated)
├── bin/                         # Compiled executables
├── obj/                         # Object files
├── Makefile                     # Build system
└── README.md                    # This file

## Features

### Part 1: Complex Number Rotation
Rotates N complex numbers by angle 2πx, where:
- x > 0: counterclockwise rotation
- x < 0: clockwise rotation

**Usage:** `./bin/complex_calc 1 N x input.txt`

### Part 2: Sum of Powers of Roots of Unity
Calculates: 1 + e^(i2π/N) + (e^(i2π/N))² + ... + (e^(i2π/N))^(k-1)

**Usage:** `./bin/complex_calc 2 N k`

### Part 3: Complex Inner Product
Computes the complex inner product of two vectors from input files.

**Usage:** `./bin/complex_calc 3 N vec1.txt vec2.txt`

### Part 4: DFT Component
Calculates the inner product of an input vector with the roots of unity vector:
(1, e^(i2π/N), (e^(i2π/N))², ..., (e^(i2π/N))^(N-1))

**Usage:** `./bin/complex_calc 4 N input.txt`

## Building and Running

### Prerequisites
- C++17 compatible compiler (g++)
- Make build system

### Build Commands
make                    # Build release version
make debug             # Build with debug symbols  
make clean             # Remove build and test files
make distclean         # Full cleanup including directories


### Generate Test Files
make test-files                  # Create input and output files
make generate-test-files         # Create input test files
make generate-instructor-files   # Create instructor test files (f1.txt, f2.txt)
make generate-output            # Create expected output files

### Testing
make test              # Run all comprehensive tests
make test-basic        # Run basic tests only
make test-instructor   # Run instructor-provided test cases
make test1             # Test Part 1 (rotation) only
make test2             # Test Part 2 (sum of powers) only
make test3             # Test Part 3 (inner product) only
make test4             # Test Part 4 (DFT component) only

## Input File Format

Complex numbers should be formatted as:
1.0 + 2.0i
-3.5 - 1.2i
0.0 + 4.0i

## Output Format

Results are displayed in the same format as input:
2.0 + 1.0i
-1.2 - 3.5i
4.0 + 0.0i

## Examples

### Example 1: Rotation
# Rotate 3 complex numbers by π/2 (90° counterclockwise)
./bin/complex_calc 1 3 0.25 input/test1.txt

### Example 2: Sum of Powers
# Sum first 5 powers of 8th root of unity
./bin/complex_calc 2 8 5

### Example 3: Inner Product
# Inner product of two 3-element vectors
./bin/complex_calc 3 3 input/vec1.txt input/vec2.txt

### Example 4: DFT Component
# DFT component of 4-element signal
./bin/complex_calc 4 4 input/signal.txt

## AI Disclosure

This project was developed with assistance from Claude Code extension for debugging and code optimization support.