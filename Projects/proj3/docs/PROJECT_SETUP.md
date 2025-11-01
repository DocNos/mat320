# MAT320 Project 3 - Project Setup Documentation

**Date:** 2025-10-13
**Status:** Initial setup complete

## Directory Structure

Following genStandards/NAMING_STANDARDS.md conventions:

```
proj3/
├── bin/           # Compiled executables (bitrev, fft2, timing_test)
├── obj/           # Object files (.o files)
├── input/         # Test input files (complex number data)
├── output/        # Expected output files for validation
├── tests/         # Test suites and validation scripts
├── docs/          # Documentation and calculation work
├── scripts/       # Python verification scripts
├── bitrev.cpp     # Bit reversal implementation
├── bitrev.h       # Bit reversal header
├── fft2.cpp       # Non-recursive FFT implementation
├── fft2.h         # FFT header
├── timing_test.cpp # Timing comparison framework
├── Makefile       # Build system (standards-compliant)
├── specDoc.txt    # Project specification
├── proj3.pdf      # Official project PDF
└── BitRev.pdf     # Bit reversal reference
```

## Makefile Targets

### Build Commands
- `make` or `make all` - Build all executables
- `make debug` - Build with debug symbols
- `make clean` - Remove object files and binaries
- `make distclean` - Full cleanup
- `make mrproper` - Reset to pristine state

### Testing Commands
- `make test` - Run all tests
- `make test-bitrev` - Test bit reversal (N=8,16)
- `make test-fft2` - Test FFT implementation
- `make test-timing` - Run timing comparisons

### Generation Commands
- `make generate-inputs` - Create test input files
- `make generate-outputs` - Generate expected outputs (MUST VERIFY!)
- `make generate-tests` - Generate both

### Verification Commands
- `make verify` - Run Python verification script
- `make check` - Validate executables exist

## Project Requirements

### Part 1: Bit Reversal (bitrev.cpp)
- Input: Command line argument N (power of 2)
- Output: Table of i, binary(i), bitReverse(i), binary(bitReverse(i))
- Required functions:
  - `unsigned int bitReverse(unsigned int i, unsigned int numBits)`
  - `string toBinary(unsigned int n, unsigned int numBits)`
  - `unsigned int getNumBits(unsigned int N)`

### Part 2: Non-Recursive FFT (fft2.cpp)
- Input: N (power of 2), input file with N complex numbers
- Output: N complex numbers (FFT result)
- Required functions:
  - `struct Complex` with operators +, -, *
  - `vector<Complex> readComplexFile(const string& filename, int N)`
  - `unsigned int bitReverse(unsigned int i, unsigned int numBits)`
  - `vector<Complex> fftNonRecursive(vector<Complex>& data)`

### Part 3: Timing Comparison (timing_test.cpp)
- Compare: DFT Direct, Recursive FFT, Non-Recursive FFT
- Test size: N=1024
- Output: timing.txt with performance comparison

## Standards Compliance

### Following genStandards/NAMING_STANDARDS.md:
✓ Standard directory structure (SRCDIR, BINDIR, OBJDIR, etc.)
✓ Universal target naming (all, clean, test, generate-*)
✓ Phony target declarations
✓ Comprehensive help target
✓ Safe directory creation patterns

### Following genStandards/VERIFICATION_CHECKLIST.md:
- [ ] Manual calculation verification (to be documented)
- [ ] Python verification script (to be created in scripts/)
- [ ] Step-by-step work shown (to be documented in docs/)
- [ ] Cross-verification with numpy.fft

## Next Steps

1. **Implement bitrev.cpp**
   - Implement bitReverse function using bit manipulation
   - Implement toBinary for binary string conversion
   - Implement getNumBits to calculate log2(N)
   - Create main() to parse args and generate output

2. **Implement fft2.cpp**
   - Create Complex struct with operators
   - Implement readComplexFile
   - Implement fftNonRecursive with Cooley-Tukey algorithm
   - Test with known inputs

3. **Create timing_test.cpp**
   - Implement or import DFT direct method
   - Implement or import recursive FFT (from proj2)
   - Create timing framework using chrono
   - Generate timing.txt report

4. **Verification**
   - Create Python verification script in scripts/
   - Document manual calculations in docs/
   - Verify bit reversal outputs manually
   - Cross-check FFT outputs with numpy.fft

## Important Reminders

⚠️ **ALWAYS verify expected outputs** using the verification checklist before committing them!

⚠️ **Show your work** - Document step-by-step calculations for all expected outputs

⚠️ **Use proper headers** - Only standard C++ headers, no external dependencies

⚠️ **Compile requirement** - Must compile with `g++` standard headers only