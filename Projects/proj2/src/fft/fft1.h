#include <complex>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>

using complex = std::complex<double>;
using string = std::string;
using CVector = std::vector<complex>;

void PrintFormattedVector(CVector input);

class FFT
{
public:
    FFT(string _filename, int _size)
    : filename_(_filename), size_(_size)
    , numbers_(CVector()), result_(CVector())
    {}
    void Read();
    //   X[k] = Σ(even indices) + Σ(odd indices)
    //   = E[k] + W_N^k * O[k]
    // W_N^k = e^(-2πik/N) = cos(-2πk/N) + i·sin(-2πk/N)
    // X[k] = E[k] + W_N^k * O[k]
    // X[k + N/2] = E[k] - W_N^k * O[k]
        // W_N^(k + N/2) = W_N^k * W_N^(N/2) = W_N^k * (-1) = -W_N^k
    CVector Execute(CVector input);
    void Print();



    string filename_;
    int size_;
    CVector numbers_;
    CVector result_;
};