#include "fft1.h"


CVector FFT::Execute(CVector input)
{
    int N = static_cast<int>(input.size());
    // std::cout << "FFT called with N=" << N << std::endl;
    if(N == 1) {
        // std::cout << "Base case: returning " << input[0] << std::endl;
        return input;
    }
    
    CVector even, odd;
    for(int i = 0; i < N; i+=2) {
        even.push_back(input[i]);
    }
    for(int i = 1; i < N; i+=2) {        
        odd.push_back(input[i]);
    }
    
        
    CVector evenRes = Execute(even);
    CVector oddRes = Execute(odd);
            
    CVector result(N);
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

void PrintFormattedVector(CVector input)
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