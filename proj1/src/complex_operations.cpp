#include <iostream>
#include "complex_operations.h"

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
// Rotate ///////////////////////////////////////////////////////////////
void Rotate::execute(){
    for(auto vec : data_)
    {
        result_.emplace_back(calculate(vec));
    }
}
// Input complex vector in a+bi form
// e^i*θ = cosθ + i*sinθ
complex Rotate::calculate(complex input){
    double theta = 2.0 * M_PI * angleScalar_;
    complex result(cos(theta), sin(theta));
    complex mult = input * result;
    //double realTolerance = abs(mult.real()) < 1e-20 ? 0.0 : mult.real();
    //double imagTolerance = abs(mult.imag()) < 1e-20 ? 0.0 : mult.imag();
    return mult;//complex(realTolerance, imagTolerance);
}

void Rotate::print(){
    for(auto res: result_)
    {
        //double realVal = res.real() + 0.0;
        //double imagVal = res.imag() + 0.0;
        string op = res.imag() < 0 ? " - " : " + ";        
        double imag_val = res.imag() < 0 ? -res.imag() : res.imag();
        std::cout << std::fixed << std::setprecision(1) 
                  << res.real() << op << imag_val << "i" << std::endl;
    }
}

///////////////////////////////////////////////////////////////
// Sum Unity //////////////////////////////////////////////////


// 1 + e^(i2pi/N) +(e^(i2pi/N))^2 + ... +(e^(i2pi/N))^k-1
void SumUnity::execute(){
    //complex sum;
    //std::cout<<result_ << std::endl;
    for(int i = 0; i < length_; ++i)
    {        
        double theta = (2.0 * M_PI * i) / root_;
        double real = cos(theta);
        double imag = sin(theta);
        complex val(real, imag);
        
        result_ += val;
        
    }
}

void SumUnity::print(){
    

    std::cout << std::fixed << std::setprecision(2) 
    << root_ << "th root, "<< length_ <<
     " powers:" << std::endl << result_.real() << " + " << result_.imag()
     << "i" << std::endl;
    //       
        //double imag_val = result_.imag() < 0 ? -result_.imag() : result_.imag();
        //std::cout //<< std::fixed << std::setprecision(1) 
          //        << result_.real() << op << result_.imag() << "i" << std::endl;
    
}

///////////////////////////////////////////////////////////////
// Inner Product //////////////////////////////////////////////

void InnerProd::execute(){

}


///////////////////////////////////////////////////////////////
// Inner Unity ////////////////////////////////////////////////

void InnerUnity::execute(){

}

