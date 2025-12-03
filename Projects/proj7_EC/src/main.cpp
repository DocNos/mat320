#include <iostream>
#include "filter.h"
#include "wav.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    string buzzPreset = argv[1];
    string resonPreset = argv[2];
    ResonFilter res(buzzPreset, resonPreset);
    res.Execute();
    string outFile = "output/" + buzzPreset 
    + "_" + resonPreset + ".wav";
    res.Write(outFile);
    cout << "Audio generation complete!" << endl;

    return 0;
}