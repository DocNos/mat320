# MAT 320 Project 2 - DFT and FFT Implementation

**Author:** Nixx Varboncoeur  
**Course:** MAT 320 - Digital Signal Processing  
**Instructor:** Matt Klassen  

## Assignment Specifications

### Part II: Discrete Fourier Transform – Basic and Recursive Forms

#### 1. Discrete Fourier Transform (DFT) - Basic Version
- **Input:** command line arg N, text file of N complex numbers
- **Output:** DFT of input as list of N complex numbers

#### 2. Recursive (Fast) Fourier Transform (FFT) - First Version
- **Input:** command line arg N = 2^m, text file of N complex numbers
- **Output:** DFT of input as list of N complex numbers

### Implementation Requirements

1. **File Structure:** Two separate programs required:
   - `dft1.cpp` - Basic DFT implementation
   - `fft1.cpp` - Recursive FFT implementation
   - Both should compile with g++ without requiring additional headers or linked files

2. **DFT Algorithm:** 
   - Use Formula (2.6) on page 152 of textbook
   - Transform inputs x₀, ..., x_{N-1} to outputs X₀, ..., X_{N-1}

3. **FFT Algorithm:**
   - Based on section 6 of chapter 8, pages 162-163 of textbook
   - Recursive function using Formula (6.9) on page 163
   - Identical inputs/outputs to DFT

4. **Recursive Function Guidelines:**
   - VOID function taking: input array, output array, integer N = 2^m
   - Base case (N=1): copy input to output
   - General case (N=2^m): 
     - Create 4 arrays of N/2 complex numbers (even/odd input/output pairs)
     - Assign inputs by even/odd indices
     - Recursive calls for even and odd components
     - Two loops implementing equation (6.9) to combine results
     - Second loop handles simplified exponential factor

### Sample Data
From dftoutput.txt - both implementations should produce identical results for N=8, precision=4.

## Project Structure

```
proj2/
├── src/                          # Source code files
│   ├── dft1.cpp                 # DFT implementation
│   └── fft1.cpp                 # FFT implementation
├── include/                      # Header files
├── bin/                         # Compiled executables
├── obj/                         # Object files
├── input/                       # Test input files
├── output/                      # Expected output files
├── tests/                       # Test suites
├── docs/                        # Documentation
├── Makefile                     # Build system
└── README.txt                   # This file
```

## Building and Running

### Build Commands
```bash
make                    # Build release version
make debug             # Build with debug symbols
make clean             # Remove build artifacts
make help              # Show all available targets
```

### Testing
```bash
make test              # Run all tests
make test-quick        # Run quick smoke tests
make generate-tests    # Generate test files
```

## Implementation Notes

*This section will be updated as development progresses*

## AI Disclosure

*This project may be developed with assistance from Claude Code extension for debugging and optimization support*