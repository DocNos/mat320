#include "dft1.h"

void DFT::Read()
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
// Converts sequence of complex numbers from time domain to 
//      the frequency domain. 
// The DFT reveals the spectral content of a signal - it tells you which
//   frequencies are present and their relative strengths.
void DFT::Execute()
{
    // frequency bin index
    for(int k = 0; k < size_; ++k)
    {
        complex sum(0, 0);
        // time sample index, dot product with original signal.
        for(int n = 0; n < size_; ++n)
        {
            double angle = -2.0 * M_PI * k * n / size_;
            complex twiddle(cos(angle), sin(angle));
            sum += numbers_[n] * twiddle;
        }
        result_.emplace_back(sum);        
    }
}

void DFT::Print()
{
    PrintFormattedVector(result_);
    /*
    for(int i = 0; i < size_; ++i)
    {
        string op = result_[i].imag() < 0 ? " - " : " + ";        
        //double imag_val = res.imag() < 0 ? -res.imag() : res.imag();
        std::cout << std::fixed << std::setprecision(1) 
              << result_[i].real() << op
              << result_[i].imag() << "i" << std::endl;
    }
              */
    
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