#include <iostream>
#include <string>
#include <cstdlib>
#include "complex_operations.h"

int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }
    switch(atoi(argv[1]))
    {
        case(1):{
            Rotate rot(atoi(argv[2]), atof(argv[3]), argv[4]);
            comVec result = Read(rot.filename_);
            rot.execute();
        } break;
        case(2):{
            SumUnity sum(atoi(argv[2]), atoi(argv[3]));
            sum.execute();
        }break;
        case(3):{
            //InnerProd inner(null, null, argv[3])
        }break;
        case(4):{

        }break;
        default: return;
    }

}

