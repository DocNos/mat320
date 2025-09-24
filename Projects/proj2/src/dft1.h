//#include "include.h"
#include <complex>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using complex = std::complex<double>;
using string = std::string;
using CVector = std::vector<complex>;

class DFT{
public:
    DFT(string _filename, int _size)
    : filename_(_filename), size_(_size)
    , numbers_(CVector(size_))
    {}
    void Read();
    CVector Execute();
    void Print();


private:
    string filename_;
    int size_;
    CVector numbers_;
};