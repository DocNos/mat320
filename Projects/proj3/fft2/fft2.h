#ifndef FFT2_H
#define FFT2_H


#include "bitrev_lib.h"

#include <cmath>
#include <string>
using string = std::string;
#include <iostream>
#include <complex>
using complex = std::complex<double>;
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using c_vector = std::vector<complex>;
using u_vector = std::vector<unsigned int>;




c_vector Read(string filename, u_int num);
//u_int bitReverse(u_int input, u_int numBits);
//u_int getNumBits(u_int N);

c_vector fftNonRecursive(c_vector& data);

class FFT
{
public:
    FFT(string _filename, int _size)
    : filename_(_filename), size_(_size)
    , numbers_(c_vector()), result_(c_vector())
    {}
    void Read();
    //   X[k] = Σ(even indices) + Σ(odd indices)
    //   = E[k] + W_N^k * O[k]
    // W_N^k = e^(-2πik/N) = cos(-2πk/N) + i·sin(-2πk/N)
    // X[k] = E[k] + W_N^k * O[k]
    // X[k + N/2] = E[k] - W_N^k * O[k]
        // W_N^(k + N/2) = W_N^k * W_N^(N/2) = W_N^k * (-1) = -W_N^k
    c_vector Execute(c_vector input);
    void Print();



    string filename_;
    int size_;
    c_vector numbers_;
    c_vector result_;
};


#endif // FFT2_H