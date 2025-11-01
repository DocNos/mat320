#!/usr/bin/env python3
"""
Verification script for MAT320 Project 3 - FFT Implementation
Generated on: 2025-10-13
Purpose: Verify FFT outputs using NumPy's reference implementation
"""

import numpy as np
import sys
from typing import List, Tuple


def read_complex_file(filename: str) -> List[complex]:
    """
    Read complex numbers from file (format: real imag per line).

    Args:
        filename: Path to input file

    Returns:
        List of complex numbers
    """
    complexes = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                if line and not line.startswith('#'):
                    parts = line.split()
                    if len(parts) >= 2:
                        real = float(parts[0])
                        imag = float(parts[1])
                        complexes.append(complex(real, imag))
    except FileNotFoundError:
        print(f"✗ File not found: {filename}")
        return []

    return complexes


def write_complex_file(filename: str, data: List[complex]):
    """
    Write complex numbers to file (format: real imag per line).

    Args:
        filename: Path to output file
        data: List of complex numbers
    """
    with open(filename, 'w') as f:
        for c in data:
            f.write(f"{c.real} {c.imag}\n")


def verify_fft_result(input_data: List[complex], expected_output: List[complex],
                      tolerance: float = 1e-9) -> bool:
    """
    Verify FFT output against NumPy's reference implementation.

    Args:
        input_data: Input complex numbers
        expected_output: Expected FFT output
        tolerance: Floating point comparison tolerance

    Returns:
        True if verification passes
    """
    if len(input_data) != len(expected_output):
        print(f"✗ Length mismatch: input has {len(input_data)}, output has {len(expected_output)}")
        return False

    # Compute reference FFT using NumPy
    reference_fft = np.fft.fft(input_data)

    print(f"\nVerifying FFT for N={len(input_data)}")
    print("=" * 80)

    all_passed = True
    max_error = 0.0

    for i, (expected, reference) in enumerate(zip(expected_output, reference_fft)):
        error = abs(expected - reference)
        max_error = max(max_error, error)

        if error > tolerance:
            print(f"✗ Index {i}: Expected {expected:.10f}, Reference {reference:.10f}, Error {error:.2e}")
            all_passed = False
        else:
            print(f"✓ Index {i}: {expected:.10f} (error: {error:.2e})")

    print("=" * 80)
    print(f"Maximum error: {max_error:.2e}")

    if all_passed:
        print(f"✓ All {len(input_data)} FFT outputs verified successfully")
    else:
        print(f"✗ Some FFT outputs failed verification")

    return all_passed


def verify_parseval_theorem(input_data: List[complex], output_data: List[complex],
                            tolerance: float = 1e-9) -> bool:
    """
    Verify Parseval's theorem: energy is conserved in FFT.
    Sum of |x[n]|^2 should equal (1/N) * Sum of |X[k]|^2

    Args:
        input_data: Time domain data
        output_data: Frequency domain data (FFT output)
        tolerance: Tolerance for comparison

    Returns:
        True if theorem holds
    """
    N = len(input_data)

    # Time domain energy
    time_energy = sum(abs(x)**2 for x in input_data)

    # Frequency domain energy (divided by N)
    freq_energy = sum(abs(X)**2 for X in output_data) / N

    error = abs(time_energy - freq_energy)

    print(f"\nParseval's Theorem Verification:")
    print(f"  Time domain energy:      {time_energy:.10f}")
    print(f"  Frequency domain energy: {freq_energy:.10f}")
    print(f"  Error:                   {error:.2e}")

    if error < tolerance:
        print(f"✓ Parseval's theorem holds (error < {tolerance})")
        return True
    else:
        print(f"✗ Parseval's theorem violated (error >= {tolerance})")
        return False


def generate_test_signals() -> dict:
    """
    Generate standard test signals for FFT verification.

    Returns:
        Dictionary of test signals
    """
    signals = {}

    # Test 1: Impulse (delta function)
    signals['impulse_8'] = [1.0 + 0j] + [0.0 + 0j] * 7

    # Test 2: All ones (DC signal)
    signals['dc_8'] = [1.0 + 0j] * 8

    # Test 3: Alternating +1, -1 (Nyquist frequency)
    signals['nyquist_8'] = [(1.0 if i % 2 == 0 else -1.0) + 0j for i in range(8)]

    # Test 4: Complex exponential (single frequency)
    # e^(i*2π*k*n/N) for k=1, N=8
    signals['complex_exp_8'] = [np.exp(2j * np.pi * 1 * n / 8) for n in range(8)]

    # Test 5: Real sine wave
    signals['sine_8'] = [np.sin(2 * np.pi * 1 * n / 8) + 0j for n in range(8)]

    # Test 6: Real cosine wave
    signals['cosine_8'] = [np.cos(2 * np.pi * 1 * n / 8) + 0j for n in range(8)]

    # Larger size tests
    signals['impulse_16'] = [1.0 + 0j] + [0.0 + 0j] * 15
    signals['dc_16'] = [1.0 + 0j] * 16

    return signals


def verify_known_transforms():
    """
    Verify FFT outputs for signals with known analytical transforms.
    """
    print("\n" + "=" * 80)
    print("Verifying Known Analytical Transforms")
    print("=" * 80)

    all_passed = True

    # Test 1: Impulse → All ones
    print("\nTest 1: Impulse signal")
    print("  Input: [1, 0, 0, 0, 0, 0, 0, 0]")
    print("  Expected FFT: [1, 1, 1, 1, 1, 1, 1, 1]")
    impulse = [1.0 + 0j] + [0.0 + 0j] * 7
    fft_impulse = np.fft.fft(impulse)
    expected = np.ones(8, dtype=complex)
    if np.allclose(fft_impulse, expected, atol=1e-10):
        print("  ✓ Impulse FFT correct")
    else:
        print("  ✗ Impulse FFT incorrect")
        all_passed = False

    # Test 2: DC signal → Impulse in frequency domain
    print("\nTest 2: DC signal (all ones)")
    print("  Input: [1, 1, 1, 1, 1, 1, 1, 1]")
    print("  Expected FFT: [8, 0, 0, 0, 0, 0, 0, 0]")
    dc = [1.0 + 0j] * 8
    fft_dc = np.fft.fft(dc)
    expected_dc = [8.0 + 0j] + [0.0 + 0j] * 7
    if np.allclose(fft_dc, expected_dc, atol=1e-10):
        print("  ✓ DC signal FFT correct")
    else:
        print("  ✗ DC signal FFT incorrect")
        print(f"  Got: {fft_dc}")
        all_passed = False

    # Test 3: Nyquist frequency
    print("\nTest 3: Alternating ±1 (Nyquist frequency)")
    print("  Input: [1, -1, 1, -1, 1, -1, 1, -1]")
    print("  Expected: Energy concentrated at N/2 frequency bin")
    nyquist = [(1.0 if i % 2 == 0 else -1.0) + 0j for i in range(8)]
    fft_nyquist = np.fft.fft(nyquist)
    # Should have energy at index 4 (N/2)
    if abs(fft_nyquist[4]) > 7.9 and all(abs(fft_nyquist[i]) < 0.1 for i in [0, 1, 2, 3, 5, 6, 7]):
        print("  ✓ Nyquist frequency FFT correct")
    else:
        print("  ✗ Nyquist frequency FFT incorrect")
        print(f"  Got: {[abs(x) for x in fft_nyquist]}")
        all_passed = False

    return all_passed


def verify_fft_properties():
    """
    Verify mathematical properties of FFT.
    """
    print("\n" + "=" * 80)
    print("Verifying FFT Mathematical Properties")
    print("=" * 80)

    all_passed = True

    # Property 1: Linearity
    print("\nProperty 1: Linearity - FFT(a*x + b*y) = a*FFT(x) + b*FFT(y)")
    x = np.random.random(8) + 1j * np.random.random(8)
    y = np.random.random(8) + 1j * np.random.random(8)
    a, b = 2.5, 3.7

    lhs = np.fft.fft(a * x + b * y)
    rhs = a * np.fft.fft(x) + b * np.fft.fft(y)

    if np.allclose(lhs, rhs, atol=1e-10):
        print("  ✓ Linearity property verified")
    else:
        print("  ✗ Linearity property failed")
        all_passed = False

    # Property 2: Parseval's theorem
    print("\nProperty 2: Parseval's Theorem - Energy conservation")
    data = np.random.random(16) + 1j * np.random.random(16)
    fft_data = np.fft.fft(data)

    time_energy = np.sum(np.abs(data)**2)
    freq_energy = np.sum(np.abs(fft_data)**2) / len(data)

    if np.allclose(time_energy, freq_energy, atol=1e-10):
        print(f"  ✓ Parseval's theorem holds")
        print(f"    Time energy: {time_energy:.10f}")
        print(f"    Freq energy: {freq_energy:.10f}")
    else:
        print(f"  ✗ Parseval's theorem failed")
        all_passed = False

    # Property 3: Inverse FFT
    print("\nProperty 3: Inverse FFT - IFFT(FFT(x)) = x")
    original = np.random.random(32) + 1j * np.random.random(32)
    recovered = np.fft.ifft(np.fft.fft(original))

    if np.allclose(original, recovered, atol=1e-10):
        print("  ✓ Inverse FFT property verified")
    else:
        print("  ✗ Inverse FFT property failed")
        all_passed = False

    return all_passed


def create_reference_outputs(output_dir: str = "output"):
    """
    Create reference output files for standard test cases.

    Args:
        output_dir: Directory to write output files
    """
    print("\n" + "=" * 80)
    print("Generating Reference Output Files")
    print("=" * 80)

    signals = generate_test_signals()

    for name, signal in signals.items():
        # Compute FFT using NumPy
        fft_result = np.fft.fft(signal)

        # Write to file
        output_file = f"{output_dir}/fft_{name}_expected.txt"
        write_complex_file(output_file, fft_result.tolist())

        print(f"✓ Created {output_file}")
        print(f"  Input size: {len(signal)}")
        print(f"  Output size: {len(fft_result)}")


def verify_all_tests():
    """Run all verification tests"""
    print("=" * 80)
    print("FFT VERIFICATION SUITE")
    print("MAT320 Project 3")
    print("=" * 80)

    results = []

    # Test known transforms
    results.append(("Known Transforms", verify_known_transforms()))

    # Test mathematical properties
    results.append(("Properties", verify_fft_properties()))

    # Summary
    print("\n" + "=" * 80)
    print("SUMMARY")
    print("=" * 80)

    passed = sum(1 for _, result in results if result)
    total = len(results)

    for test_name, result in results:
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"{status}: {test_name}")

    print("=" * 80)
    print(f"Total: {passed}/{total} test suites passed")
    print("=" * 80)

    return all(result for _, result in results)


if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "generate":
        # Generate reference outputs
        import os
        output_dir = "output"
        os.makedirs(output_dir, exist_ok=True)
        create_reference_outputs(output_dir)
    else:
        # Run verification tests
        success = verify_all_tests()
        sys.exit(0 if success else 1)