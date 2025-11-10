# Project 4: Low Pass Filter

**Course:** MAT320 - Digital Signal Processing
**Date:** Fall 2025

Digital lowpass filter implementation for 16-bit mono WAV files using the equation `y[t] = x[t] + a1*x[t-1]` with iterative application and -1.5 dB normalization.

## Quick Start

```bash
# Build (creates directories automatically)
make all

# Run with subtle filtering
./bin/lowpass 0.02 10 input.wav

# Run with strong filtering
./bin/lowpass 0.99 100 input.wav

# Run test
make test
```

## Usage

```bash
./bin/lowpass <a1> <n> <input.wav>
```

- `<a1>`: Filter coefficient (0 < |a1| < 1)
  - **Larger values** (0.9-0.99) = stronger lowpass effect
  - **Smaller values** (0.01-0.1) = weaker lowpass effect
- `<n>`: Number of iterations (positive integer)
- `<input.wav>`: Input WAV file (16-bit mono)
- **Output:** `output.wav` (same directory)

## Build Commands

```bash
make all          # Build program (creates bin/ and obj/ directories)
make clean        # Remove build artifacts
make distclean    # Complete cleanup
make debug        # Debug build with symbols
make release      # Optimized build
make test         # Build and run test
```

## How It Works

1. **Filter equation:** `y[t] = x[t] + a1*x[t-1]` applied n times iteratively
2. **Lowpass effect:** Higher iterations = stronger effect
3. **Normalization:** Output scaled to -1.5 dB of maximum 16-bit value
4. **WAV I/O:** Preserves sample rate and format from input

## Examples

```bash
# Weak lowpass (subtle effect)
./bin/lowpass 0.02 10 audio.wav

# Moderate lowpass (noticeable effect)
./bin/lowpass 0.5 50 audio.wav

# Strong lowpass (aggressive filtering, like Figure 3.1 in textbook)
./bin/lowpass 0.99 100 audio.wav
```

## Project Structure

```
proj4/
├── src/           # Source files (lowpass.cpp, lowpass.h)
├── bin/           # Compiled executable (created by make)
├── obj/           # Object files (created by make)
├── docs/          # Documentation
├── Makefile       # Build system
└── README.md      # This file
```

## Requirements

- C++ compiler with C++17 support (g++)
- Make
- Input: 16-bit mono WAV files

## Troubleshooting

**"Insufficient arguments"** - Provide all 3 arguments
**"Coefficient should be less than 1"** - Use |a1| < 1
**"Cannot execute binary"** - Recompile on target platform (WSL vs Windows)
**No output file** - Check input file format (must be 16-bit mono WAV)

## References

- Project spec: `docs/specs.txt` and `proj4.pdf`
- Textbook: Chapter 4, Section 3 (Figure 3.1, page 67)