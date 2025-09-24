#include <iostream>
#include "dft1.h"

// DFT MAIN
/*
dft1.cpp and fft1.cpp should run with the same
command line arguments and give identical output.

Here are some sample inputs and outputs:
(N=8, precision is set to 4)

*/
int main(int argc, char*argv[])
{
    if(argc < 3){
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }
    DFT fourier = DFT(argv[1], atoi(argv[2]));
    CVector result = fourier.Execute();
    fourier.Print();
    return 0;
}

