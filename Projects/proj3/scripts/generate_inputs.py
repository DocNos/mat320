#!/usr/bin/env python3
"""
Test input generator for MAT320 Project 3 - FFT
Generated on: 2025-10-13
Purpose: Generate various test input files for FFT testing
"""

import numpy as np
import os
from typing import List


def write_complex_file(filename: str, data: List[complex], add_header: bool = False):
    """
    Write complex numbers to file in format: real imag (one per line).

    Args:
        filename: Path to output file
        data: List of complex numbers
        add_header: Whether to add a comment header
    """
    with open(filename, 'w') as f:
        if add_header:
            f.write(f"# N={len(data)} complex numbers (format: real imag)\n")
        for c in data:
            f.write(f"{c.real:.10f} {c.imag:.10f}\n")
    print(f"✓ Created {filename} with {len(data)} complex numbers")


def generate_impulse(N: int) -> List[complex]:
    """
    Generate impulse signal: [1, 0, 0, ..., 0]

    Args:
        N: Signal length

    Returns:
        List of complex numbers
    """
    return [1.0 + 0j] + [0.0 + 0j] * (N - 1)


def generate_dc(N: int) -> List[complex]:
    """
    Generate DC signal: [1, 1, 1, ..., 1]

    Args:
        N: Signal length

    Returns:
        List of complex numbers
    """
    return [1.0 + 0j] * N


def generate_nyquist(N: int) -> List[complex]:
    """
    Generate Nyquist frequency signal: [1, -1, 1, -1, ...]

    Args:
        N: Signal length (should be even)

    Returns:
        List of complex numbers
    """
    return [(1.0 if i % 2 == 0 else -1.0) + 0j for i in range(N)]


def generate_sine_wave(N: int, frequency: int = 1) -> List[complex]:
    """
    Generate real sine wave at given frequency.

    Args:
        N: Signal length
        frequency: Frequency bin (0 to N-1)

    Returns:
        List of complex numbers
    """
    return [np.sin(2 * np.pi * frequency * n / N) + 0j for n in range(N)]


def generate_cosine_wave(N: int, frequency: int = 1) -> List[complex]:
    """
    Generate real cosine wave at given frequency.

    Args:
        N: Signal length
        frequency: Frequency bin (0 to N-1)

    Returns:
        List of complex numbers
    """
    return [np.cos(2 * np.pi * frequency * n / N) + 0j for n in range(N)]


def generate_complex_exponential(N: int, frequency: int = 1) -> List[complex]:
    """
    Generate complex exponential: e^(i*2π*k*n/N)

    Args:
        N: Signal length
        frequency: Frequency bin k

    Returns:
        List of complex numbers
    """
    return [np.exp(2j * np.pi * frequency * n / N) for n in range(N)]


def generate_random_complex(N: int, magnitude: float = 1.0, seed: int = 42) -> List[complex]:
    """
    Generate random complex numbers.

    Args:
        N: Signal length
        magnitude: Maximum magnitude
        seed: Random seed for reproducibility

    Returns:
        List of complex numbers
    """
    np.random.seed(seed)
    real_parts = magnitude * (2 * np.random.random(N) - 1)
    imag_parts = magnitude * (2 * np.random.random(N) - 1)
    return [complex(r, i) for r, i in zip(real_parts, imag_parts)]


def generate_chirp(N: int) -> List[complex]:
    """
    Generate chirp signal (increasing frequency).

    Args:
        N: Signal length

    Returns:
        List of complex numbers
    """
    return [np.exp(2j * np.pi * n * n / (2 * N)) for n in range(N)]


def generate_square_wave(N: int, duty_cycle: float = 0.5) -> List[complex]:
    """
    Generate square wave.

    Args:
        N: Signal length
        duty_cycle: Fraction of period that is high (0 to 1)

    Returns:
        List of complex numbers
    """
    threshold = int(N * duty_cycle)
    return [(1.0 if i < threshold else 0.0) + 0j for i in range(N)]


def generate_mixed_frequencies(N: int) -> List[complex]:
    """
    Generate signal with multiple frequency components.

    Args:
        N: Signal length

    Returns:
        List of complex numbers
    """
    signal = np.zeros(N, dtype=complex)
    # Add multiple frequency components
    signal += 1.0 * np.array([np.cos(2 * np.pi * 1 * n / N) for n in range(N)])  # f=1
    signal += 0.5 * np.array([np.cos(2 * np.pi * 2 * n / N) for n in range(N)])  # f=2
    signal += 0.3 * np.array([np.cos(2 * np.pi * 3 * n / N) for n in range(N)])  # f=3
    return signal.tolist()


def generate_all_inputs(input_dir: str = "input"):
    """
    Generate all test input files.

    Args:
        input_dir: Directory to write input files
    """
    os.makedirs(input_dir, exist_ok=True)

    print("=" * 80)
    print("GENERATING TEST INPUT FILES")
    print("=" * 80)
    print()

    # Small size tests (N=8)
    print("Generating N=8 test cases...")
    write_complex_file(f"{input_dir}/impulse_8.txt", generate_impulse(8), add_header=True)
    write_complex_file(f"{input_dir}/dc_8.txt", generate_dc(8), add_header=True)
    write_complex_file(f"{input_dir}/nyquist_8.txt", generate_nyquist(8), add_header=True)
    write_complex_file(f"{input_dir}/sine_8.txt", generate_sine_wave(8, 1), add_header=True)
    write_complex_file(f"{input_dir}/cosine_8.txt", generate_cosine_wave(8, 1), add_header=True)
    write_complex_file(f"{input_dir}/complex_exp_8.txt", generate_complex_exponential(8, 1), add_header=True)
    write_complex_file(f"{input_dir}/random_8.txt", generate_random_complex(8), add_header=True)
    print()

    # Medium size tests (N=16)
    print("Generating N=16 test cases...")
    write_complex_file(f"{input_dir}/impulse_16.txt", generate_impulse(16), add_header=True)
    write_complex_file(f"{input_dir}/dc_16.txt", generate_dc(16), add_header=True)
    write_complex_file(f"{input_dir}/nyquist_16.txt", generate_nyquist(16), add_header=True)
    write_complex_file(f"{input_dir}/sine_16.txt", generate_sine_wave(16, 2), add_header=True)
    write_complex_file(f"{input_dir}/cosine_16.txt", generate_cosine_wave(16, 2), add_header=True)
    write_complex_file(f"{input_dir}/mixed_16.txt", generate_mixed_frequencies(16), add_header=True)
    write_complex_file(f"{input_dir}/random_16.txt", generate_random_complex(16, seed=43), add_header=True)
    print()

    # Larger size tests (N=32)
    print("Generating N=32 test cases...")
    write_complex_file(f"{input_dir}/impulse_32.txt", generate_impulse(32), add_header=True)
    write_complex_file(f"{input_dir}/dc_32.txt", generate_dc(32), add_header=True)
    write_complex_file(f"{input_dir}/sine_32.txt", generate_sine_wave(32, 4), add_header=True)
    write_complex_file(f"{input_dir}/chirp_32.txt", generate_chirp(32), add_header=True)
    write_complex_file(f"{input_dir}/square_32.txt", generate_square_wave(32), add_header=True)
    write_complex_file(f"{input_dir}/random_32.txt", generate_random_complex(32, seed=44), add_header=True)
    print()

    # Timing test size (N=1024)
    print("Generating N=1024 test case for timing...")
    write_complex_file(f"{input_dir}/random_1024.txt", generate_random_complex(1024, seed=100), add_header=True)
    write_complex_file(f"{input_dir}/mixed_1024.txt", generate_mixed_frequencies(1024), add_header=True)
    print()

    # Create a simple default input for quick testing
    print("Creating default sample input...")
    simple_input = [
        1.0 + 0.0j,
        0.707 + 0.707j,
        0.0 + 1.0j,
        -0.707 + 0.707j,
        -1.0 + 0.0j,
        -0.707 - 0.707j,
        0.0 - 1.0j,
        0.707 - 0.707j
    ]
    write_complex_file(f"{input_dir}/sample_input.txt", simple_input, add_header=True)
    print()

    print("=" * 80)
    print(f"✓ All test input files generated in {input_dir}/")
    print("=" * 80)
    print()
    print("Test files created:")
    print(f"  - N=8:    7 test cases")
    print(f"  - N=16:   7 test cases")
    print(f"  - N=32:   6 test cases")
    print(f"  - N=1024: 2 test cases (for timing)")
    print(f"  - sample_input.txt: Quick test (N=8)")
    print()


def generate_expected_outputs(input_dir: str = "input", output_dir: str = "output"):
    """
    Generate expected outputs for all test inputs using NumPy FFT.

    Args:
        input_dir: Directory containing input files
        output_dir: Directory to write expected output files
    """
    os.makedirs(output_dir, exist_ok=True)

    print("=" * 80)
    print("GENERATING EXPECTED OUTPUT FILES")
    print("Using NumPy FFT as reference implementation")
    print("=" * 80)
    print()

    # Get all .txt files from input directory
    input_files = sorted([f for f in os.listdir(input_dir) if f.endswith('.txt')])

    for input_file in input_files:
        input_path = os.path.join(input_dir, input_file)

        # Read input
        data = []
        with open(input_path, 'r') as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith('#'):
                    parts = line.split()
                    if len(parts) >= 2:
                        real = float(parts[0])
                        imag = float(parts[1])
                        data.append(complex(real, imag))

        if not data:
            continue

        # Compute FFT using NumPy
        fft_result = np.fft.fft(data)

        # Write output
        output_file = input_file.replace('.txt', '_expected.txt')
        output_path = os.path.join(output_dir, output_file)

        with open(output_path, 'w') as f:
            f.write(f"# FFT output for {input_file} (N={len(data)})\n")
            for c in fft_result:
                f.write(f"{c.real:.10f} {c.imag:.10f}\n")

        print(f"✓ {input_file:25s} → {output_file:30s} (N={len(data):4d})")

    print()
    print("=" * 80)
    print(f"✓ All expected output files generated in {output_dir}/")
    print("=" * 80)
    print()
    print("IMPORTANT: These outputs were generated using NumPy's FFT.")
    print("They serve as the reference implementation for verification.")
    print("Review the VERIFICATION_CHECKLIST.md for validation guidelines.")
    print()


if __name__ == "__main__":
    import sys

    if len(sys.argv) > 1 and sys.argv[1] == "outputs":
        # Generate expected outputs
        generate_expected_outputs()
    else:
        # Generate input files (default)
        generate_all_inputs()