#include "bitrev.h"
#include "../timing.h"
#include <iostream>
#include <string>
#include <cstdlib>


// Input from binary string conversion
u_int bitReverse(u_int input, u_int numBits) 
{
    u_int reversed = 0;
    u_int last = numBits - 1;
    for(u_int bit = 0; bit < numBits; ++bit)
    {
        // checking for set bits
        // left shift -> place 1 in specified position by 'bit'
        // & mask -> if AND, is set. 
        if( input & (1 << bit))
        {
            // bitwise OR. Sets bit if confirmed set. 
            // Left shift, sets one opposite input bit.
            reversed |= (1 << (last - bit));
        }
    }
    return reversed;
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

u_int getNumBits(u_int N) 
{
    //std::cout<< "N: " << N << std::endl;
    u_int numBits = 0;
    //int div = 0;
    while(N > 1)
    {
        N >>= 1;
        //std::cout<< "N: " << N << std::endl;
        ++numBits;
    }
    //std::cout<< "NumBits: " << numBits << std::endl;
    return numBits;
    
}
// powers of 2 have exactly one bit set ie:
// 8 = 1000, 4 = 0100
// N-1 flips all but single bit (if power of 2)
// 8 & 7 = 1000 & 0111 -> AND is mask
bool isPowerOfTwo(u_int N) 
{
    return N > 0 && ((N & (N-1))  == 0);
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
        std::cerr << "  N must be a positive power of 2" << std::endl;
        return 1;
    }

    u_int N = std::atoi(argv[1]);

    if (!isPowerOfTwo(N)) {
        std::cerr << "Error: N must be a positive power of 2" << std::endl;
        return 1;
    }
    u_int numBits = getNumBits(N);
    for(u_int i = 0; i < N; ++i)
    {
        u_int reversed = bitReverse(i, numBits);
        u_vector binary = toBinary(i, numBits); 
        u_vector binaryRev = toBinary(reversed, numBits);  
        std::cout<< i << " ";
        for(u_int j = 0; j < binary.size(); ++j)
        {
            std::cout<<binary[j];
        }
        std::cout<< "  " <<reversed << " ";
        for(u_int f = 0; f < binaryRev.size(); ++f)
        {
            std::cout<<binaryRev[f];
        }
        std::cout<<std::endl;
    }
    
     
    // Format: i, binary(i), bitReverse(i), binary(bitReverse(i))
    

    return 0;
}