//#include "include.h"
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

class DFT{
public:
    DFT(string _filename, int _size)
    : filename_(_filename), size_(_size)
    , numbers_(CVector()), result_(CVector())
    {}
    void Read();
    void Execute();
    void Print();

    string filename_;
    int size_;
    CVector numbers_;
    CVector result_;
};