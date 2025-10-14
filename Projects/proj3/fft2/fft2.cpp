#include "fft2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

c_vector Read(string filename, u_int num)
{
    string line;
    std::ifstream file(filename);
    c_vector numbers;

    if(!file.is_open())
    {
        std::cerr << "Error opening file " << filename << std::endl;
        return numbers;  
    }

    u_int count = 0;
    while(count < num && std::getline(file, line))
    {
        // Skip comment lines (start with '#') and empty lines
        if (line.empty()) continue;
        
        double real, imag;
        std::istringstream stream(line);

        if(stream >> real >> imag) 
        {
            complex num(real, imag);
            numbers.emplace_back(num);
            count++;
        }
    }

    file.close();
    return numbers;
}

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

c_vector fftNonRecursive(c_vector& data) 
{
    u_int N = data.size();
    u_int numBits = getNumBits(N);    
    for(u_int i = 0; i < N; ++i)
    {
        u_int reversed = bitReverse(i, numBits);
        if(i < reversed) std::swap(data[i], data[reversed]);
    }    

    for (u_int s = 1; s <= numBits; s++) 
    { 
        unsigned int m = 1 << s;
        complex w_m = std::exp(complex(0, -2.0 * M_PI / m));
    
        for (unsigned int k = 0; k < N; k += m) 
        {
            complex w = 1.0;

            for (u_int j = 0; j < m/2; j++) 
            { 
                u_int upper = k + j;
                u_int lower = k + j + m/2;

                complex t = w * data[lower];
                complex u = data[upper];
                data[upper] = u + t;  // Upper branch: u + w*v
                data[lower] = u - t;  // Lower branch: u - w*v
                // Update twiddle factor: w = w * w_m
                w = w * w_m;
            }
        }
    }
    return data;
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <input_file>" << std::endl;
        std::cerr << "  N: number of samples (must be power of 2)" << std::endl;
        std::cerr << "  input_file: file containing N complex numbers (format: real imag per line)" << std::endl;
        return 1;
    }


    int N = std::atoi(argv[1]);
    if (!isPowerOfTwo(N)) 
    {
        std::cerr << "Error: N must be a positive power of 2" << std::endl;
        return 1;
    }
    std::string filename = argv[2];
    c_vector data = Read(filename, N);

    if (data.size() != static_cast<size_t>(N)) 
    {
        std::cerr << "Error: Expected " << N << " complex numbers, got " << data.size() << std::endl;
        return 1;
    }
    c_vector result = fftNonRecursive(data);
    for (const auto& c : result) 
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