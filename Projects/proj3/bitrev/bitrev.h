#ifndef BITREV_H
#define BITREV_H

#include "../timing.h"


//#include <string>
//using string = std::string;
using u_int = unsigned int;
u_int bitReverse(u_int input, u_int numBits);

u_vector toBinary(u_int n, u_int numBits);

u_int getNumBits(u_int N);
bool isPowerOfTwo(u_int N);

#endif // BITREV_H