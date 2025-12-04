## MAT 320 Project 7: Spectrum Shaping
# Nixx Varboncoeur
# 12 - 04 - 25

## Overview

This project implements spectrum shaping using reson filters applied to digital buzz signals. The system generates harmonic-rich buzz signals and shapes their frequency spectrum using second-order IIR bandpass (reson) filters to create various timbres and formant-like sounds.

### Core Concept
- **Buzz Signal**: Periodic impulse train or sum of harmonics (like vocal cords)
- **Reson Filter**: Bandpass filter that emphasizes specific frequency regions (like vocal tract)
- **Result**: Spectrum-shaped audio demonstrating formant synthesis principles

## Project Structure

```
proj7_EC/
├── src/                    # Source files
│   ├── main.cpp           # Entry point with user input handling
│   ├── buzz.h             # Buzz generator class definition
│   ├── buzz.cpp           # Buzz signal generation (cosine sum/impulse)
│   ├── filter.h           # ResonFilter class definition
│   ├── filter.cpp         # Reson filter implementation
│   └── wav.h              # WAV file header and I/O functions
├── bin/                   # Compiled executables (generated)
├── obj/                   # Object files (generated)
├── output/                # Generated WAV files (generated)
├── docs/                  # Documentation
├── Makefile               # Build system
├── README.md              # This file
├── proj7.pdf              # Project specification
└── proj7_checklist.txt    # Implementation checklist
```

## Quick Start

```bash
# Build the project
make

# Run quick test
make test-quick

# Generate vowel formants
make test-vowels

# Run full test suite
make test-suite

# Clean up
make distclean
```

## Usage

### Basic Usage

```bash
./bin/spectrum_shaper <buzz_preset> <reson_preset>
```

### Example Commands

```bash
# Generate vowel "ah" sound with full harmonic buzz
./bin/spectrum_shaper full110 vowel_a

# Very narrow resonance at 440 Hz with limited harmonics
./bin/spectrum_shaper minDull narrow_440

# Wide bandwidth formant with low frequency buzz
./bin/spectrum_shaper full50 vowel_o
```

## Presets

### Buzz Presets (Source Signal)

| Preset | Frequency | Harmonics | Description |
|--------|-----------|-----------|-------------|
| `minDull` | 110 Hz | 5 | Very dull, almost sine-wave-like |
| `midDull` | 110 Hz | 10 | Still dull, limited harmonics |
| `maxDull` | 110 Hz | 50 | Darker, warmer buzz |
| `full110` | 110 Hz | 200 | Full, rich buzz (recommended) |
| `full220` | 220 Hz | 100 | Higher fundamental frequency |
| `full50` | 50 Hz | 441 | Very low bass rumble |

### Reson Presets (Filter Settings)

| Preset | Center Freq | Bandwidth | Description |
|--------|-------------|-----------|-------------|
| `vowel_a` | 730 Hz | 50 Hz | Formant for "ah" sound |
| `vowel_e` | 530 Hz | 50 Hz | Formant for "eh" sound |
| `vowel_i` | 270 Hz | 40 Hz | Formant for "ee" sound |
| `vowel_o` | 570 Hz | 50 Hz | Formant for "oh" sound |
| `vowel_u` | 440 Hz | 40 Hz | Formant for "oo" sound |
| `narrow_440` | 440 Hz | 20 Hz | Very narrow at A4 |
| `wide_1000` | 1000 Hz | 200 Hz | Wide band at 1kHz |

## Make Targets

### Build Targets
```bash
make              # Build the executable
make all          # Same as make
make release      # Optimized production build
make debug        # Debug build with symbols
make clean        # Remove build artifacts
make distclean    # Complete cleanup including WAV files
```

### Quick Test Targets
```bash
make test-quick             # Single test (full110 + vowel_a)
make test-vowels            # Test all vowel formants
```

### Comprehensive Test Targets
```bash
make test-suite             # Run complete test suite
make test-buzz-variants     # Test harmonic richness
make test-bandwidth         # Test bandwidth effects
make test-frequencies       # Test different buzz frequencies
```

### Playback & Utilities
```bash
make play-all      # Play all generated WAV files
make list-output   # List generated files
make help          # Display help information
make info          # Show build configuration
```

## Technical Details

### Reson Filter

**Transfer Function:**
```
H(z) = (1 - R²) / (1 - 2R*cos(θ)*z^(-1) + R²*z^(-2))
```

**Difference Equation:**
```
y[n] = gain * x[n] + a1 * y[n-1] - a2 * y[n-2]
```

**Parameter Calculations:**
- `θ = 2π * fc / fs` (fc = center frequency, fs = sample rate)
- `R = exp(-π * BW / fs)` (BW = bandwidth in Hz)
- `a1 = 2 * R * cos(θ)`
- `a2 = R²`
- `gain = 1 - R²`

### Buzz Signal Generation

**Cosine Sum Method:**
```
buzz[n] = (1/K) * Σ cos(2π * k * f_buzz * n / fs) for k=1 to K
```

**Impulse Train Method:**
```
buzz[n] = 1.0 if (n % period == 0) else 0.0
where period = round(sample_rate / f_buzz)
```

### WAV File Specifications
- **Sample Rate**: 44100 Hz (CD quality)
- **Channels**: 1 (mono)
- **Bit Depth**: 16-bit PCM
- **Format**: Standard WAV with proper header


## Testing

### Suggested Test Cases

1. **Vowel Synthesis** (`make test-vowels`)
   - Demonstrates formant synthesis with all vowel sounds
   - Uses full110 buzz with each vowel preset

2. **Harmonic Richness** (`make test-buzz-variants`)
   - Compares different levels of harmonic content
   - Shows effect of buzz richness on filtered output

3. **Bandwidth Effects** (`make test-bandwidth`)
   - Demonstrates narrow vs. wide resonances
   - Shows selectivity and ringing behavior

4. **Frequency Variations** (`make test-frequencies`)
   - Tests different buzz fundamental frequencies
   - Shows harmonic alignment effects

