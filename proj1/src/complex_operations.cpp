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
        std::istringstream stream(line);
        //stream.str(line);
        if(stream >> num){
            numbers.emplace_back(num);
        }
    }
    return numbers;
}