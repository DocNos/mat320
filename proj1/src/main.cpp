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
            Rotate rot(atoi(argv[2]), atof(argv[3]), Read(argv[4]));
            rot.execute();
            rot.print();
        } break;
        case(2):{
            SumUnity sum(atoi(argv[2]), atoi(argv[3]));
            sum.execute();
        }break;
        case(3):{
            InnerProd inner(atoi(argv[2]), Read(argv[3]), Read(argv[4]));
            inner.execute();
        }break;
        case(4):{
            InnerUnity unity( atoi(argv[2]), atoi(argv[3]), Read(argv[4]));
            unity.execute();
        }break;
        default: return 0;
    }
    return 0;
}

