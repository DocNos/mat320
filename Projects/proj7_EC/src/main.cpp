#include <iostream>
#include "filter.h"
#include "wav.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    if(argc != 3) 
    {
        cerr << "requires 3 arguments" << endl;
        return 1;
    }
    
    string buzzPreset = argv[1];
    if(!FindBuzz(buzzPreset)) 
    {
        cerr << "Buzz preset " << buzzPreset << " does not exist" << endl;
        return 1;
    }
    string resonPreset = argv[2];
    if(!FindReson(resonPreset))
    {
        cerr << "Reson preset " << resonPreset << " does not exist" << endl;
        return 1;
    }
    ResonFilter res(buzzPreset, resonPreset);
    res.Execute();
    string outFile = "output/" + buzzPreset 
    + "_" + resonPreset + ".wav";
    res.Write(outFile);
    cout << "Audio generation complete!" << endl;

    return 0;
}