#include "wavAPI.h"

int main(int argc, char* argv[])
{
    Pluck filterPercussion(GetPreset(0));
    Pluck filterLead(GetPreset(1));
    Pluck filterMelody(GetPreset(2));
    vector<int16_t> percussionOut = filterPercussion.Execute();
    vector<int16_t> leadOut = filterLead.Execute();
    vector<int16_t> melodyOut = filterMelody.Execute();

    vector<vector<int16_t>> voices = { percussionOut, leadOut, melodyOut};
    vector<int16_t> mixed = Mix(voices, filterPercussion.numSamples_);

    
    WriteWav("output/mixed.wav", filterLead.header_, mixed);
    return 0;
}