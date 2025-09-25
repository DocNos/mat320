#include "fft1.h"

int main(int argc, char *argv[])
{
    if(argc < 3){
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }
    FFT fourier(argv[1], atoi(argv[2]));
    fourier.Read();
    //PrintFormattedVector(fourier.numbers_);
    fourier.result_ = fourier.Execute(fourier.numbers_);
    fourier.Print();

    return 0;
}