#include "fft2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>


void PrintFormattedVector(c_vector input)
{
    for(auto num: input)
    {
        double real = num.real();
        double imag = num.imag();
        if(real == (int)real)
        {
            std::cout << std::fixed << std::setprecision(0)
            << (int)real;
        } else {
            std::cout << std::fixed << std::setprecision(3) << real;
        }
        if(imag >= 0)
        {
            //std::setprecision(0);
            std::cout << "+";
        } 
        if(imag == (int)imag)
        {
            std::cout << std::fixed << std::setprecision(0)
            <<  (int)imag;
        } else {
            std::cout << std::fixed << std::setprecision(3) << imag;
        }
        
        std::cout << "i" << std::endl;
    }
}


/* 
    "Bottom up"
    as opposed to recursive, which starts with full size, splitting
    and performing the dft. 
    Merge two half-sizes into one full size
bit-reversing -> bit-reverse the indexes so that correct
    indices are adjacent for the butterfly operation
butterfly operation -> merges two dft's into a larger one. 
    X[k] = E[k] + W_N^k * O[k]          (for k = 0 to N/2-1)
    X[k+N/2] = E[k] - W_N^k * O[k]      (for k = 0 to N/2-1)
    Where W_N^k = e^(-2πik/N) is the "twiddle factor" (Nth root of unity)
        e^(-2πik/N) -> vector that has rotated k*n steps

    W_N^(k+N/2) = W_N^k · W_N^(N/2) = W_N^k · (-1) = -W_N^k

DFT checks correlation between a signal and a wave that completes
k full rotations. 
    Checking if the signal contains a particular frequency. 

*/

void FFT2::Execute() 
{
    // have already verifed is power of two and expected input size
    // index swapping in-place with bit reversal
    for(u_int i = 0; i < size_; ++i)
    {
        u_int reversed = Reverse(i);
        // only swap if lower index (crossover)
        if(i < reversed) std::swap(numbers_[i], numbers_[reversed]);
    }
    // iterate through stages (log2(N) stages)
    for(u_int stage = 1; stage <= countBits(size_); ++stage)
    {
        // 2^stage
        u_int blockSize = 1 << stage; 
        // "base case" root of unit
        complex principle = 
            std::exp(complex(0, -2.0 * M_PI / blockSize));
        // iterate over current block
        for(u_int blockIndex = 0; 
            blockIndex < size_; 
            blockIndex += blockSize)
        {
            complex twiddle = 1.0;
            // Butterfly operation - combine dft of adjacent indices
            for(u_int i = 0; i < blockSize/2; ++i)
            {
                // Even index -> first in vector
                u_int evenIndex = blockIndex + i;
                u_int oddIndex = evenIndex + blockSize/2;

                // Butterfly - mirrored operations
                complex twiddleOdd = twiddle * numbers_[oddIndex];
                complex even = numbers_[evenIndex];
                numbers_[evenIndex] = even + twiddleOdd;
                numbers_[oddIndex] = even - twiddleOdd;

                twiddle *= principle;
            }
        }
    }

}

c_vector FFT::Execute(c_vector input)
{
    int N = static_cast<int>(input.size());
    // std::cout << "FFT called with N=" << N << std::endl;
    if(N == 1) {
        // std::cout << "Base case: returning " << input[0] << std::endl;
        return input;
    }
    
    c_vector even, odd;
    for(int i = 0; i < N; i+=2) {
        even.push_back(input[i]);
    }
    for(int i = 1; i < N; i+=2) {        
        odd.push_back(input[i]);
    }       
    c_vector evenRes = Execute(even);
    c_vector oddRes = Execute(odd);
            
    c_vector result(N);
    for(int k = 0; k < N/2; ++k) {       
        double angle = -2.0 * M_PI * k / N;
        complex twiddle(cos(angle), sin(angle));
        complex t = twiddle * oddRes[k];
        
        result[k] = evenRes[k] + t;
        result[k + N/2] = evenRes[k] - t;        
        
    }
    return result;
}

void FFT::Read()
{
    string line;
    std::ifstream file(filename_);
    if(!file.is_open()){
        std::cerr << "Error opening file " << filename_ << std::endl;
        return;
    }
    while(std::getline(file, line))
    {
        complex num;
        double real, imag;
        char plus, i;
        std::istringstream stream(line);
        if(stream >> real >> plus >> imag >> i){
            double imagFinal = (plus == '-') ? -imag : imag;
            num = complex(real, imagFinal);
            numbers_.emplace_back(num);
            // std::cout << "Read from " << filename << " "
            // << num << std::endl;
        }
    }

}

void FFT::Print()
{
    PrintFormattedVector(result_);
}

void DFT::Execute()
{
    for(int k = 0; k < (int)size_; ++k)
    {
        result_[k] = 0;
        for(int n = 0; n < (int)size_; ++n)
        {
            double angle = -2.0 * M_PI * k * n / size_;
            complex twiddle(cos(angle), sin(angle));
            result_[k] += vec1_[n] * twiddle;
        }
    }
}

void DFT::Print()
{
    for(const auto& c : result_)
    {
        std::cout << c.real() << " " << c.imag() << std::endl;
    }
}

void Timing::test_dft(u_int size, c_vector vector)
{
    timePoint start = NowTime();
    DFT dft(size, vector);    
    dft.Execute();
    timePoint end = NowTime();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dft_Speed = duration.count() / 1000000.0;
}

void Timing::test_fftRec(c_vector input)
{
    timePoint start = NowTime();
    FFT fourier("", input.size());
    fourier.Execute(input);
    timePoint end = NowTime();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    fftRecursive_Speed = duration.count() / 1000000.0;
}

void Timing::test_fftIP(c_vector input)
{
    timePoint start = NowTime();
    FFT2 fourier(input.size(), "");
    fourier.Execute();
    timePoint end = NowTime();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    fftInPlace_Speed = duration.count() / 1000000.0;
   
}

void Timing::TestPrint()
{

}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <input_file>" << std::endl;
        std::cerr << "  N: number of samples (must be power of 2)" << std::endl;
        std::cerr << "  input_file: file containing N complex numbers (format: real imag per line)" << std::endl;
        return 1;
    }
    u_int N = std::atoi(argv[1]);    
    FFT2 fourier(N, argv[2]);
    fourier.Read();    
    if(fourier.Verify()) return 1;

    fourier.Execute();
    for (const auto& c : fourier.numbers_) 
    {
        std::cout << c.real() << " " << c.imag() << std::endl;
    }

    return 0;
}

/*
void FFT::Read()
{
    string line;
    std::ifstream file(filename_);
    if(!file.is_open()){
        std::cerr << "Error opening file " << filename_ << std::endl;
        return;
    }
    while(std::getline(file, line))
    {
        complex num;
        double real, imag;
        char plus, i;
        std::istringstream stream(line);
        if(stream >> real >> plus >> imag >> i){
            double imagFinal = (plus == '-') ? -imag : imag;
            num = complex(real, imagFinal);
            numbers_.emplace_back(num);
            // std::cout << "Read from " << filename << " "
            // << num << std::endl;
        }
    }
}

c_vector FFT::Execute(c_vector input)
{
    int N = static_cast<int>(input.size());
    // std::cout << "FFT called with N=" << N << std::endl;
    if(N == 1) {
        // std::cout << "Base case: returning " << input[0] << std::endl;
        return input;
    }
    
    c_vector even, odd;
    for(int i = 0; i < N; i+=2) {
        even.push_back(input[i]);
    }
    for(int i = 1; i < N; i+=2) {        
        odd.push_back(input[i]);
    }
    
        
    c_vector evenRes = Execute(even);
    c_vector oddRes = Execute(odd);
            
    c_vector result(N);
    for(int k = 0; k < N/2; ++k) {       
        double angle = -2.0 * M_PI * k / N;
        complex twiddle(cos(angle), sin(angle));
        complex t = twiddle * oddRes[k];
        
        result[k] = evenRes[k] + t;
        result[k + N/2] = evenRes[k] - t;        
        
    }
    return result;
}


*/