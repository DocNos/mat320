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
│   ├── dft/                     # DFT implementation
│   │   ├── dft1.h              # DFT class definition  
│   │   ├── dft1.cpp            # DFT algorithm implementation
│   │   └── main.cpp            # DFT main program
│   └── fft/                     # FFT implementation
│       ├── fft1.h              # FFT class definition
│       ├── fft1.cpp            # Recursive FFT algorithm
│       └── main.cpp            # FFT main program
├── bin/                         # Compiled executables
│   ├── dft1                    # DFT program executable
│   └── fft1                    # FFT program executable
├── obj/                         # Object files
│   ├── dft/                    # DFT object files
│   └── fft/                    # FFT object files
├── input/                       # Test input files
│   ├── Input01.txt             # Test case 1 (8 complex numbers)
│   └── Input02.txt             # Test case 2 (8 complex numbers)
├── output/                      # Expected output files
│   ├── Output01.txt            # Expected results for test 1
│   └── Output02.txt            # Expected results for test 2
├── claudeLog/                   # AI collaboration session logs
├── Makefile                     # Build system
├── dftoutput.txt               # Original assignment sample data
└── README.txt                   # This file
```

## Building and Running

### Build Commands
```bash
make                    # Build both programs (release)
make both              # Build both dft1 and fft1 
make dft1              # Build DFT program only
make fft1              # Build FFT program only
make debug             # Debug build with symbols
make clean             # Remove build artifacts
make help              # Show all available targets
```

### Testing
```bash
make test              # Run comprehensive test comparison
make test-dft          # Test DFT implementation only
make test-fft          # Test FFT implementation only
make test-both         # Test both programs and compare outputs
make test-quick        # Run quick smoke tests
```

### Usage Examples
```bash
# Run DFT on test data
./bin/dft1 input/Input01.txt 8

# Run FFT on same test data  
./bin/fft1 input/Input01.txt 8

# Both should produce identical output
```

## Implementation Status

### ✅ DFT Implementation (Complete)
- **Algorithm**: Basic O(N²) discrete Fourier transform
- **Input parsing**: Handles complex number format (e.g., "1+2i", "-3-4i")
- **Output formatting**: Dynamic precision (integers without decimals, 3 decimal places when needed)
- **Testing**: Produces correct results matching expected outputs
- **File**: `src/dft/dft1.cpp` (90 lines)

### ✅ FFT Implementation (Complete)  
- **Algorithm**: Recursive Cooley-Tukey FFT, O(N log N)
- **Constraint**: Requires N = 2^m (power of 2)
- **Base case**: N=1 returns input unchanged
- **Recursive structure**: Divides into even/odd indices
- **Twiddle factors**: Uses e^(-2πik/N) complex exponentials
- **Testing**: Produces identical output to DFT implementation
- **File**: `src/fft/fft1.cpp` (97 lines)

### ✅ Build System (Complete)
- **Separate executables**: Individual dft1 and fft1 programs
- **Modular structure**: Each program in its own subdirectory
- **Comprehensive testing**: Individual and comparative testing
- **Clean architecture**: Follows genStandards conventions

## AI Disclosure

*This project may be developed with assistance from Claude Code extension for debugging and optimization support*