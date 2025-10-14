
#include "bitrev.h"
#include <iostream>
#include <string>
#include <cstdlib>

/**
 * Reverse the bits of integer i using exactly numBits bits.
 *
 * Algorithm from spec:
 * - Start with reversed = 0 and MASK = N >> 1
 * - For each bit position (from left to right):
 *   - Check if current bit in i is set: i & (1 << bit_position)
 *   - If set, set corresponding reversed bit: reversed |= MASK
 *   - Shift MASK right: MASK >>= 1
 */
u_int bitReverse(u_int i, u_int numBits) 
{
    unsigned int reversed = 0;
    // TODO: Implement bit reversal algorithm
    // Hint: Use MASK pattern starting at N>>1, shift right each iteration
    // For each bit in i, set corresponding reversed bit
    return reversed;
}

/**
 * Convert integer to binary string representation.
 *
 * Must produce exactly numBits digits with leading zeros.
 */
string toBinary(u_int n, u_int numBits) {
    string binary = "";
    // TODO: Implement binary string conversion
    // Hint: Build string from right to left, checking each bit with (n & (1 << bit))
    // Add leading zeros if needed to reach numBits length
    return binary;
}

/**
 * Calculate the number of bits needed (log2(N)).
 *
 * For N = 8, returns 3 (since 2^3 = 8)
 * For N = 16, returns 4 (since 2^4 = 16)
 */
u_int getNumBits(u_int N) {
    u_int numBits = 0;
    // TODO: Implement log2 calculation
    // Hint: Keep dividing N by 2 until it reaches 1, counting divisions
    // Or use bit shifting: count how many times you can right-shift until 0
    return numBits;
}

/**
 * Check if N is a power of 2.
 *
 * A number is a power of 2 if it has exactly one bit set.
 * Bitwise trick: N & (N-1) == 0 for powers of 2
 */
bool isPowerOfTwo(unsigned int N) {
    // TODO: Implement power of 2 check
    // Hint: Use the trick (N > 0) && ((N & (N - 1)) == 0)
    return false;
}

/**
 * Main function: Parse command line and generate bit reversal table.
 *
 * Expected output format (example for N=8):
 * 0, 000, 0, 000
 * 1, 001, 4, 100
 * 2, 010, 2, 010
 * ...
 */
int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
        std::cerr << "  N must be a positive power of 2" << std::endl;
        return 1;
    }

    // Parse N from command line
    unsigned int N = std::atoi(argv[1]);

    // Validate N is a power of 2
    if (N == 0 || !isPowerOfTwo(N)) {
        std::cerr << "Error: N must be a positive power of 2" << std::endl;
        return 1;
    }

    // Calculate number of bits needed
    unsigned int numBits = getNumBits(N);

    // Generate and print bit reversal table
    // Format: i, binary(i), bitReverse(i), binary(bitReverse(i))
    for (unsigned int i = 0; i < N; i++) {
        // TODO: Implement output generation
        // 1. Get bit-reversed value: ir = bitReverse(i, numBits)
        // 2. Get binary representations: b1 = toBinary(i, numBits), b2 = toBinary(ir, numBits)
        // 3. Print in format: i, b1, ir, b2

        // Example output line: 1, 001, 4, 100
        std::cout << "TODO: implement output" << std::endl;
    }

    return 0;
}