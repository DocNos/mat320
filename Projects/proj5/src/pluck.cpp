#include "pluck.h"


void Pluck::Delay()
{

}

void Pluck::Lowpass()
{

}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cerr << "no frequency found" << endl;
        return 1;
    }
    Pluck newPluck(8, stof(argv[1]));

    return 0;
}