# MAT320 Project 3 - Fast Fourier Transform

**Author:** Nixx Varboncoeur
**Course:** MAT320 - Digital Signal Processing
**Date:** 10_17-25

---

## Project Overview

This project implements and compares three methods for computing the Discrete Fourier Transform (DFT):

1. **Direct DFT** - O(N²) complexity, brute force approach
2. **Recursive FFT** - O(N log N) complexity using Cooley-Tukey algorithm (divide and conquer)
3. **Non-Recursive FFT** - O(N log N) complexity using in-place bit reversal and butterfly operations

The implementation demonstrates the dramatic performance improvement achieved by the Fast Fourier Transform algorithm, showing approximately **93x speedup** for N=1024.

---

## Directory Structure

```

├── bin/                    # Compiled executables (bitrev, fft2)
├── bitrev/                 # Bit reversal implementation
│   ├── bitrev.cpp         # Bit reversal main program
│   └── bitrev.h           # Header file
├── fft2/                   # FFT implementations
│   ├── fft2.cpp           # Main FFT source (all three algorithms)
│   └── fft2.h             # Header with FFT2, FFT, DFT, Timing classes
├── scripts/                # Python utilities for testing
├── claudeLog/              # Development session logs
├── Makefile               # Build system
├── timing.txt             # Performance comparison results
└── README.md              # This file
```

---

## Quick Start

### Prerequisites
- C++ compiler with C++17 support (g++ recommended)
- Make utility
- Linux/Unix environment (or WSL on Windows)

### Building the Project

```bash
# Build all executables
make

# Or build individually
make bin/bitrev
make bin/fft2

# Clean and rebuild
make clean
make
```

### Running the Programs

#### 1. Bit Reversal Test
```bash
./bin/bitrev <N>

# Example: Generate bit-reversed order for N=8
./bin/bitrev 8
```

#### 2. Non-Recursive FFT
```bash
./bin/fft2 <N> <input_file>

# Example: Compute FFT for 8 samples
./bin/fft2 8 input/sample_input.txt
```

#### 3. Timing Comparison
```bash
# Run all three algorithms and compare performance
./bin/fft2 --timing

# Or use Makefile target
make test-timing
```

---

## Input File Format

Input files should contain complex numbers in the format:
```
real imag
real imag
...
```

Example (`input/sample_input.txt`):
```
1.0 0.0
2.0 0.0
3.0 0.0
4.0 0.0
5.0 0.0
6.0 0.0
7.0 0.0
8.0 0.0
```

**Note:** N must be a power of 2 (8, 16, 32, 64, 128, 256, 512, 1024, etc.)

---

## Output

### FFT Output
The program outputs the FFT result as complex numbers:
```
real imaginary
real imaginary
...
```

### Timing Output
Timing results are written to `timing.txt`:
```
Timing Comparison for N=1024
=============================
Direct DFT:          0.027937 seconds
Recursive FFT:       0.000300 seconds
Non-Recursive FFT:   0.000096 seconds

Speedups:
=============================
Recursive Fast Fourier Transform is 9312.33% faster than a direct DFT
In place FFT with leveraging bit shifting is 312.50% faster than the Recusive FFT
```

---

## Algorithm Implementation

### Non-Recursive FFT (In-Place)
The main contribution of this project is the non-recursive FFT implementation in `fft2/fft2.cpp`:

1. **Bit Reversal**: Reorders input array indices using bit-reversed permutation
2. **Butterfly Operations**: Combines smaller DFTs into larger ones using twiddle factors
3. **Stages**: log₂(N) stages, each processing blocks of increasing size (2, 4, 8, ..., N)

**Key Optimization:** Mirror symmetry of roots of unity (W^(k+N/2) = -W^k) allows computing two outputs per twiddle multiplication.

### Complexity Analysis
- **Direct DFT**: O(N²) - N complex multiplications for each of N outputs
- **FFT**: O(N log N) - Divide and conquer reduces operations exponentially
- **Speedup**: For N=1024: ~1,000,000 ops → ~10,000 ops ≈ 100x faster

---

## Makefile Targets

### Build Targets
- `make` or `make all` - Build all executables
- `make build` - Same as `make all`
- `make clean` - Remove build artifacts
- `make distclean` - Full cleanup including binaries
- `make debug` - Build with debug symbols

### Test Targets
- `make test` - Run all tests
- `make test-bitrev` - Test bit reversal with N=8,16
- `make test-fft2` - Test FFT implementation
- `make test-timing` - Run performance comparison tests

### Utility Targets
- `make generate-inputs` - Create test input files (requires Python)
- `make verify` - Verify correctness against NumPy (requires Python/NumPy)
- `make check` - Validate build outputs
- `make help` - Show all available targets

---

## Performance Results

### Timing Tests (N=1024)
Based on included `timing.txt`:

| Algorithm | Time (seconds) | Speedup vs DFT |
|-----------|----------------|----------------|
| Direct DFT | 0.027937 | 1.00x (baseline) |
| Recursive FFT | 0.000300 | 93.12x faster |
| Non-Recursive FFT | 0.000096 | 291.01x faster |

**Key Findings:**
- FFT provides ~93-290x speedup over direct DFT
- Non-recursive implementation is ~3x faster than recursive (cache locality benefits)
- Results confirm theoretical O(N log N) vs O(N²) complexity difference

---

## Implementation Details

### Classes

#### FFT2 (Non-Recursive FFT)
- **File:** `fft2/fft2.h`, `fft2/fft2.cpp`
- **Key Methods:**
  - `Execute()` - In-place FFT using bit reversal and butterfly operations
  - `Reverse()` - Bit-reverse an index
  - `countBits()` - Calculate log₂(N)
- **Complexity:** O(N log N) time, O(1) extra space (in-place)

#### FFT (Recursive FFT)
- **File:** `fft2/fft2.h`, `fft2/fft2.cpp`
- **Key Methods:**
  - `Execute()` - Divide-and-conquer FFT
- **Complexity:** O(N log N) time, O(N log N) space (recursion)

#### DFT (Direct DFT)
- **File:** `fft2/fft2.h`, `fft2/fft2.cpp`
- **Key Methods:**
  - `Execute()` - Brute force DFT computation
- **Complexity:** O(N²) time

#### Timing
- **File:** `fft2/fft2.h`, `fft2/fft2.cpp`
- **Purpose:** Performance measurement and comparison
- **Output:** Writes results to `timing.txt`

---

## Technical Notes

### Bit Reversal
The bit reversal permutation is critical for the non-recursive FFT:
- Maps index i to its bit-reversed value
- Example (N=8, 3 bits): 5 (101₂) → 5 (101₂), 6 (110₂) → 3 (011₂)
- Ensures butterfly operations connect correct pairs

### Twiddle Factors
Roots of unity W_N^k = e^(-2πik/N) represent rotations on the unit circle:
- W₈⁰ = 1 (0°)
- W₈¹ = e^(-iπ/4) (45°)
- W₈² = e^(-iπ/2) = -i (90°)
- etc.

### Mirror Symmetry
W_N^(k+N/2) = -W_N^k allows computing both:
- X[k] = E[k] + W^k · O[k]
- X[k+N/2] = E[k] - W^k · O[k]

with a single twiddle multiplication.

---

## Development Logs

Detailed session logs are available in the `claudeLog/` directory:
- `session_001_2025-10-13.md` - Project setup and bit reversal
- `session_002_2025-10-16.md` - FFT implementation and bug fixes
- `session_003_2025-10-17.md` - Timing integration and finalization

These logs document:
- Algorithm explanations and geometric intuition
- Bug fixes and debugging process
- Performance optimization decisions
- Testing and verification steps

---

## Compilation Details

### Compiler Flags
- `-Wall -Wextra` - Enable all warnings
- `-std=c++17` - Use C++17 standard
- `-O2` - Optimization level 2 (release build)
- `-I.` - Include current directory for headers

### Dependencies
- Standard C++ library
- `<complex>` - Complex number support
- `<chrono>` - High-resolution timing
- `<fstream>` - File I/O

---

## Verification

### Correctness Testing
The implementation can be verified against NumPy's FFT:

```bash
# Generate test inputs and expected outputs
make generate-tests

# Run verification
make verify
```

### Manual Testing
```bash
# Test with small input (N=8)
./bin/fft2 8 input/sample_input.txt > output_fft.txt

# Compare with expected output
diff output_fft.txt output/expected_8.txt
```

---

## Known Limitations

1. **Power of 2 Requirement:** N must be a power of 2 (Cooley-Tukey radix-2 algorithm)
2. **Input Format:** Requires specific complex number format (real imag per line)
3. **Precision:** Uses double precision (IEEE 754), small numerical errors possible
4. **Memory:** In-place algorithm modifies input array

---

## Future Enhancements

- Support for arbitrary N (Bluestein's algorithm or mixed-radix FFT)
- Inverse FFT implementation
- Multi-dimensional FFT (2D for images)
- GPU acceleration using CUDA/OpenCL
- SIMD optimization (AVX/SSE)

---

## References

### Algorithm
- Cooley, James W.; Tukey, John W. (1965). "An algorithm for the machine calculation of complex Fourier series"
- Numerical Recipes in C, Chapter 12: Fast Fourier Transform

### Documentation
- Project specification: `specDoc.txt`
- Bit reversal details: `BitRev.pdf`
- Project requirements: `proj3.pdf`

---

## Troubleshooting

### Build Errors
```bash
# Clean and rebuild
make clean
make

# Check compiler version
g++ --version  # Should be 7.0+ for C++17
```

### Runtime Errors
```bash
# Verify input file exists and has correct format
cat input/random_1024.txt

# Check N is power of 2
# Valid: 8, 16, 32, 64, 128, 256, 512, 1024
# Invalid: 10, 100, 1000
```

### Performance Issues
```bash
# Ensure optimizations enabled
# Check CXXFLAGS in Makefile includes -O2

# For timing tests, use N=1024 for meaningful results
make test-timing
```

---

## Contact

For questions or issues regarding this implementation, please refer to:
- Development logs in `claudeLog/`
- Project specification in `specDoc.txt`
- Code comments in source files

---

## License

This project is submitted as coursework for MAT320. All rights reserved.

---

**Last Updated:** October 17, 2025
**Build Status:** ✓ Passing
**Test Status:** ✓ All tests passing
**Performance:** ✓ Meeting O(N log N) complexity targets