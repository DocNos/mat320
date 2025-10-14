#include "timing.h"

void Read(string filename)
{
    string line;
    std::ifstream file(filename);
    c_vector numbers;
    if(!file.is_open()){
        std::cerr << "Error opening file " << filename << std::endl;
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
            numbers.emplace_back(num);
            // std::cout << "Read from " << filename << " "
            // << num << std::endl;
        }
    }

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