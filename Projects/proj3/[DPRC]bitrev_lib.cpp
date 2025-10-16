
#include "bitrev_lib.h"

u_int bitReverse(u_int input, u_int numBits)
{
    u_int reversed = 0;
    u_int last = numBits - 1;
    for(u_int bit = 0; bit < numBits; ++bit)
    {
        if( input & (1 << bit))
        {
            reversed |= (1 << (last - bit));
        }
    }
    return reversed;
}

u_int getNumBits(u_int N)
{
    u_int numBits = 0;
    while(N > 1)
    {
        N >>= 1;
        ++numBits;
    }
    return numBits;
}

bool isPowerOfTwo(u_int N)
{
    return N > 0 && ((N & (N-1))  == 0);
}

u_vector toBinary(u_int n, u_int numBits)
{
    u_vector binary = u_vector(numBits, 0);
    u_int last = binary.size() - 1;
    for(u_int i = 0; i < numBits; ++i)
    {
        binary[last - i] = (n & (1 << i) ) ? (1) : (0);
    }
    return binary;
}