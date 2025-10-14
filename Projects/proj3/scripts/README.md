# Python Verification Scripts for MAT320 Project 3

This directory contains Python scripts for generating test inputs and verifying the mathematical correctness of implementations.

## Scripts Overview

### 1. `generate_inputs.py`
Generates comprehensive test input files for FFT testing.

**Usage:**
```bash
# Generate all test input files
python3 generate_inputs.py

# Generate expected output files (requires numpy)
python3 generate_inputs.py outputs
```

**Or via Makefile:**
```bash
make generate-inputs    # Generate input files
make generate-outputs   # Generate expected outputs
make generate-tests     # Generate both
```

**Generated Test Cases:**
- **N=8**: 7 test cases (impulse, DC, Nyquist, sine, cosine, complex exponential, random)
- **N=16**: 7 test cases (impulse, DC, Nyquist, sine, cosine, mixed frequencies, random)
- **N=32**: 6 test cases (impulse, DC, sine, chirp, square wave, random)
- **N=1024**: 2 test cases for timing (random, mixed frequencies)
- **sample_input.txt**: Quick test file (N=8)

### 2. `verify_bitrev.py`
Verifies bit reversal algorithm correctness.

**Usage:**
```bash
python3 verify_bitrev.py
# Or: make verify-bitrev
```

**Tests Performed:**
- ✓ Bit reversal for N=8, 16, 32
- ✓ Specific test cases from spec document
- ✓ Mathematical properties:
  - Involution: `bitReverse(bitReverse(i)) = i`
  - Self-inverse elements (palindromes)
  - Special cases (zero, all-ones)

**Output Format:**
Generates tables matching the project specification:
```
i, binary(i), bitReverse(i), binary(bitReverse(i))
0, 000, 0, 000
1, 001, 4, 100
2, 010, 2, 010
...
```

### 3. `verify_fft.py`
Verifies FFT implementation using NumPy as reference.

**Usage:**
```bash
python3 verify_fft.py
# Or: make verify-fft

# Generate reference FFT outputs
python3 verify_fft.py generate
```

**Tests Performed:**
- ✓ Known analytical transforms:
  - Impulse → All ones
  - DC signal → Impulse in frequency domain
  - Nyquist frequency signal
- ✓ Mathematical properties:
  - Linearity: `FFT(a*x + b*y) = a*FFT(x) + b*FFT(y)`
  - Parseval's theorem: Energy conservation
  - Inverse FFT: `IFFT(FFT(x)) = x`

**Verification Functions:**
- `verify_fft_result()` - Compare against NumPy FFT
- `verify_parseval_theorem()` - Check energy conservation
- `verify_known_transforms()` - Test known analytical cases
- `verify_fft_properties()` - Test mathematical properties

## Requirements

**Python 3** with:
- `numpy` - For FFT reference implementation and verification

**Install dependencies:**
```bash
pip install numpy
```

## Following genStandards

These scripts follow the **VERIFICATION_CHECKLIST.md** guidelines:

1. **Slow down** - Scripts show step-by-step verification
2. **Show work** - All calculations are explicit
3. **Double-check** - Multiple verification methods
4. **Verify identities** - Tests mathematical properties
5. **Use systematic methods** - Reference implementations from NumPy

## Integration with Makefile

All scripts are integrated into the project Makefile:

```bash
# Generation
make generate-inputs      # Create test inputs
make generate-outputs     # Create expected outputs
make generate-tests       # Both inputs and outputs

# Verification
make verify-bitrev        # Verify bit reversal
make verify-fft           # Verify FFT (requires numpy)
make verify               # Run all verifications
```

## Output Directories

- **input/**: Test input files (complex numbers)
- **output/**: Expected output files (FFT results)

## Important Notes

1. **Always verify** generated outputs before using them as expected results
2. **NumPy FFT** serves as the reference implementation
3. **Floating point tolerance** is set to 1e-9 for comparisons
4. **Document calculations** - Keep records of verification in docs/

## Test Signal Types

### Basic Signals
- **Impulse**: `[1, 0, 0, ..., 0]` - Tests all-pass response
- **DC**: `[1, 1, 1, ..., 1]` - Tests zero frequency response
- **Nyquist**: `[1, -1, 1, -1, ...]` - Tests maximum frequency

### Periodic Signals
- **Sine wave**: Real sine at specific frequency
- **Cosine wave**: Real cosine at specific frequency
- **Complex exponential**: `e^(i*2π*k*n/N)`
- **Mixed frequencies**: Multiple frequency components

### Special Signals
- **Chirp**: Increasing frequency sweep
- **Square wave**: Step function
- **Random**: Random complex numbers (reproducible seed)

## Confidence Levels

After running verification:
- **High confidence**: All tests pass with error < 1e-10
- **Medium confidence**: Tests pass with error < 1e-6
- **Low confidence**: Some tests fail or errors exceed tolerance

Always document your confidence level in expected output files!