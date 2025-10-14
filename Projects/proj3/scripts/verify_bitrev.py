#!/usr/bin/env python3
"""
Verification script for MAT320 Project 3 - Bit Reversal
Generated on: 2025-10-13
Purpose: Verify bit reversal calculations are mathematically correct
"""

def bit_reverse(i, num_bits):
    """
    Reverse the bits of integer i using exactly num_bits bits.
    This is the reference implementation to verify against.

    Args:
        i: Integer to reverse
        num_bits: Number of bits to use

    Returns:
        Bit-reversed integer
    """
    reversed_val = 0
    for bit_pos in range(num_bits):
        # Check if bit at position bit_pos is set in i
        if i & (1 << bit_pos):
            # Set the corresponding reversed bit
            reversed_val |= (1 << (num_bits - 1 - bit_pos))
    return reversed_val


def to_binary(n, num_bits):
    """
    Convert integer to binary string with leading zeros.

    Args:
        n: Integer to convert
        num_bits: Number of bits in output string

    Returns:
        Binary string representation
    """
    return format(n, f'0{num_bits}b')


def verify_bit_reversal(N):
    """
    Verify bit reversal for all values from 0 to N-1.

    Args:
        N: Power of 2 (e.g., 8, 16, 32)

    Returns:
        True if verification passes, False otherwise
    """
    import math

    if N <= 0 or (N & (N - 1)) != 0:
        print(f"✗ N={N} is not a power of 2")
        return False

    num_bits = int(math.log2(N))
    print(f"\nVerifying bit reversal for N={N} (using {num_bits} bits)")
    print("=" * 60)

    all_passed = True

    for i in range(N):
        # Calculate bit reversal
        reversed_i = bit_reverse(i, num_bits)

        # Get binary representations
        binary_i = to_binary(i, num_bits)
        binary_reversed = to_binary(reversed_i, num_bits)

        # Verify that reversing twice gives original
        double_reversed = bit_reverse(reversed_i, num_bits)

        if double_reversed != i:
            print(f"✗ Failed for i={i}: double reversal gave {double_reversed}, expected {i}")
            all_passed = False

        # Print the entry (matches expected output format)
        print(f"{i}, {binary_i}, {reversed_i}, {binary_reversed}")

    print("=" * 60)
    if all_passed:
        print(f"✓ All {N} bit reversals verified successfully")
    else:
        print(f"✗ Some bit reversals failed verification")

    return all_passed


def verify_specific_cases():
    """
    Verify specific known cases manually.
    These are edge cases and examples from the spec.
    """
    print("\n" + "=" * 60)
    print("Verifying Specific Test Cases")
    print("=" * 60)

    test_cases = [
        # (i, num_bits, expected_reversed, description)
        (0, 3, 0, "Zero always reverses to zero"),
        (1, 3, 4, "001 reverses to 100 (from spec N=8 example)"),
        (2, 3, 2, "010 reverses to 010 (palindrome)"),
        (3, 3, 6, "011 reverses to 110 (from spec N=8 example)"),
        (4, 3, 1, "100 reverses to 001 (from spec N=8 example)"),
        (5, 3, 5, "101 reverses to 101 (palindrome)"),
        (6, 3, 3, "110 reverses to 011 (from spec N=8 example)"),
        (7, 3, 7, "111 reverses to 111 (all ones)"),
        # Additional N=16 cases
        (0, 4, 0, "N=16: Zero"),
        (1, 4, 8, "N=16: 0001 reverses to 1000"),
        (15, 4, 15, "N=16: All ones (1111)"),
    ]

    all_passed = True

    for i, num_bits, expected, description in test_cases:
        actual = bit_reverse(i, num_bits)
        binary_i = to_binary(i, num_bits)
        binary_actual = to_binary(actual, num_bits)

        if actual == expected:
            print(f"✓ {description}")
            print(f"  {i} ({binary_i}) → {actual} ({binary_actual})")
        else:
            print(f"✗ {description}")
            print(f"  Expected: {expected}, Got: {actual}")
            all_passed = False

    print("=" * 60)

    return all_passed


def verify_properties():
    """
    Verify mathematical properties of bit reversal.
    """
    print("\n" + "=" * 60)
    print("Verifying Mathematical Properties")
    print("=" * 60)

    all_passed = True

    # Property 1: Involution (reversing twice gives original)
    print("\n1. Testing Involution Property: bitReverse(bitReverse(i)) = i")
    for num_bits in [3, 4, 5]:
        N = 2 ** num_bits
        for i in range(N):
            double_reversed = bit_reverse(bit_reverse(i, num_bits), num_bits)
            if double_reversed != i:
                print(f"  ✗ Failed for i={i}, num_bits={num_bits}")
                all_passed = False
    print("  ✓ Involution property holds for all tested values")

    # Property 2: Self-inverse elements (palindromes)
    print("\n2. Finding Self-Inverse Elements (Binary Palindromes)")
    for num_bits in [3, 4, 5]:
        N = 2 ** num_bits
        self_inverse = []
        for i in range(N):
            if bit_reverse(i, num_bits) == i:
                self_inverse.append(i)
        print(f"  N={N} ({num_bits} bits): {len(self_inverse)} self-inverse elements: {self_inverse}")

    # Property 3: Zero and all-ones are always self-inverse
    print("\n3. Testing Special Cases")
    for num_bits in [3, 4, 5, 6]:
        N = 2 ** num_bits
        zero_reversed = bit_reverse(0, num_bits)
        all_ones_reversed = bit_reverse(N - 1, num_bits)

        if zero_reversed == 0 and all_ones_reversed == (N - 1):
            print(f"  ✓ N={N}: Zero and all-ones are self-inverse")
        else:
            print(f"  ✗ N={N}: Failed special case test")
            all_passed = False

    print("=" * 60)

    return all_passed


def verify_all_tests():
    """Run all verification tests"""
    print("=" * 60)
    print("BIT REVERSAL VERIFICATION SUITE")
    print("MAT320 Project 3")
    print("=" * 60)

    results = []

    # Test standard sizes
    results.append(("N=8", verify_bit_reversal(8)))
    results.append(("N=16", verify_bit_reversal(16)))
    results.append(("N=32", verify_bit_reversal(32)))

    # Test specific cases
    results.append(("Specific Cases", verify_specific_cases()))

    # Test mathematical properties
    results.append(("Properties", verify_properties()))

    # Summary
    print("\n" + "=" * 60)
    print("SUMMARY")
    print("=" * 60)

    passed = sum(1 for _, result in results if result)
    total = len(results)

    for test_name, result in results:
        status = "✓ PASS" if result else "✗ FAIL"
        print(f"{status}: {test_name}")

    print("=" * 60)
    print(f"Total: {passed}/{total} test suites passed")
    print("=" * 60)

    return all(result for _, result in results)


if __name__ == "__main__":
    import sys
    success = verify_all_tests()
    sys.exit(0 if success else 1)