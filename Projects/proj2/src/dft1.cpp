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

CVector DFT::Execute()
{

}

void DFT::Print()
{

}