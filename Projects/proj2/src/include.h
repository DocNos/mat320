#pragma once

#include <string.h>
#include <complex.h>

using string = std::string;
using complex = std::complex;

comVec Read(string filename)
{
    string line;
    comVec numbers;
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr << "Error opening file " << filename << std::endl;
        return numbers;
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
    return numbers;
}