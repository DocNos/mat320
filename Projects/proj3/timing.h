#pragma once

#include <string>
using string = std::string;
#include <iostream>
#include <complex>
using complex = std::complex<float>;
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using c_vector = std::vector<complex>;
using u_vector = std::vector<unsigned int>;

void Read(string filename);

void PrintFormattedVector(c_vector input);

// each bit followed by its reverse in sequence ie:
/*
   [0] 0 00000    [1] 0 00000
   [2] 1 00001    [3] 16 10000
   [4] 2 00010    [5] 8 01000
*/
void printBits(u_vector bits);
