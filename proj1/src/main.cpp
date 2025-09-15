#include <iostream>
#include <string>
#include <cstdlib>
#include "complex_operations.h"

int main(int argc, char *argv[])
{
    // Extract executable name from argv[0]
    std::string exe_name = argv[0];
    size_t last_slash = exe_name.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        exe_name = exe_name.substr(last_slash + 1);
    }
    
    int part;
    int arg_offset;
    
    // Determine part based on executable name
    if (exe_name == "rot") {
        part = 1;
        arg_offset = 0;
    } else if (exe_name == "sum") {
        part = 2;
        arg_offset = 0;
    } else if (exe_name == "dot") {
        part = 3;
        arg_offset = 0;
    } else if (exe_name == "prod") {
        part = 4;
        arg_offset = 0;
    } else {
        // Combined executable mode - part number from command line
        if(argc < 2){
            std::cerr << "Not enough arguments" << std::endl;
            return 1;
        }
        part = atoi(argv[1]);
        arg_offset = 1;
    }

    switch(part)
    {
        case(1):{
            Rotate rot(atoi(argv[1+arg_offset]), atof(argv[2+arg_offset]), Read(argv[3+arg_offset]));
            rot.execute();
            rot.print();
        } break;
        case(2):{
            SumUnity sum(atoi(argv[1+arg_offset]), atoi(argv[2+arg_offset]));
            sum.execute();
            sum.print();
        }break;
        case(3):{
            InnerProd inner(atoi(argv[1+arg_offset]), Read(argv[2+arg_offset]), Read(argv[3+arg_offset]));
            inner.execute();
            inner.print();
        }break;
        case(4):{
            InnerUnity unity( atoi(argv[1+arg_offset]), Read(argv[2+arg_offset]));
            unity.execute();
            unity.print();
        }break;
        default: return 0;
    }
    return 0;
}

